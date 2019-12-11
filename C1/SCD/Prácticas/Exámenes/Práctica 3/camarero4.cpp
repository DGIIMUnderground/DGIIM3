/*
    Modifica tu solución del ejercicio de los filósofos con camarero de la práctica 3 para que
    el camarero atienda prioritariamente las peticiciones de sentarse o levantarse de aquellos filósofos que tienen un número alto,
    asumiendo que se numeran del 0 al 4.

    En cada iteración:
    Se comprueba si hay peticiones del 4. Si no hay, pasa al siguiente.
    Se comprueba si hay peticiones del 3. Si no hay, pasa al siguiente.
    Se comprueba si hay peticiones del 2. Si no hay, pasa al siguiente.
    Se comprueba si hay peticiones del 1. Si no hay, pasa al siguiente.
    Se comprueba si hay peticiones del 0. Si no hay, pasa al 4.

*/
#include <mpi.h>
#include <thread>       // this_thread::sleep_for
#include <random>       // dispositivos, generadores y distribuciones aleatorias
#include <chrono>       // duraciones (duration), unidades de tiempo
#include <iostream>

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

const int
    num_filosofos = 5,
    num_procesos  = 2 * num_filosofos + 1,

    id_camarero    = 10,
    tag_sentarse   = 0,
    tag_levantarse = 1;

template< int min, int max > int aleatorio() {
    static default_random_engine generador( (random_device())() );
    static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
    return distribucion_uniforme( generador );
}


void funcion_filosofos(int id) {
    // Recalcular tenedores para los filósofos y excluir al camarero.
    int id_ten_izq = (id+1)              % (num_procesos - 1),
        id_ten_der = (id+num_procesos-2) % (num_procesos - 1);

    int var = 0;

    while (true) {
        // 1. Pedir permiso para sentarse
        cout <<"Filósofo " <<id << " pide sentarse" <<endl;
        MPI_Ssend(&var, 1, MPI_INT, id_camarero, tag_sentarse, MPI_COMM_WORLD);

        // 2. Tomar tenedores
        cout <<"Filósofo " <<id << " solicita ten. izq." <<id_ten_izq <<endl;
        MPI_Ssend(&var, 1, MPI_INT, id_ten_izq, 0, MPI_COMM_WORLD);

        cout <<"Filósofo " <<id <<" solicita ten. der." <<id_ten_der <<endl;
        MPI_Ssend(&var, 1, MPI_INT, id_ten_der, 0, MPI_COMM_WORLD);

        // 3. Comer
        cout <<"Filósofo " <<id <<" comienza a comer" <<endl ;
        sleep_for( milliseconds( aleatorio<10,100>() ) );

        // 4. Soltar tenedores
        cout <<"Filósofo " <<id <<" suelta ten. izq. " <<id_ten_izq <<endl;
        MPI_Ssend(&var, 1, MPI_INT, id_ten_izq, 0, MPI_COMM_WORLD);

        cout<< "Filósofo " <<id <<" suelta ten. der. " <<id_ten_der <<endl;
        MPI_Ssend(&var, 1, MPI_INT, id_ten_der, 0, MPI_COMM_WORLD);

        // 5. Levantarse
        cout <<"Filósofo " <<id << " se levanta" <<endl;
        MPI_Ssend(&var, 1, MPI_INT, id_camarero, tag_levantarse, MPI_COMM_WORLD);

        // 6. Pensar
        cout << "Filosofo " << id << " comienza a pensar" << endl;
        sleep_for( milliseconds( aleatorio<10,100>() ) );
    }
}

void funcion_tenedores( int id ) {
    int valor, id_filosofo;
    MPI_Status estado;

    while ( true )
    {
        MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
        id_filosofo = estado.MPI_SOURCE;

        cout << "\t\t\tTen. " <<id <<" ha sido cogido por filo. " <<id_filosofo <<endl;

        MPI_Recv(&valor, 1, MPI_INT, id_filosofo, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
        cout << "\t\t\tTen. "<< id<< " ha sido liberado por filo. " <<id_filosofo <<endl ;
    }
}

void funcion_camarero () {
    int valor, tag_valida, id_filosofo, flag, filosofos_comiendo = 0;
    bool target_found = false;
    MPI_Status estado;

    while (true) {
        if (filosofos_comiendo < 4)         // entre 0 y 3 comiendo => se pueden levantar y sentar
            tag_valida = MPI_ANY_TAG;
        else                                // 4 comiendo => solo se pueden levantar.
            tag_valida = tag_levantarse;

        target_found = false;
        while (!target_found) {
            for (int i = num_filosofos * 2 - 2;     // Filósofos pares. Usutalmente tienen id 8, 6, 4, 2, 0.
                     i >= 0 && !target_found;
                     i -= 2)
            {
                MPI_Iprobe(i, tag_valida, MPI_COMM_WORLD, &flag, &estado);

                if (flag > 0) {
                    target_found = true;
                    id_filosofo = i;

                    cout << "\t\t\t\t\tCamarero selecciona " << id_filosofo << " por su prioridad ante las peticiones actuales\n" << flush;
                }
            }
        }

        MPI_Recv(&valor, 1, MPI_INT, id_filosofo, estado.MPI_TAG, MPI_COMM_WORLD, &estado);

        if (estado.MPI_TAG == tag_sentarse)
            filosofos_comiendo++;
        else if (estado.MPI_TAG == tag_levantarse)
            filosofos_comiendo--;
    }
}


int main( int argc, char** argv ) {
    int id_propio, num_procesos_actual;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
    MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );


    if (num_procesos == num_procesos_actual) {
        // ejecutar la función correspondiente a 'id_propio'
        if (id_propio % 2 == 0 && id_propio != id_camarero)
            funcion_filosofos(id_propio);
        else if (id_propio != id_camarero)
            funcion_tenedores(id_propio);
        else
            funcion_camarero();
    }
    else {
        if (id_propio == 0) {
            cout << "el número de procesos esperados es:    " << num_procesos        << endl
                 << "el número de procesos en ejecución es: " << num_procesos_actual << endl
                 << "(programa abortado)" << endl ;
        }
    }

    MPI_Finalize();
    return 0;
}
