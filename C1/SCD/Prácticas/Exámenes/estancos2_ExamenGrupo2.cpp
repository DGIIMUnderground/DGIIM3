/**
   @autor Blanca

@brief a partir del seminario de la práctica de los estancos añadir una suministradora que coloque en un palé los ingredientes para una vez de un estanquero
   
 */

#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "Semaphore.h"

using namespace std ;
using namespace SEM ;


//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos valores enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}


const int numEstanqueros= 1, numFumadores=3; //cada fumador consume un tipo concreto de producto, un estanquero puede vender cualquiera

mutex mtxCout; 

class Monitor {

private:
  // PALÉ DEL SUMINISTRADOR
  static const int tamPila=3;
  
  struct Pila {

    mutex mtxPila;
  
    int p [ tamPila ];
    int ind=0; //apunta siempre a la última posición vacía 
    const int ERROR=-1;
  

    void escribir ( int n) {
      mtxPila.lock();
      if ( ind < tamPila ) {
	p [ind++]=n; 
      }
      mtxPila.unlock();
    
    }

    int leer() {

      mtxPila.lock();
      int ret=(ind > 0 )?p[ --ind] : ERROR ;
      mtxPila.unlock();
      return ret; 
    }
  
  };

  Pila pale;


  //inicializamos estanqueros y fumadores sin suministros
  int numLote=0; 
  Semaphore sReponer = tamPila;  //huecos vacío de la pila de lotes 
  Semaphore sMostrador = 1; //mostrador vacío indica 1
  Semaphore estanquero[numEstanqueros]={0};
  Semaphore sFumador[numFumadores]=  {0,0,0}; 
  
public:

  /**
     Repone a un estanquero aleatorio @param num_suministros un número concreto de elementos
   */
  void suministra( ) {

    sem_wait( sReponer );
    
    //tiempo que tarda de gestion
    chrono::milliseconds duracion_reponer( aleatorio<20,200>() );
    this_thread::sleep_for( duracion_reponer);
    mtxCout.lock();
    cout << "La suministradora añande el lote " <<numLote<< " a la pila " << endl << flush;
    mtxCout.unlock();

    pale.escribir( numLote++); 
     
     

    //indicamos al estanquero nuevos suministros en el palé
    sem_signal(estanquero[0] );   
  }

  /**
     @brief el estanquer @param n_estanquero repone en el mostrado
   */
  void poner( int n_estanquero) {
    
    sem_wait( estanquero[n_estanquero]); // tiene sumisnistros 
    sem_wait( sMostrador); // comprobamos que esté vacío

    mtxCout.lock();
    cout << " Estanquero coge el lote  "<< pale.leer()<< endl<<flush;
    mtxCout.unlock();
    sem_signal( sReponer);
    
    int elemento = aleatorio <0,(numFumadores-1)>();

    //tiempo que tarda en reponer
    chrono::milliseconds duracion_poner( aleatorio<20,200>() );
    this_thread::sleep_for( duracion_poner);
    mtxCout.lock();
    cout << "Estanquero coloca elemento " << elemento << " en el mostrador "<< endl<<flush;
    mtxCout.unlock();
     
    //activamos fumador correspondiente
    sem_signal( sFumador[elemento] );
    
    //veamos si se necesita volver a reponer

    
  }

  void fumador ( int n_fumador) {

    sem_wait ( sFumador[n_fumador]); // hay en el mostrador algo para él

    mtxCout.lock();
    cout << "Fumador " << n_fumador << " coge objeto del mostrador" << endl; 
    sem_signal( sMostrador);
    mtxCout.unlock();
    
    // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );

   // informa de que comienza a fumar
   mtxCout.lock();
   cout << "fumador " << n_fumador << " empieza a fumar"<< endl;
   mtxCout.unlock();
   
   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );
   
   mtxCout.lock();
   cout << "Fumador " << n_fumador 	<< " ha terminado fumar ("
	<<duracion_fumar.count()<< " ms)"<< endl;
   mtxCout.unlock();
   
	       
  }

  /**
     @brief Determina si se ha cumplido cierta condición 
   */

  bool fin() {
    //Como el enunciado pide un bucle infinito en esta condición aparecerá devolverá siempre que nunca aca
    bool ret = false; 
    return ret; 
  }
  
}; 

Monitor gestor;

//----- funciones  hebra --------

void funcion_hebra_fumador( int id) {
  while ( !gestor.fin()) {  // gestor.fin() siempre devuelve false ya que nos están pidiendo un bucle infinito
    gestor.fumador(id); 
  }

  
  mtxCout.lock();
  cout << "\t\t\tFIN fumador " << id << " se cansa de fumar"<< endl; 
  mtxCout.unlock();
}

void funcion_hebra_estanquero( int id ) {
  while (!gestor.fin())
    gestor.poner( id);

  
  mtxCout.lock();
  cout << "\t\t\tFIN Estanquero "<< id << " ya no le quedan recursos"<< endl; mtxCout.unlock();
}

void funcion_hebra_gestor() {
  while (!gestor.fin() )
    gestor.suministra();


  git 
  mtxCout.lock();
  cout << "\t\t\tFIN El gestor ya no tiene más abastos" << endl;
  mtxCout.unlock();
}
  

// ------------- main --------------


int main() {

  //creamos hebras 
  thread suministradora ( funcion_hebra_gestor);

  thread hEstanquero[ numEstanqueros],hFumadores[numFumadores];
  for ( int i=0; i<numEstanqueros; i++)
    hEstanquero[i] = thread( funcion_hebra_estanquero, i);

   for ( int i=0; i<numFumadores; i++)
    hFumadores[i] = thread( funcion_hebra_fumador, i);


  suministradora.join(); 
}
