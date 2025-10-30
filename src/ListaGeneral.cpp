#include "ListaGeneral.h"

ListaGeneral::~ListaGeneral() {
    std::cout << "\n--- Liberación de Memoria en Cascada ---\n";
    NodoGeneral* cur = cabeza;
    while (cur) {
        std::cout << "[Destructor General] Liberando Nodo: " << (cur->sensor ? cur->sensor->getNombre() : "<null>") << ".\n";
        // liberar sensor concreto primero (polimórfico)
        delete cur->sensor;
        NodoGeneral* tmp = cur;
        cur = cur->siguiente;
        delete tmp;
    }
    cabeza = cola = nullptr;
}

void ListaGeneral::insertarFinal(SensorBase* s) {
    NodoGeneral* n = new NodoGeneral(s);
    if (!cabeza) {
        cabeza = cola = n;
    } else {
        cola->siguiente = n;
        cola = n;
    }
    tam++;
}

void ListaGeneral::procesarTodos() {
    std::cout << "\n--- Ejecutando Polimorfismo ---\n";
    NodoGeneral* cur = cabeza;
    while (cur) {
        if (cur->sensor) {
            cur->sensor->procesarLectura();
        }
        cur = cur->siguiente;
    }
}

void ListaGeneral::imprimirTodos() const {
    NodoGeneral* cur = cabeza;
    while (cur) {
        if (cur->sensor) cur->sensor->imprimirInfo();
        cur = cur->siguiente;
    }
}
