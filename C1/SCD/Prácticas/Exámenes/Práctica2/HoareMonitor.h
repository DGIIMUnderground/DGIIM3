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

//
//
//
// Declarations.
//
// *****************************************************************************

#ifndef HOARE_MONITORS_HPP
#define HOARE_MONITORS_HPP

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <cassert>
#include <vector>
#include <map>
#include <thread>  // thread
#include <memory> // shared_ptr, make_shared

#include "Semaphore.h"

using namespace SEM ;

#define USE_FIFO_QUEUES


// uncomment to get a log
//#define TRAZA_M

namespace HM
{

using namespace std ;
class HoareMonitor ;
class ThreadsQueue ;
class ThreadsQueueFIFO ;
template<class T> class Call_proxy ;

// *****************************************************************************
//
// Class: CondVar
//
// a class for conditions variables with "urgent wait" semantics on signal
// used in Hoare Monitors
// only to be used from  HoareMonitor class
//
// *****************************************************************************

class CondVar
{
   public:

   void     wait();     // unconditionally wait on the underlying thread queue
   void     signal();   // signal operation, with "urgent wait" semantics
   unsigned get_nwt() ; // returns number of threads waiting in the cond.var.

   bool empty() { return get_nwt() == 0 ; }

   // create an un-initialized un-usable condition variable
   CondVar();

   // --------------------------------------------------------------------------
   private:

   // forward declaration of friend monitor class
   friend class HoareMonitor ;

   // instance variables:
   HoareMonitor * monitor = nullptr; // reference to the monitor for this variable
   unsigned       index   = 0;       // index of the corresponding threads queue in
                                     // the monitor threads queues vector

   // private constructor, only to be used from inside monitor implementation
   CondVar( HoareMonitor * p_monitor, unsigned p_index ) ;

   // check whether this v.c. has been properly created, abort when not.
   void check();
};

// ************************************
// functions used for debug: allow to print a thread name if registered

// register calling thread name
void register_thread_name( const std::string & name );
void register_thread_name( const std::string & rol, const int num );

// get calling thread registered name (or "unknown" if not registered)
std::string get_thread_name()  ;

// *****************************************************************************
//
// Class: HoareMonitor
//
// Base class for classic Hoare-style monitors
// (with "urgent wait" signal semantics)
//
// *****************************************************************************

class HoareMonitor
{
   public:

   // --------------------------------------------------------------------------
   protected:  // methods to be called from derived classes (concrete monitors)

   // constructors and destructor
   HoareMonitor() ;
   HoareMonitor( const std::string & p_name ) ;
   ~HoareMonitor();

   // create a new condition variable in this monitor
   CondVar newCondVar() ;

   // --------------------------------------------------------------------------
   private:

   // number of references (from MRef objects) pointing here
   // (must be atomic because it can be incremented/decremented by multiple threads)
   //atomic<int> reference_count ;

   // allow friend classes to access private parts of this class
   template<typename MonClass> friend class Call_proxy ;
   template<typename MonClass> friend class MRef ;
   friend class CondVar ;

   // name of this monitor (useful for debugging)
   std::string name ;

   // true iif any thread is running in the monitor
   bool running ;

   // identifier for thread currently in the monitor (when running==true)
   std::thread::id running_thread_id ;

   // queue for threads waiting to enter the monitor
   Semaphore * mutex_sem;

   // queue for threads waiting to re-enter the monitor after signal
   Semaphore * urgent_sem ;       // semaphore for urgent processes
   unsigned    urgent_count ; // number of threads waiting in urgent semaphore

   // vector with all queues for user defined condition variables
   std::vector<Semaphore *> cond_sem ;   // semaphore with the queues
   std::vector<unsigned>    cond_count ; // number of threads waiting in each cond.var.


   // enter and leave the monitor
   void enter();
   void leave();

   // initialize the monitor just after creation
   void initialize();

   // wait, signal and query on user-defined condition variables
   // (q_index is the index of the corresponding queue in the queues table)
   void     wait   ( unsigned q_index );
   void     signal ( unsigned q_index );
   unsigned get_nwt( unsigned q_index );

   // allow a waiting thread to enter the monitor
   void allow_another_to_enter() ;
} ;

// *****************************************************************************
extern std::mutex mcout ;

#ifdef TRAZA_M
#define logM( msg )     \
   {                   \
        mcout.lock();  \
        std::cout << msg << std::endl << std::flush ; \
        mcout.unlock(); \
   }
#else
#define logM( msg )
#endif

#define logEnt()   logM( "" << __FUNCTION__ << ": inicio (línea " << __LINE__ << ", archivo " << __FILE__ << ")" )

// *****************************************************************************
//
// Class: MRef
//
// reference to a monitor.
// all monitors should be accesed through these references
//
// implements the "execute around" pattern
// as described here:
//    Wrapping C++ Member Function Calls.
//    Bjarne Stroustrup
//    The C++ Report, June 2000.
//    http://www.stroustrup.com/wrapper.pdf
//
// *****************************************************************************

template<class MonClass> class MRef
{
   private:
   shared_ptr<MonClass> monPtr ; // shared pointer to the monitor

   public:

   // create a reference from a shared_ptr
   inline MRef( shared_ptr<MonClass>  p_monPtr )
   {
      assert( p_monPtr != nullptr );
      monPtr = p_monPtr ;
      logM( "inicio MRef( monitor * )  : rc == " << monPtr->reference_count  );
   }

   // obtain a call proxy through the dereference operator
   inline Call_proxy<MonClass>  operator -> ()
   {
     assert( monPtr != nullptr );
     return Call_proxy<MonClass>( *monPtr ) ; // acquires mutual exclusion
   }
} ;

// -----------------------------------------------------------------------------
// creation of a monitor reference by using a list of
// actual parameters (the list must match a monitor constructor parameters list)

template< class MonClass, class... Args > inline
MRef<MonClass> Create( Args &&... args )
{
   // equivalent to 'new'
   return MRef<MonClass>( make_shared<MonClass>( args... ) );
}

// *****************************************************************************
//
// Class Call_proxy<...>
//
// *****************************************************************************

template<class MonClass> class Call_proxy
{
   private:
   MonClass & monRef ; // monitor reference

   public:
   inline Call_proxy( MonClass & mr ) : monRef(mr) { monRef.enter(); }
   inline MonClass * operator -> () { return &monRef; }
   inline ~Call_proxy() { monRef.leave(); }
};

} // namespace HW end

// *****************************************************************************

#endif // ifndef MONITORESHSU_HPP
