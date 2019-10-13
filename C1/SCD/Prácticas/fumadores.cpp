// @author: Asmilex

#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include "Semaphore.h"

using namespace std;
using namespace SEM;

const int num_fumadores = 3;


//
// ─────────────────────────────────────────────────── ESTRUCTURAS AUXILIARES ─────
//

    class Recursos {
    private:

        Semaphore estanquero = 1;
        Semaphore vector_recursos[num_fumadores] = {0, 0, 0};

    public:
        auto pedir (int tipo) -> void {
            vector_recursos[tipo].sem_wait();

            estanquero.sem_signal();
        };

        auto proveer (int tipo) -> void {
            estanquero.sem_wait();

            cout << "Estanquero repone " << tipo << endl << flush;

            vector_recursos[tipo].sem_signal();
        };
    };

//
// ──────────────────────────────────────────────────── FUNCIONES PRINCIPALES ─────
//

    /*
        Plantilla de función para generar un entero aleatorio uniformemente
        distribuido entre dos valores enteros, ambos incluidos
        (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
    */

    template< int min, int max > int aleatorio () {
        static default_random_engine generador( (random_device())() );
        static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
        return distribucion_uniforme( generador );
    }


// ────────────────────────────────────────────────────────────────────────────────


    Recursos mostrador;

    // función que ejecuta la hebra del estanquero
    void funcion_hebra_estanquero () {
        while (true) {
            mostrador.proveer(aleatorio<0, 2>());
        }
    }


    // Función que simula la acción de fumar, como un retardo aleatoria de la hebra
    void fumar (int num_fumador) {
        // calcular milisegundos aleatorios de duración de la acción de fumar)
        chrono::milliseconds duracion_fumar( aleatorio<20,200>() );

        // informa de que comienza a fumar
        cout << "Fumador " << num_fumador << "  :"
             << " empieza a fumar (" << duracion_fumar.count() << " milisegundos)" << endl << flush;

        // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
        this_thread::sleep_for( duracion_fumar );

        // informa de que ha terminado de fumar
        cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl << flush;
    }


    // función que ejecuta la hebra del fumador
    void funcion_hebra_fumador (int num_fumador) {
        /*
            Fumador 0: necesita cerillas
            Fumador 1: necesita tabaco
            Fumador 2: necesita papel
        */

        while (true) {
            mostrador.pedir(num_fumador);
            fumar(num_fumador);
        }
    }

// ────────────────────────────────────────────────────────────────────────────────


int main() {
    thread estanquero (funcion_hebra_estanquero);
    thread fumadores[num_fumadores];

    for (int i = 0; i < num_fumadores; i++)
        fumadores[i] = thread(funcion_hebra_fumador, i);

    for (int i = 0; i < num_fumadores; i++)
        fumadores[i].join();

    return 0;
}