#include <iostream>
#include "SensorTemperatura.h"
#include "SensorPresion.h"
#include "ListaGeneral.h"

int main() {
    std::cout << "--- Sistema IoT de Monitoreo Polimórfico (Mitad 1) ---\n\n";

    ListaGeneral gestion; // lista polimórfica que guarda SensorBase*

    // Crear sensores
    SensorBase* t1 = new SensorTemperatura("T-001");
    SensorBase* p1 = new SensorPresion("P-105");

    // Insertarlos en la lista de gestión
    gestion.insertarFinal(t1);
    std::cout << "Sensor 'T-001' creado e insertado en la lista de gestión.\n";
    gestion.insertarFinal(p1);
    std::cout << "Sensor 'P-105' creado e insertado en la lista de gestión.\n\n";

    // Registrar lecturas
    std::cout << "Opción 3: Registrar Lectura\n";
    t1->agregarLectura(45.3);
    t1->agregarLectura(42.1);
    p1->agregarLectura(80);
    p1->agregarLectura(85);

    // Mostrar info básica
    std::cout << "\nSensores registrados:\n";
    gestion.imprimirTodos();

    // Procesamiento polimórfico
    std::cout << "\nOpción 4: Ejecutar Procesamiento Polimórfico\n";
    gestion.procesarTodos();

    std::cout << "\nOpción 5: Cerrar Sistema (Liberar Memoria)\n";
    // Al terminar main, ~ListaGeneral libera sensores y nodos
    std::cout << "Sistema cerrado.\n";
    return 0;
}
