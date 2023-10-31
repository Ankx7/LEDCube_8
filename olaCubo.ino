#include <Arduino.h>

// Definición de pines
const int dataPin = 8;   // Pin de datos conectado a los Serial Data (DS) de los multiplexores
const int clockPin = 10;  // Pin de reloj conectado a los Clock (SH_CP) de los multiplexores
const int latchPin = 9;  // Pin de activación conectado a los Latch (ST_CP) de los multiplexores

// Definición de constantes del cubo
const int cubeSize = 8;  // Tamaño del cubo de LEDs (8x8x8)

// Definición del patrón de onda 3D
byte wavePattern[] = {
  B00000000,
  B00000001,
  B00000010,
  B00000100,
  B00001000,
  B00010000,
  B00100000,
  B01000000,
  B10000000,
  B01000000,
  B00100000,
  B00010000,
  B00001000,
  B00000100,
  B00000010,
  B00000001
};
// Variables de estado
int currentLayer = 0;  // Capa actual del cubo

// Función para enviar datos a los multiplexores
void sendData(byte data)
{
  shiftOut(dataPin, clockPin, LSBFIRST, data);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
}

// Función para actualizar el cubo con la capa actual
void updateCube()
{
  // Activar la capa actual
  sendData(B11111111 ^ (1 << currentLayer));
  
  // Enviar datos de columna para cada fila
  for (int row = 0; row < cubeSize; row++) {
    byte columnData = wavePattern[(currentLayer + row) % sizeof(wavePattern)];
    sendData(columnData);
  }
  
  // Desactivar todas las filas
  sendData(B11111111);
}

void setup()
{
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  // Actualizar el cubo con la capa actual
  updateCube();
  
  // Incrementar la capa actual
  currentLayer = (currentLayer + 1) % cubeSize;
  
  delay(100);  // Ajusta la velocidad de la animación según sea necesario
}
