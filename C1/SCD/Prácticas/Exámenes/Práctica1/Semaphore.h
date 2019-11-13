// *****************************************************************************
//
// Semaphores implementation using C++11 concurrency features.
// (public interface declarations)
// Copyright (C) 2017  Carlos Ureña Almagro
//
// April, 2017: created
// Sept, 15, 2017: removed reference count, now 'std::shared_ptr' is used instead
// July, 15, 2018: implemented guaranteed FIFO order
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

#ifndef SEMAPHORES_HPP
#define SEMAPHORES_HPP

#include <memory>   // (for std::shared_ptr)
#include <mutex>
#include <condition_variable>
#include <string>
#include <cassert>

namespace SEM
{

// incomplete declaration of the implementation class
class SemaphoreRepr ;

// *****************************************************************************
// Classic semaphore objects
// it is a reference to a Semaphore implementation object

class Semaphore
{
   public:

   // initialization
   Semaphore( unsigned init_value );
   Semaphore( unsigned init_value, const std::string & p_name );

   // dissallow asignements
   void operator = (Semaphore & sem ) = delete ;

   // allows to initialize a semaphore by using another (a new alias is created)
   Semaphore( const Semaphore & sem ) ;

   // operations (member methods)
   void     sem_wait();
   void     sem_signal() ;

   // operations (non member functions)
   friend inline void sem_wait  ( Semaphore & s ) { s.sem_wait()  ; }
   friend inline void sem_signal( Semaphore & s ) { s.sem_signal(); }

   private:
   std::shared_ptr<SemaphoreRepr>  repr; // pointer to semaphore implementation
} ;

}      // end namespace SCD
#endif // ifndef SEMAPHORES_HPP
