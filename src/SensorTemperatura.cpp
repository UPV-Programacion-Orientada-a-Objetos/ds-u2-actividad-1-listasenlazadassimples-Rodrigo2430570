#include "SensorTemperatura.h"
#include <iostream>

SensorTemperatura::~SensorTemperatura() {
    // historial se destruye automáticamente liberando nodos
    std::cout << "  [Destructor Sensor " << nombre << "] Liberando Lista Interna (Temperatura)...\n";
}

void SensorTemperatura::agregarLectura(double valor) {
    float v = static_cast<float>(valor);
    std::cout << "[Log] Insertando Nodo<float> en " << nombre << ". Valor=" << v << "\n";
    historial.insertarFinal(v);
}

void SensorTemperatura::procesarLectura() {
    std::cout << "-> Procesando Sensor " << nombre << " (Temperatura) ...\n";
    float eliminado = 0.0f;
    if (historial.eliminarMinimo(eliminado)) {
        double prom = historial.promedio();
        std::cout << "[Sensor Temp] Lectura más baja (" << eliminado << ") eliminada. Promedio restante: " << prom << ".\n";
    } else {
        std::cout << "[Sensor Temp] Sin lecturas para procesar.\n";
    }
}

void SensorTemperatura::imprimirInfo() const {
    std::cout << "[" << nombre << "] Tipo: Temperatura (float)" << "\n";
}
