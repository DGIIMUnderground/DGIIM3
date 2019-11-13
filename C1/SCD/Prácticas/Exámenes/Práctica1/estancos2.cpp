// @author: Andrés Millán Muñoz

#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include "Semaphore.h"

using namespace std;
using namespace SEM;

const int num_fumadores   = 3;
const int num_estanqueros = 2;


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


//
// ─────────────────────────────────────────────────── ESTRUCTURAS AUXILIARES ─────
//

    class Recursos {
    private:
        static const int num_suministros = 5;

        Semaphore necesita_gestion = 1;

        Semaphore estanqueros[2]                 = {0, 0};
        Semaphore vector_recursos[num_fumadores] = {0, 0, 0};

    public:

        auto pedir (int tipo) -> void {
            vector_recursos[tipo].sem_wait();

            //estanquero.sem_signal(); // Deshabilitado para este problema en específico
        };

        auto proveer (int tipo, int indice_estanquero) -> void {
            static int necesita_provisiones = 0;  // Por cómo funciona el gestor, esto en principio se hace en exclusión mutua

            estanqueros[indice_estanquero].sem_wait();
            cout << "Estanquero número " << indice_estanquero << " repone el mostrador " << tipo << "\n" << flush;
            vector_recursos[tipo].sem_signal();

            necesita_provisiones = (necesita_provisiones + 1) % num_suministros;
            if (necesita_provisiones == 0)
                necesita_gestion.sem_signal();
        };

        auto gestionar_estanqueros() -> void {
            necesita_gestion.sem_wait();

            int indice_estanquero = aleatorio<0, num_estanqueros - 1>();
            for (int i = 0; i < num_suministros; i++)
                estanqueros[indice_estanquero].sem_signal();

            cout  << "\nEl gestor proporciona " << num_suministros << " al estanquero número " << indice_estanquero << "\n" << flush;
        }
    };


//
// ─────────────────────────────────────────────── INICIALIZADORES DE THREADS ─────
//

    Recursos mostrador;

    // función que ejecuta la hebra del estanquero
    void funcion_hebra_estanquero (int i) {
        while (true) {
            mostrador.proveer(aleatorio<0, 2>(), i);
            this_thread::sleep_for(chrono::milliseconds( aleatorio<20, 30>() ));
        }
    }


    auto funcion_hebra_gestora() -> void {
        while (true) {
            mostrador.gestionar_estanqueros();
            this_thread::sleep_for(chrono::milliseconds( aleatorio<20, 200>() ));
        }
    }

    void fumar (int num_fumador) {
        chrono::milliseconds duracion_fumar( aleatorio<20,200>() );

        cout << "Fumador " << num_fumador << "  :"
             << " empieza a fumar (" << duracion_fumar.count() << " milisegundos)" << endl << flush;

        this_thread::sleep_for( duracion_fumar );

        cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl << flush;
    }


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
    thread gestor (funcion_hebra_gestora);

    thread fumadores[num_fumadores];
    for (int i = 0; i < num_fumadores; i++)
        fumadores[i] = thread(funcion_hebra_fumador, i);

    thread estanqueros[num_estanqueros];
    for (int i = 0; i < 2; i++)
        estanqueros[i] = thread(funcion_hebra_estanquero, i);

    for (int i = 0; i < num_fumadores; i++)
        fumadores[i].join();

    for (int i = 0; i < num_estanqueros; i++)
        estanqueros[i].join();

    gestor.join();

    return 0;
}