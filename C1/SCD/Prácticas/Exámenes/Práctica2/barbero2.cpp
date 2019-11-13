// @author: Andrés Millán

/*
    Existen dos tipos de clientes: 0 y 1. Existen sillas específicas para cada uno. Se puede asumiar que hay 3 clientes por cada tipo.
    El barbero no puede cortar de forma consecutiva a dos clientes del mismo tipo.
    Cuando termina de pelar a alguien, comprueba que puede pelar al del otro tipo. Si no, se va a dormir hasta que lo despierte el correspondiente.
    Empieza pelando a uno del tipo 0. Si llega uno al que no le toca, se espera.
    Una vez despierten al barbero, debe pelar a su tipo correspondiente.
*/

#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include "HoareMonitor.h"

using namespace std;
using namespace HM;

const int
       num_barberos = 1
     , num_clientes = 6;

//
// ─────────────────────────────────────────────────────────── CLASE BARBERIA ─────
//


    class Barberia : public HoareMonitor {
    private:
        static const int tipos = 2;

        CondVar
              espera_tipo[tipos]
            , cama          = newCondVar()
            , silla         = newCondVar();

        int ultimo_pelado = 1;  // Empieza por defecto pelando a uno de tipo 0.

    public:
        Barberia () {
            for (int i = 0; i < tipos; i++)
                espera_tipo[i] = newCondVar();
        }

        void cortar_pelo (int i, int tipo) {
            cout << "\t\tCliente " << i << " (tipo "<< tipo << ") entra a la barbería\n" << flush;

            while (!silla.empty() || ultimo_pelado == tipo) {
                cout << "\t\tCliente " << i << " (tipo "<< tipo << "): la silla está ocupada, me voy a esperar\n" << flush;

                espera_tipo[tipo].wait();
            }

            if (!cama.empty()) {
                cout << "\t\tCliente " << i << " (tipo "<< tipo << "): Barbero no se duerma por favor\n" << flush;
                cama.signal();
            }

            cout << "\t\tCliente " << i << " (tipo "<< tipo << "): allé voy\n" << flush;
            silla.wait();
        }


        void siguiente_cliente() {
            if (    (ultimo_pelado == 0 && espera_tipo[1].empty() && silla.empty())
                ||  (ultimo_pelado == 1 && espera_tipo[0].empty() && silla.empty()))
            {
                cout << "Barbero: buah, no hay nadie, I sleep\n" << flush;
                cama.wait();
            }

            cout << "Barbero: Que pase el siguiente\n" << flush;

            if (ultimo_pelado == 1) {
                espera_tipo[0].signal();
            }
            else if (ultimo_pelado == 0) {
                espera_tipo[1].signal();
            }
        }


        void fin_cliente() {
            if (ultimo_pelado == 0)
                ultimo_pelado = 1;
            else
                ultimo_pelado = 0;

            cout << "Barbero: he acabado con un cliente tipo "<< ultimo_pelado << "\n\n" << flush;

            silla.signal();
        }
    };

//
// ───────────────────────────────────────────────────────── FUNCIONES ESPERA ─────
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


    void esperar_fuera_barberia (int i) {
        cout << "\t\tCliente " << i << ": me voy a tomar el fresco\n" << flush;
        chrono::milliseconds sleep_time( aleatorio<200,2000>() );
        this_thread::sleep_for( sleep_time );
    };


    void cortar_pelo_a_cliente () {
        cout << "Barbero: ¿Solo las puntas? Le voy a cortar hasta las neuronas\n" << flush;
        chrono::milliseconds sleep_time( aleatorio<20,200>() );
        this_thread::sleep_for( sleep_time );
    };

//
// ────────────────────────────────────────────────────────── THREAD STARTERS ─────
//


    void thread_barbero (MRef<Barberia> barberia) {
        while (true) {
            barberia -> siguiente_cliente();
            cortar_pelo_a_cliente();
            barberia -> fin_cliente();
        }
    }


    void thread_cliente (int i, int tipo, MRef<Barberia> barberia) {
        assert (i < num_clientes);

        while (true) {
            barberia -> cortar_pelo(i, tipo);
            esperar_fuera_barberia(i);
        }
    }

// ────────────────────────────────────────────────────────────────────────────────


int main(int argc, char const *argv[])
{
    MRef<Barberia> barberia = Create<Barberia>();

    thread barberos[num_barberos];
    thread clientes[num_clientes];

    for (unsigned int i = 0; i < num_barberos; i++) {
        barberos[i] = thread(thread_barbero, barberia);
    }

    for (unsigned int i = 0; i < num_clientes; i++) {
        clientes[i] = thread(thread_cliente, i, (i%2),barberia);
    }

    for (unsigned int i = 0; i < num_barberos; i++) {
        barberos[i].join();
    }

    for (unsigned int i = 0; i < num_clientes; i++) {
        clientes[i].join();
    }

    return 0;
}
