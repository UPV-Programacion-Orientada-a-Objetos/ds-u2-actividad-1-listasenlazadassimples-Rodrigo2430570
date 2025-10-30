#ifndef SENSOR_TEMPERATURA_H
#define SENSOR_TEMPERATURA_H

#include "SensorBase.h"
#include "ListaSensor.h"

class SensorTemperatura : public SensorBase {
private:
    ListaSensor<float> historial;

public:
    explicit SensorTemperatura(const char* id) : SensorBase(id) {}
    ~SensorTemperatura();

    void agregarLectura(double valor) override;
    void procesarLectura() override;
    void imprimirInfo() const override;
};

#endif // SENSOR_TEMPERATURA_H
