#include <Wire.h>

#define SAMPLING_TIME_MS 10
#define ACCEL_SENSITIVITY_2G 32768.0
#define MPU6050_ADDR 0x68

int16_t x_new_sample = 0;
int16_t y_new_sample = 0;
int16_t z_new_sample = 0;

float convert_acceleration(int16_t raw_data){
  return (float) (raw_data / ACCEL_SENSITIVITY_2G) * 19.613;
}

void setup() {
  Serial.begin(38400);
  while (!Serial);
  
  // Inicializa o MPU6050
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B);   // Registrador de PWR_MGMT_1
  Wire.write(0);      // Configura para ativar o MPU6050 (remover suspensão)
  Wire.endTransmission(true);
}

void loop() {

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B); // Registrador de dados do acelerômetro (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 6, true);
  //Wire.requestFrom((uint16_t) MPU6050_ADDR, (size_t) 6, (bool) true);
  //Wire.requestFrom((uint16_t) MPU6050_ADDR, (uint8_t) 6, (bool) true);
  //Wire.requestFrom((uint16_t) MPU6050_ADDR, (uint8_t) 6, (uint8_t) true);
  //Wire.requestFrom((uint8_t) MPU6050_ADDR, (size_t) 6, (bool) true);
  //Wire.requestFrom((uint16_t) MPU6050_ADDR, (uint8_t) 6, (uint8_t) true);
  //Wire.requestFrom((uint8_t) MPU6050_ADDR, (uint8_t) 6, (uint8_t) true);
  //Wire.requestFrom((uint8_t) MPU6050_ADDR, (uint8_t) 6, (uint8_t) true);
  //Wire.requestFrom((int) MPU6050_ADDR, (int) 6, (int) true);
  
  x_new_sample = Wire.read() << 8 | Wire.read();
  y_new_sample = Wire.read() << 8 | Wire.read();
  z_new_sample = Wire.read() << 8 | Wire.read();

  Serial.print(convert_acceleration(x_new_sample), 3);  
  Serial.print(',');
  Serial.print(convert_acceleration(y_new_sample), 3);
  Serial.print(',');
  Serial.println(convert_acceleration(z_new_sample), 3);

  delay(SAMPLING_TIME_MS);
}
