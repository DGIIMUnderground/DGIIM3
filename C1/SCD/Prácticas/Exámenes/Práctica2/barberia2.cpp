// Paula Villanueva Nuñez
#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "Semaphore.h"
#include "HoareMonitor.h"

using namespace std;
using namespace HM;

mutex mtx;
const int n = 5; // Numero de clientes

template<int min, int max> int aleatorio (){
  static default_random_engine generador((random_device())());
  static uniform_int_distribution<int> distribucion_uniforme(min, max);
  return distribucion_uniforme(generador);
}

void esperarFueraBarberia (int num_cliente){
    // calcular milisegundos aleatorios de duración de la acción de esperar fuera)
    chrono::milliseconds duracion_esperar_fuera(aleatorio<20,200>());

    // informa de que comienza a esperar fuera
    mtx.lock();
    cout << "\t\tCliente " << num_cliente << "  :" << " empieza a esperar fuera de la barbería (" << duracion_esperar_fuera.count() << " milisegundos)" << endl;
    mtx.unlock();

    // espera bloqueada un tiempo igual a 'duracion_esperar_fuera' milisegundos
    this_thread::sleep_for(duracion_esperar_fuera);

    // informa de que ha terminado de esperar fuera
    mtx.lock();
    cout << "\t\tCliente " << num_cliente << "  : termina de esperar fuera." << endl;
    mtx.unlock();
}

void cortarPeloACliente (){
    // calcular milisegundos aleatorios de duración de la acción de cortar)
    chrono::milliseconds duracion_cortar(aleatorio<20,200>());

    // informa de que comienza a cortar
    mtx.lock();
    cout << "\t\tCortando pelo a la pareja de clientes durante " << duracion_cortar.count() << " milisegundos" << endl;
    mtx.unlock();

    // espera bloqueada un tiempo igual a 'duracion_cortar' milisegundos
    this_thread::sleep_for(duracion_cortar);

    // informa de que ha terminado de cortar
    mtx.lock();
    cout << "\t\tHe terminado de cortar el pelo, comienza espera de nueva pareja de clientes" << endl;
    mtx.unlock();
}

class Barberia : public HoareMonitor{
private:
    CondVar sala_espera, silla, barbero;

public:
    Barberia ();
    void cortarPelo (int num_cliente);
    void siguienteCliente ();
    void finCliente ();
};

Barberia::Barberia (){
    sala_espera = newCondVar ();
    silla = newCondVar ();
    barbero = newCondVar ();
}

void Barberia::cortarPelo (int num_cliente){
    if (sala_espera.get_nwt() == 0){ // No hay nadie en la sala de espera
        mtx.lock();
        cout << "\tEl cliente " << num_cliente << " encuentra la sala de espera vacia\n";
        mtx.unlock();
    }
    else if (sala_espera.get_nwt() == 1 && !barbero.empty()){ // Si hay una persona esperando y el barbero esta durmiendo
        mtx.lock();
        cout << "\tEl cliente " << num_cliente << " encuentra a una persona esperando y al barbero durmiendo\n";
        cout << "\tEl cliente " << num_cliente << " despierta al barbero\n";
        mtx.unlock();

        barbero.signal(); // Despierta al barbero
    }
    
    mtx.lock();
    cout << "\tEl cliente " << num_cliente << " se sienta en la sala de espera\n";
    mtx.unlock();

    sala_espera.wait(); // Espera a que lo llamen

    mtx.lock();
    cout << "\tEl cliente " << num_cliente << " ha acabado de esperar\n";
    mtx.unlock();

    mtx.lock();
    cout << "\tEl cliente " << num_cliente << " se ha sentado en la silla\n";
    mtx.unlock();

    silla.wait(); // Se sienta en la silla de pelar

    mtx.lock();
    cout << "\tEl cliente " << num_cliente << " ha terminado de cortarse el pelo\n";
    mtx.unlock();
}

void Barberia::siguienteCliente (){    
    if ((sala_espera.get_nwt() <= 1) && silla.empty()){ // no hay ninguna pareja ni esta pelando
        mtx.lock();
        cout << "No hay ninguna pareja en la barberia, el barbero se duerme\n";
        mtx.unlock();

        barbero.wait(); // El barbero se duerme

        mtx.lock();
        cout << "El barbero se despierta\n";
        mtx.unlock();
    }
    
    if (silla.empty()){ // Siempre va a estar vacia
        mtx.lock();
        cout << "Silla vacia, que entre la siguiente pareja\n";
        mtx.unlock();
        
        sala_espera.signal(); // Llama a un cliente
        sala_espera.signal(); // Llama a otro cliente
    }
}

void Barberia::finCliente (){
    mtx.lock();
    cout << "La pareja de clientes ha terminado. Los clientes se van\n";
    mtx.unlock();

    silla.signal(); // Vacia la silla
    silla.signal(); // Vacia la silla
}

void funcion_hebra_barbero (MRef<Barberia> barberia){
    while (true){
        barberia->siguienteCliente();
        cortarPeloACliente();
        barberia->finCliente();
    }
}

void funcion_hebra_cliente (MRef<Barberia> barberia, int num_cliente){
    while (true){
        barberia->cortarPelo(num_cliente);
        esperarFueraBarberia(num_cliente);
    }
}

int main (){
    cout << "-------------------------------------------------------------------------------" << endl
         << "Problema del barbero durmiente." << endl
         << "-------------------------------------------------------------------------------" << endl
         << flush;

    MRef<Barberia> barberia = Create<Barberia>();
    thread h_barbero, h_clientes[n];

    h_barbero = thread(funcion_hebra_barbero, barberia);

    for (int i = 0; i < n; i++)
        h_clientes[i] = thread(funcion_hebra_cliente, barberia, i);

    for (int i = 0; i < n; i++)
        h_clientes[i].join();

    h_barbero.join();
}
