// IoTSerialSim.ino - Simulador de sensores (Temperatura y Presion) via Serial
// Envia lineas con el formato esperado por la app C++:
//   ID=XXX;TYPE=T|P;VAL=nnn.nn\n
#include <Arduino.h>

// Configuracion
#define BAUDRATE 9600
#define ID_TEMP   "T-001"
#define ID_PRES   "P-105"

unsigned long lastMs = 0;
bool toggleTemp = true; // alterna entre T y P

void setup() {
  Serial.begin(BAUDRATE);
  // Semilla pseudo-aleatoria robusta
#if defined(A0)
  randomSeed(analogRead(A0));
#else
  randomSeed(micros());
#endif
  delay(500);
  Serial.println("# IoTSerialSim listo. Enviando tramas cada 1s...");
}

void loop() {
  unsigned long now = millis();
  if (now - lastMs >= 1000) {
    lastMs = now;
    if (toggleTemp) {
      // Temperatura entre 20.0 y 50.0 (1 decimal)
      long t10 = random(200, 501); // 200..500 en decimas
      float temp = t10 / 10.0;
      // Construir linea sin printf/snprintf para evitar problemas con floats en AVR
      Serial.print("ID="); Serial.print(ID_TEMP);
      Serial.print(";TYPE=T;VAL="); Serial.println(temp, 1);
    } else {
      // Presion entre 70 y 100 (entero)
      int pres = (int)random(70, 101);
      Serial.print("ID="); Serial.print(ID_PRES);
      Serial.print(";TYPE=P;VAL="); Serial.println(pres);
    }
    toggleTemp = !toggleTemp;
  }

  // Si quieres, puedes leer comandos por Serial aqui para cambiar IDs o rangos
  // (opcional, no necesario para la practica)
}
