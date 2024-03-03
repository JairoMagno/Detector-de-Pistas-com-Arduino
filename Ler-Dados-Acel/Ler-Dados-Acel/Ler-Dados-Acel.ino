#include <Wire.h>

const int MPU6050_ADDR = 0x68; // Endereço I2C do MPU6050

void setup() {
  Wire.begin();        // Inicializa a comunicação I2C
  Serial.begin(9600); // Inicializa a comunicação serial
  while (!Serial);

  // Inicializa o MPU6050
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1C);   // Registrador de configuração do acelerômetro (ACCEL_CONFIG)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 1, true); // Solicita 1 byte de dados

  // Lê o valor do registrador de configuração do acelerômetro
  byte accelConfig = Wire.read();

  // Mapeia o valor do registrador para a escala completa
  float fullScaleRange;
  switch (accelConfig & 0x18) {
    case 0x00:
      fullScaleRange = 2.0; // ±2g
      break;
    case 0x08:
      fullScaleRange = 4.0; // ±4g
      break;
    case 0x10:
      fullScaleRange = 8.0; // ±8g
      break;
    case 0x18:
      fullScaleRange = 16.0; // ±16g
      break;
  }

  // Exibe a escala completa no monitor serial
  Serial.print("Escala Completa do Acelerômetro (g): ");
  Serial.println(fullScaleRange);
}

void loop() {
  // Seu código principal aqui
}