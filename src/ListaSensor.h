#ifndef LISTA_SENSOR_H
#define LISTA_SENSOR_H

#include <cstddef>
#include <iostream>

// Nodo de lista enlazada simple
template <typename T>
struct Nodo {
    T dato;
    Nodo<T>* siguiente;
    explicit Nodo(const T& v) : dato(v), siguiente(nullptr) {}
};


template <typename T>
class ListaSensor {
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    std::size_t tam;

public:
    ListaSensor() : cabeza(nullptr), cola(nullptr), tam(0) {}

    // Destructor: libera todos los nodos
    ~ListaSensor() { limpiar(); }

    // Regla de 3 (parcial): deshabilitar copia/assign para esta mitad
    ListaSensor(const ListaSensor&) = delete;
    ListaSensor& operator=(const ListaSensor&) = delete;

    // Inserta al final
    void insertarFinal(const T& valor) {
        Nodo<T>* n = new Nodo<T>(valor);
        if (!cabeza) {
            cabeza = cola = n;
        } else {
            cola->siguiente = n;
            cola = n;
        }
        tam++;
    }

    // Búsqueda lineal (igualdad exacta)
    bool buscar(const T& valor) const {
        Nodo<T>* cur = cabeza;
        while (cur) {
            if (cur->dato == valor) return true;
            cur = cur->siguiente;
        }
        return false;
    }

    // Elimina el nodo con el valor mínimo y devuelve el valor eliminado
    bool eliminarMinimo(T& eliminado) {
        if (!cabeza) return false;
        Nodo<T>* cur = cabeza;
        Nodo<T>* ant = nullptr;

        Nodo<T>* minNode = cabeza;
        Nodo<T>* antMin = nullptr;
        T minVal = cabeza->dato;

        while (cur) {
            if (cur->dato < minVal) {
                minVal = cur->dato;
                minNode = cur;
                antMin = ant;
            }
            ant = cur;
            cur = cur->siguiente;
        }

        // Desenlazar minNode
        if (antMin) {
            antMin->siguiente = minNode->siguiente;
        } else {
            // mínimo estaba en cabeza
            cabeza = minNode->siguiente;
        }
        if (minNode == cola) {
            cola = antMin;
        }

        eliminado = minVal;
        delete minNode;
        tam--;
        return true;
    }

    // Calcula el promedio (como double). Si está vacía, devuelve 0.0
    double promedio() const {
        if (!cabeza) return 0.0;
        long double suma = 0.0L;
        std::size_t count = 0;
        Nodo<T>* cur = cabeza;
        while (cur) {
            suma += static_cast<long double>(cur->dato);
            count++;
            cur = cur->siguiente;
        }
        if (count == 0) return 0.0;
        return static_cast<double>(suma / static_cast<long double>(count));
    }

    // Tamaño actual
    std::size_t size() const { return tam; }

    // Limpia toda la lista
    void limpiar() {
        Nodo<T>* cur = cabeza;
        while (cur) {
            Nodo<T>* tmp = cur;
            cur = cur->siguiente;
            // Log mínimo opcional
            // std::cout << "[Log] Liberando Nodo valor: " << tmp->dato << "\n";
            delete tmp;
        }
        cabeza = cola = nullptr;
        tam = 0;
    }

    // Solo para depuración controlada (no imprimir gran cosa por ahora)
    void imprimirDebug(const char* prefijo = "") const {
        std::cout << prefijo << "[Lista] tam=" << tam << " -> ";
        Nodo<T>* cur = cabeza;
        while (cur) {
            std::cout << cur->dato;
            if (cur->siguiente) std::cout << ", ";
            cur = cur->siguiente;
        }
        std::cout << "\n";
    }
};

#endif // LISTA_SENSOR_H
