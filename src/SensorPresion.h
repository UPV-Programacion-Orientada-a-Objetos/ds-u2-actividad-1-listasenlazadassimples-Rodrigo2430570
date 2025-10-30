#ifndef SENSOR_PRESION_H
#define SENSOR_PRESION_H

#include "SensorBase.h"
#include "ListaSensor.h"

class SensorPresion : public SensorBase {
private:
    ListaSensor<int> historial;

public:
    explicit SensorPresion(const char* id) : SensorBase(id) {}
    ~SensorPresion();

    void agregarLectura(double valor) override;
    void procesarLectura() override;
    void imprimirInfo() const override;
};

#endif // SENSOR_PRESION_H
