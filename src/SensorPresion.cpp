#include "SensorPresion.h"
#include <iostream>

SensorPresion::~SensorPresion() {
    std::cout << "  [Destructor Sensor " << nombre << "] Liberando Lista Interna (Presion)...\n";
}

void SensorPresion::agregarLectura(double valor) {
    int v = static_cast<int>(valor);
    std::cout << "[Log] Insertando Nodo<int> en " << nombre << ". Valor=" << v << "\n";
    historial.insertarFinal(v);
}

void SensorPresion::procesarLectura() {
    std::cout << "-> Procesando Sensor " << nombre << " (Presion) ...\n";
    double prom = historial.promedio();
    if (historial.size() > 0) {
        std::cout << "[Sensor Presion] Promedio de lecturas: " << prom << ".\n";
    } else {
        std::cout << "[Sensor Presion] Sin lecturas para procesar.\n";
    }
}

void SensorPresion::imprimirInfo() const {
    std::cout << "[" << nombre << "] Tipo: Presion (int)" << "\n";
}
