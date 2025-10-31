#include <iostream>
#include <cstdio>
#include <cstring>
#include "SensorTemperatura.h"
#include "SensorPresion.h"
#include "ListaGeneral.h"
#include "SerialWin.h"

static void leerLinea(const char* prompt, char* buf, std::size_t max) {
    std::cout << prompt;
    std::cout.flush();
    if (!std::cin.getline(buf, static_cast<std::streamsize>(max))) {
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        buf[0] = '\0';
    }
}

static int leerEntero(const char* prompt) {
    char tmp[64];
    leerLinea(prompt, tmp, sizeof(tmp));
    return std::atoi(tmp);
}

static double leerDouble(const char* prompt) {
    char tmp[64];
    leerLinea(prompt, tmp, sizeof(tmp));
    return std::atof(tmp);
}

static void crearSensorTemp(ListaGeneral& gestion) {
    char id[50];
    leerLinea("Ingrese ID de sensor Temperatura: ", id, sizeof(id));
    if (id[0] == '\0') return;
    if (gestion.buscarPorId(id)) {
        std::cout << "[Aviso] Ya existe un sensor con ID '" << id << "'.\n";
        return;
    }
    SensorBase* s = new SensorTemperatura(id);
    gestion.insertarFinal(s);
    std::cout << "Sensor '" << id << "' creado e insertado en la lista de gestión.\n";
}

static void crearSensorPres(ListaGeneral& gestion) {
    char id[50];
    leerLinea("Ingrese ID de sensor Presion: ", id, sizeof(id));
    if (id[0] == '\0') return;
    if (gestion.buscarPorId(id)) {
        std::cout << "[Aviso] Ya existe un sensor con ID '" << id << "'.\n";
        return;
    }
    SensorBase* s = new SensorPresion(id);
    gestion.insertarFinal(s);
    std::cout << "Sensor '" << id << "' creado e insertado en la lista de gestión.\n";
}

static void registrarLectura(ListaGeneral& gestion) {
    char id[50];
    leerLinea("ID del sensor: ", id, sizeof(id));
    if (id[0] == '\0') return;
    double val = leerDouble("Valor de la lectura: ");
    SensorBase* s = gestion.buscarPorId(id);
    if (!s) {
        std::cout << "[Error] No existe sensor con ID '" << id << "'.\n";
        return;
    }
    s->agregarLectura(val);
}

// Formato de trama esperado: ID=T-001;TYPE=T;VAL=45.3\n
static bool parsearTrama(const char* linea, char* idOut, char* tipoOut, double& valOut) {
    // Inicializar outputs
    idOut[0] = '\0';
    tipoOut[0] = '\0';
    valOut = 0.0;
    if (!linea) return false;
    const char* pID = std::strstr(linea, "ID=");
    const char* pTYPE = std::strstr(linea, "TYPE=");
    const char* pVAL = std::strstr(linea, "VAL=");
    if (!pID || !pTYPE || !pVAL) return false;

    // Extraer ID hasta ';'
    pID += 3;
    int i = 0;
    while (*pID && *pID != ';' && i < 49) { idOut[i++] = *pID++; }
    idOut[i] = '\0';

    // Extraer TYPE (un caracter)
    pTYPE += 5;
    if (*pTYPE) { tipoOut[0] = *pTYPE; tipoOut[1] = '\0'; }

    // Extraer VAL hasta fin o ';'
    pVAL += 4;
    char num[64];
    int j = 0;
    while (*pVAL && *pVAL != ';' && *pVAL != '\n' && j < 63) { num[j++] = *pVAL++; }
    num[j] = '\0';
    valOut = std::atof(num);
    return idOut[0] != '\0' && tipoOut[0] != '\0';
}

static void leerDesdeSerial(ListaGeneral& gestion) {
    char opt[8];
    leerLinea("Seleccione fuente: 1) COM (Arduino)  2) Manual: ", opt, sizeof(opt));
    if (opt[0] == '2') {
        std::cout << "Ingrese lineas con formato ID=XXX;TYPE=T|P;VAL=nnn (linea vacia para terminar)\n";
        while (true) {
            char linea[256];
            leerLinea("> ", linea, sizeof(linea));
            if (linea[0] == '\0') break;
            char id[50], tipo[4]; double valor = 0.0;
            if (!parsearTrama(linea, id, tipo, valor)) {
                std::cout << "[Serial] Trama invalida.\n";
                continue;
            }
            SensorBase* s = gestion.buscarPorId(id);
            if (!s) {
                s = (tipo[0] == 'T') ? static_cast<SensorBase*>(new SensorTemperatura(id))
                                     : static_cast<SensorBase*>(new SensorPresion(id));
                gestion.insertarFinal(s);
                std::cout << "[Serial] Sensor '" << id << "' creado e insertado.\n";
            }
            s->agregarLectura(valor);
        }
        return;
    }

    // Opcion COM
    char port[32];
    leerLinea("Puerto COM (ej. COM3): ", port, sizeof(port));
    if (port[0] == '\0') std::strcpy(port, "COM3");
    SerialWin ser;
    if (!ser.open(port, 9600)) {
        std::cout << "[Serial] No se pudo abrir el puerto.\n";
        return;
    }
    std::cout << "Leyendo 5 lineas del puerto (formato ID=..;TYPE=..;VAL=..)...\n";
    for (int k = 0; k < 5; ++k) {
        char linea[256];
        if (!ser.readLine(linea, sizeof(linea), 3000)) {
            std::cout << "[Serial] Tiempo de espera agotado.\n";
            break;
        }
        char id[50], tipo[4]; double valor = 0.0;
        if (!parsearTrama(linea, id, tipo, valor)) {
            std::cout << "[Serial] Trama invalida: " << linea;
            continue;
        }
        SensorBase* s = gestion.buscarPorId(id);
        if (!s) {
            s = (tipo[0] == 'T') ? static_cast<SensorBase*>(new SensorTemperatura(id))
                                 : static_cast<SensorBase*>(new SensorPresion(id));
            gestion.insertarFinal(s);
            std::cout << "[Serial] Sensor '" << id << "' creado e insertado.\n";
        }
        s->agregarLectura(valor);
    }
}

int main() {
    std::cout << "--- Sistema IoT de Monitoreo Polimórfico ---\n";
    ListaGeneral gestion;

    while (true) {
        std::cout << "\nMenu:\n";
        std::cout << "1) Crear Sensor Temperatura (FLOAT)\n";
        std::cout << "2) Crear Sensor Presion (INT)\n";
        std::cout << "3) Registrar Lectura\n";
        std::cout << "4) Ejecutar Procesamiento Polimórfico\n";
    std::cout << "5) Cerrar Sistema\n";
    std::cout << "6) Leer desde Serial (Arduino)\n";
    std::cout << "7) Listar Sensores\n";

        int op = leerEntero("Seleccione opcion: ");
        switch (op) {
            case 1: crearSensorTemp(gestion); break;
            case 2: crearSensorPres(gestion); break;
            case 3: registrarLectura(gestion); break;
            case 4: std::cout << "\nOpcion 4: Ejecutar Procesamiento Polimorfico\n"; gestion.procesarTodos(); break;
            case 5:
                std::cout << "\nOpcion 5: Cerrar Sistema (Liberar Memoria)\n";
                std::cout << "Sistema cerrado.\n";
                return 0;
            case 6: leerDesdeSerial(gestion); break;
            case 7: gestion.imprimirTodos(); break;
            default: std::cout << "Opcion invalida.\n"; break;
        }
    }
}
