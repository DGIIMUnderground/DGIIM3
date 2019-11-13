// *****************************************************************************
//
// Urgent wait Hoare-like monitors for C++11
// (private implementation)
// Copyright (C) 2017  Carlos Ureña  (University of Granada)
//
// History:
//
// November 2016 : created
// March 2017    : updated
// Oct-Nov 2017  : using shared_ptr for monitors references
// November 2017 : using guranteed FIFO queues for urgent and monitor queues
// December 2017 : added TODO above, about implementation
// July 2018     : changed implementation to use FIFO semaphores, as described in
//                 Hoare's article.
//
// References:
//
//  This implementation is based on the design (using semaphores) introduced by Hoare:
//
//  Hoare, C. A. R. (October 1974).
//  "Monitors: an operating system structuring concept".
//  Comm. ACM. 17 (10): 549–557.
//  doi: 10.1145/355620.361161.
//  PDF: https://www.classes.cs.uchicago.edu/archive/2017/fall/33100-1/papers/hoare-monitors.pdf
//
// also described here:
//
// Operating System Concepts, by Abraham Silberschatz, Peter B. Galvin, Greg Gagne
// (and briefly commented here:
//  https://softwareengineering.stackexchange.com/questions/334811/implement-a-monitor-in-terms-of-semaphores
//
// This program uses the "execute around" pattern designed by B.Stroustroup, as described here:
//
//    Wrapping C++ Member Function Calls.
//    Bjarne Stroustrup
//    The C++ Report, June 2000.
//    http://www.stroustrup.com/wrapper.pdf
//
// The code is also inspired in a Java implementation for monitors described here:
//
// Better monitors for Java - By Theodore S. Norvell
// http://www.javaworld.com/article/2077769/core-java/better-monitors-for-java.html
//
// without the need for "enter" or "leave" calls, which are automatically done by
// using the wrapper pattern.
//
//
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// *****************************************************************************

#include <iostream>
#include <cassert>
#include <thread>  // incluye std::this_thread::get_id()
#include <system_error>
#include "HoareMonitor.h"

namespace HM
{

std::mutex mcout ;
using namespace std ;


// *****************************************************************************
//
// Class: CondVar
//
// *****************************************************************************

// create an unusable, not initialized condition variable


CondVar::CondVar()
{
   monitor = nullptr ;
}

// -----------------------------------------------------------------------------
//  create an usable condition variable

CondVar::CondVar( HoareMonitor * p_monitor, unsigned p_index )
{
   assert( p_monitor != nullptr );
   assert( p_index == (p_monitor->cond_sem).size()-1 );

   monitor = p_monitor ;
   index   = p_index ;
}
// -----------------------------------------------------------------------------
static const char * const msg =
"error: intento de usar una variable condición que no está inicializada con la función 'newCondVar()' en el constructor del monitor." ;
// "error: trying to use a condition variable which is not properly initialized with 'newCondVar()' function" ;

void CondVar::check( )
{
   if ( monitor == nullptr )
   {
      cout << msg << endl ;
      exit(1);
   }
}
// -----------------------------------------------------------------------------
// unconditionally wait on the underlying thread queue

void CondVar::wait()
{
   check();
   monitor->wait( index ) ;
}
// -----------------------------------------------------------------------------
// signal operation, with "urgent wait" semantics

void CondVar::signal()
{
   check();
   monitor->signal( index );
}
// -----------------------------------------------------------------------------
// returns number of threads waiting in the cond.var.

unsigned CondVar::get_nwt()
{
   check();
   return monitor->get_nwt( index );
}

// *****************************************************************************
//
// Class: HoareMonitor
//
// *****************************************************************************

void HoareMonitor::initialize()
{
   running    = false ;
   urgent_sem = new Semaphore( 0, "urgent" );
   mutex_sem  = new Semaphore( 1, "monitor mutex semaphore" ); // initially open

}
// -----------------------------------------------------------------------------
HoareMonitor::HoareMonitor()
{
   name = "unknown" ;
   initialize();
}
// -----------------------------------------------------------------------------

HoareMonitor::HoareMonitor( const std::string & p_name )
{
   name = p_name ;
   initialize();
}
// -----------------------------------------------------------------------------
HoareMonitor::~HoareMonitor()
{
   //cout << "starts monitor destructor" << endl ;

   assert( ! running );

   // assert no threads are waiting
   assert( urgent_count == 0 );

   for( auto & cn : cond_count )
      assert( cn == 0  );

   // destroy all threads queues
   for( auto & sem_ptr : cond_sem )
   {
      assert( sem_ptr != nullptr );
      delete sem_ptr ;
   }

   assert( urgent_count == 0 );
   assert( urgent_sem != nullptr );
   delete urgent_sem ;
   urgent_sem = nullptr ;

   assert( mutex_sem != nullptr );
   delete mutex_sem ;
   mutex_sem = nullptr ;


   //cout << "ends monitor destructor" << endl ;
}
// -----------------------------------------------------------------------------

CondVar HoareMonitor::newCondVar()
{
   cond_sem.push_back( new Semaphore( 0 ) ); // add semaphore to monitor
   cond_count.push_back( 0 );                // add the thread count
   return CondVar( this, cond_sem.size()-1 );  // built and return cond.var.
}

// -----------------------------------------------------------------------------
// enter the monitor, waiting if neccesary

void HoareMonitor::enter()
{
   assert( mutex_sem != nullptr );
  // acquire queues access mutex
  mutex_sem->sem_wait(  );


  // register this thread is running in the monitor
  assert( ! running );
  running = true ;
  running_thread_id = std::this_thread::get_id();
}
// -----------------------------------------------------------------------------
// end running monitor code

void HoareMonitor::leave()
{
   // check this is the thread running in the monitor
   assert( running );
   assert( std::this_thread::get_id() == running_thread_id );

   // release one thread or either unlock monitor mutex
   allow_another_to_enter();
}
// -----------------------------------------------------------------------------
// allow a waiting thread to enter the monitor, if any

void HoareMonitor::allow_another_to_enter()
{
   running = false ;
   if ( urgent_count > 0 )  // if any thread in the urgent queue
      urgent_sem->sem_signal();          //   release one, allow it to enter (remains closed)
   else                                // if no thread in the urgent
      mutex_sem->sem_signal();         //   signal the monitor queue
}
// -----------------------------------------------------------------------------
// wait on a queue

void HoareMonitor::wait( unsigned q_index )
{
   // check this is the thread running in the monitor
   assert( running );
   assert( std::this_thread::get_id() == running_thread_id );

   // check 'q_index' is a valid queue index, and that a semaphore is there
   assert( q_index < cond_sem.size() );
   assert( cond_sem[q_index] != nullptr );

   // register a new thread is waiting
   cond_count[q_index] ++ ;

   // allow another thread to start or continue running in the monitor, if any is waiting
   allow_another_to_enter();

   // blocked wait on the condition threads queue
   cond_sem[q_index]->sem_wait( );

   // register the thread is no longer waiting
   assert( cond_count[q_index] > 0 );
   cond_count[q_index] -- ;

   // re-enter the monitor: register this is the thread running in the monitor
   running = true ;
   running_thread_id = std::this_thread::get_id();
}

// -----------------------------------------------------------------------------
// signal with urgent wait semantics on a user-defined variable condition

void HoareMonitor::signal( unsigned q_index )
{
   // check this is the thread running in the monitor
   assert( running );
   assert( std::this_thread::get_id() == running_thread_id );

   // check 'q_index' is a valid queue index, and that a semaphore is there
   assert( q_index < cond_sem.size() );
   assert( cond_sem[q_index] != nullptr );

   // if there is any thread in the queue, signal it and enter urgent queue
   urgent_count ++ ;
   if ( cond_count[q_index] >  0 )
   {
      cond_sem[q_index]->sem_signal() ;
      urgent_sem->sem_wait( );
   }
   urgent_count -- ;

   // register this is the running thread
   running = true ;
   running_thread_id = std::this_thread::get_id();

}
// -----------------------------------------------------------------------------
// returns number of waiting threads in a queue (associated to a user-defined cv)

unsigned HoareMonitor::get_nwt( unsigned q_index )
{
  assert( running );
  assert( std::this_thread::get_id() == running_thread_id );
  assert( q_index < cond_sem.size() );
  assert( cond_sem[q_index] != nullptr );

  return cond_count[q_index] ;
}

/// ***************************************

// names map, updated in registerThreadName
static std::map< std::thread::id, std::string > names_map ;

// mutex used to access the names map
static std::mutex names_mtx ;

// -----------------------------------------------------------------------------
// register calling thread name in the monitor, useful for debugging

void register_thread_name( const std::string & rol, const int number )
{
  const std::string name = rol + " " + std::to_string(number) ;
  register_thread_name( name );
}
// -----------------------------------------------------------------------------

void register_thread_name( const std::string & name )
{
  std::unique_lock<std::mutex> lock( names_mtx );
  // get thread id in ttid
  const std::thread::id ttid = std::this_thread::get_id() ; // this thread ident.

  // search and abort if already registered
  const auto iter = names_map.find( ttid );
  if ( iter != names_map.end() )
  {
    logM("that id was already registered, with name == '" << iter->second << "', aborting");
    exit(1);
  }

  // insert name
  names_map.insert( std::pair< std::thread::id, std::string>( ttid, name ) );
}

// -----------------------------------------------------------------------------
// get this thread registered name (or "unknown" if not registered)
std::string get_thread_name()
{
  std::unique_lock<std::mutex> lock( names_mtx );

  const auto ttid = std::this_thread::get_id() ; // this thread ident.
  const auto iter = names_map.find( ttid );

  if ( iter != names_map.end() )
    return iter->second ;
  else
    return "(unknown)" ;

}

// *****************************************************************************


} //  fin namespace HoareMonitors
