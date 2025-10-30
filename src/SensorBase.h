#ifndef SENSOR_BASE_H
#define SENSOR_BASE_H

#include <iostream>
#include <cstring>

class SensorBase {
protected:
    char nombre[50]; // Identificador del sensor

public:
    explicit SensorBase(const char* id) {
        if (id) {
            std::strncpy(nombre, id, sizeof(nombre) - 1);
            nombre[sizeof(nombre) - 1] = '\0';
        } else {
            nombre[0] = '\0';
        }
    }

    virtual ~SensorBase() = default;

    // Métodos virtuales puros obligatorios
    virtual void procesarLectura() = 0;
    virtual void imprimirInfo() const = 0;

    
    virtual void agregarLectura(double valor) = 0;

    const char* getNombre() const { return nombre; }
};

#endif // SENSOR_BASE_H
