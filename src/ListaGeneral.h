#ifndef LISTA_GENERAL_H
#define LISTA_GENERAL_H

#include "SensorBase.h"
#include <cstddef>
#include <iostream>

struct NodoGeneral {
    SensorBase* sensor;
    NodoGeneral* siguiente;
    explicit NodoGeneral(SensorBase* s) : sensor(s), siguiente(nullptr) {}
};

// Lista enlazada simple NO genérica que almacena punteros a SensorBase
class ListaGeneral {
private:
    NodoGeneral* cabeza;
    NodoGeneral* cola;
    std::size_t tam;

public:
    ListaGeneral() : cabeza(nullptr), cola(nullptr), tam(0) {}

    ~ListaGeneral();

    void insertarFinal(SensorBase* s);

    std::size_t size() const { return tam; }

    // Recorre y llama polimórficamente a procesarLectura()
    void procesarTodos();

    // Recorre y muestra info
    void imprimirTodos() const;
};

#endif // LISTA_GENERAL_H
