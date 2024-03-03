#include <Wire.h>

#define BUFFER_SIZE 1000
#define SAMPLING_TIME_MS 1000
#define ACCEL_SENSITIVITY_2G 32768.0
#define MPU6050_ADDR 0x68

// Detector de Pistas utilizando Arduino Uno e o MPU6050.

byte road = 0;
bool ini = true;
uint16_t global_index = 0;

float x_buffer[BUFFER_SIZE] = {};
float y_buffer[BUFFER_SIZE] = {};
float z_buffer[BUFFER_SIZE] = {};

float x_new_sample = 0.0;
float y_new_sample = 0.0;
float z_new_sample = 0.0;

float x_old_sample = 0.0;
float y_old_sample = 0.0;
float z_old_sample = 0.0;

float x_variance = 0.0;
float y_variance = 0.0;
float z_variance = 0.0;

float x_old_mean = 0.0;
float y_old_mean = 0.0;
float z_old_mean = 0.0;

float x_mean_inc = 0.0;
float y_mean_inc = 0.0;
float z_mean_inc = 0.0;

float x_mean = 0.0;
float y_mean = 0.0;
float z_mean = 0.0;


float update_mean(float mean, float old_sample, float new_sample){
  return (mean*BUFFER_SIZE - old_sample + new_sample) / BUFFER_SIZE;
}

float update_variance(float variance, float old_mean, float mean, float old_sample, float new_sample){
  return variance + pow(mean - old_mean, 2) + (pow(new_sample - mean, 2) - pow(old_sample - mean, 2))/BUFFER_SIZE;
}

float update_slope_mean(float mean, float *buffer, int index, float new_point) {
    // Subtrai o valor antigo
    mean -= abs(buffer[index % BUFFER_SIZE]);

    // Adiciona o novo valor
    buffer[index % BUFFER_SIZE] = new_point;

    // Atualiza a média das inclinações
    mean += abs(new_point - buffer[(index - 1) % BUFFER_SIZE]);

    return mean / (BUFFER_SIZE - 1);
}

float convert_acceleration(int16_t raw_data){
  return (float) (raw_data / ACCEL_SENSITIVITY_2G) * 19.6133;
}

void debug(){
  Serial.print("x: ");
  Serial.print(x_new_sample, 3);
  Serial.print(", y: ");
  Serial.print(y_new_sample, 3);
  Serial.print(", z: ");
  Serial.println(z_new_sample, 3);
}

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  Wire.beginTransmission(MPU6050_ADDR); // Inicializa o MPU6050
  Wire.write(0x6B);                     // Registrador de PWR_MGMT_1
  Wire.write(0);                        // Configura para ativar o MPU6050 (remover suspensão)
  Wire.endTransmission(true);
}

void loop() {

  static unsigned long int time_ini = 0;
  static unsigned long int time_run = 0;

  if(millis() - time_run >= SAMPLING_TIME_MS){

    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x3B); // Registrador de dados do acelerômetro (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom((uint16_t) MPU6050_ADDR, (uint8_t) 6, (bool) true);

    x_new_sample = convert_acceleration(Wire.read() << 8 | Wire.read());
    y_new_sample = convert_acceleration(Wire.read() << 8 | Wire.read());
    z_new_sample = convert_acceleration(Wire.read() << 8 | Wire.read());

    //Save the samples that will be replaced
    x_old_sample = x_buffer[global_index];
    y_old_sample = y_buffer[global_index];
    z_old_sample = z_buffer[global_index];

    //Save the means that will be replaced
    x_old_mean = x_mean;
    y_old_mean = y_mean;
    z_old_mean = z_mean;

    //Calculation of new means
    x_mean = update_mean(x_mean, x_old_sample, x_new_sample);
    y_mean = update_mean(y_mean, y_old_sample, y_new_sample);
    z_mean = update_mean(z_mean, z_old_sample, z_new_sample);

    //Calculation of new variances 
    x_variance = update_variance(x_variance, x_old_mean, x_mean, x_old_sample, x_new_sample);
    y_variance = update_variance(y_variance, y_old_mean, y_mean, y_old_sample, y_new_sample);
    z_variance = update_variance(z_variance, z_old_mean, z_mean, z_old_sample, z_new_sample);

    //Buffer update with the new sample
    x_buffer[global_index] = x_new_sample;
    y_buffer[global_index] = y_new_sample;
    z_buffer[global_index] = z_new_sample;
    
    //Increment of the index
    global_index++;

    //Reset of the index
    if(global_index >= BUFFER_SIZE) global_index = 0;

    time_run = millis();

  }

  //Wait for buffer filling once for decision making
  if(global_index && ini){
    road = 0;
  }else{
    if(ini) ini = false;

    if(x_variance < 1 && y_variance < 1 && z_variance < 1 &&
       x_mean_inc < 2 && y_mean_inc < 2 && z_mean_inc < 2){

      road = 1;

    }
    }else
    if(x_variance > 1.5 && y_variance > 1.5 && z_variance > 1.5 &&
       x_mean_inc > 1.5 && y_mean_inc > 1.5 && z_mean_inc > 1.5){

      road = 2;

    }else
    if(x_variance > 0.5 && y_variance > 0.5 && z_variance > 0.5 &&
       x_variance < 4 && y_variance < 4 && z_variance < 4 &&
       x_mean_inc > 1.5 && y_mean_inc > 1.5 && z_mean_inc > 1.5 &&
       x_mean_inc < 3 && y_mean_inc < 3 && z_mean_inc < 3){

      road = 3;

    }else{
      road = 0;
    }

  }

  switch (road):
  {
  case 0:
    break;
  case 1:
    break;
  case 2:
    break;
  case 3:
    break;
  default:
    road = 0;
    break;
  }

}
