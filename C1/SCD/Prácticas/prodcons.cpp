// @author: Asmilex

#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <atomic>
#include <random>
#include "Semaphore.h"

using namespace std ;
using namespace SEM ;


const     int num_items        = 40,    // número de items
          tam_vec              = 3;    // tamaño del buffer
unsigned  cont_prod[num_items] = {0},   // contadores de verificación: producidos
cont_cons[num_items]           = {0};   // contadores de verificación: consumidos


const int num_productores = 4,
      num_consumidores    = 4;

// ────────────────────────────────────────────────────────────────────────────────

    /*
        Plantilla de función para generar un entero aleatorio uniformemente
        distribuido entre dos valores enteros, ambos incluidos
        (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
    */

    template< int min, int max > int aleatorio() {
        static default_random_engine generador( (random_device())() );
        static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
        return distribucion_uniforme( generador );
    }

//
// ──────────────────────────────────────────────────────── FUNCIONES COMUNES ─────
//

    int producir_dato (int indice) {
        static int contador = 0 ;
        this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

        cout << "producido: " << contador << " por " << indice << endl << flush;

        cont_prod[contador]++ ;
        return contador++ ;
    }


    void consumir_dato( unsigned dato, int indice ) {
        assert( dato < num_items );
        cont_cons[dato] ++ ;
        this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

        cout << "                  consumido: " << dato << " por " <<  indice << endl << flush;
    }


// ────────────────────────────────────────────────────────────────────────────────


    void test_contadores() {
        bool ok = true ;
        cout << "comprobando contadores ...." ;

        for( unsigned i = 0 ; i < num_items ; i++ ) {
            if ( cont_prod[i] != 1 ) {
                cout << "error: valor " << i << " producido " << cont_prod[i] << " veces." << endl;
                ok = false ;
            }
            if ( cont_cons[i] != 1 ) {
                cout << "error: valor " << i << " consumido " << cont_cons[i] << " veces" << endl ;
                ok = false ;
            }
        }

        if (ok)
            cout << endl << flush << "solución (aparentemente) correcta." << endl << flush ;
    }

//
// ──────────────────────────────────────────────────────────── SOLUCION LIFO ─────
//

    Semaphore can_consume_LIFO = 0;
    Semaphore can_produce_LIFO = tam_vec;

    int buffer[tam_vec];
    atomic<int> index_LIFO;


    void  funcion_hebra_productora_LIFO (int index) {
        for( unsigned i = index ; i < num_items; i += num_productores ) {
            int dato = producir_dato(index);

            can_produce_LIFO.sem_wait();

            buffer[index_LIFO++] = dato;

            can_consume_LIFO.sem_signal();
        }
    }


    void funcion_hebra_consumidora_LIFO (int index) {
        for( unsigned i = index ; i < num_items ; i+=num_consumidores ) {
            int dato;

            can_consume_LIFO.sem_wait();

            dato = buffer[--index_LIFO];

            can_produce_LIFO.sem_signal();

            consumir_dato( dato, index ) ;
        }
    }

//
// ──────────────────────────────────────────────────────────── SOLUCION FIFO ─────
//

    Semaphore can_consume_FIFO = 0;
    Semaphore can_produce_FIFO = tam_vec;

    int buffer_FIFO[tam_vec];
    atomic<int> next_store, next_read;


    void funcion_hebra_productora_FIFO (int index) {
        int next_index;

        for( unsigned i = index ; i < num_items ; i+=num_productores ) {
            int dato = producir_dato(index);

            can_produce_FIFO.sem_wait();

            buffer_FIFO[next_store] = dato;
            next_index = (next_store + 1) % tam_vec;
            next_store.store(next_index, memory_order::memory_order_acquire);

            can_consume_FIFO.sem_signal();
        }
    }


    void funcion_hebra_consumidora_FIFO (int index) {
        int next_index;

        for( unsigned i = index ; i < num_items ; i+=num_consumidores ) {
            int dato;

            can_consume_FIFO.sem_wait();

            dato = buffer_FIFO[next_read];
            next_index = (next_read + 1) % tam_vec;
            next_read.store(next_index, memory_order::memory_order_acquire);

            can_produce_FIFO.sem_signal();

            consumir_dato( dato, index ) ;
        }
    }


// ────────────────────────────────────────────────────────────────────────────────


    int main(int argc, char const *argv[]) {
        int choice = 0;

        while (choice != 1 && choice != 2) {
            cout << "¿LIFO (1) o FIFO? (2):\t";
            cin >> choice;
        }

        thread productores[num_productores];
        thread consumidores[num_consumidores];


        if (choice == 1)
        {
            cout << "--------------------------------------------------------"  << endl
                 << "Problema de los productores-consumidores (solución LIFO)." << endl
                 << "--------------------------------------------------------"  << endl
                 << flush ;

            for (int i = 0; i < num_consumidores; i++)
                consumidores[i] = thread(funcion_hebra_consumidora_LIFO, i);

            for (int i = 0; i < num_productores; i++)
                productores[i] = thread(funcion_hebra_productora_LIFO, i);

        }
        else if (choice == 2)
        {
            cout << "--------------------------------------------------------"  << endl
                 << "Problema de los productores-consumidores (solución FIFO)." << endl
                 << "--------------------------------------------------------"  << endl
                 << flush ;


            for (int i = 0; i < num_consumidores; i++)
                consumidores[i] = thread(funcion_hebra_consumidora_FIFO, i);

            for (int i = 0; i < num_productores; i++)
                productores[i] = thread(funcion_hebra_productora_FIFO, i);

        }

        for (int i = 0; i < num_consumidores; i++)
            consumidores[i].join();

        for (int i = 0; i < num_productores; i++)
            productores[i].join();

        test_contadores();
    }
