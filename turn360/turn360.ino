/* Get tilt angles on X and Y, and rotation angle on Z
 * Angles are given in degrees
 * 
 * License: MIT
*/

#include "Wire.h"
#include <MPU6050_light.h>
#define MOTOR_I2C 42

MPU6050 mpu(Wire);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
  delay(100);
}

void loop() {
  delay(100);
  mpu.update(); 

  Serial.println(mpu.getAngleZ());
  if(mpu.getAngleZ() >= 360 || mpu.getAngleZ() <= -360)
  {
    stop();
    delay(10);
    while(1);
  }
  else
  {
    turn90(0.8);
  }

}

  
void turn90(float a) {
  Wire.beginTransmission(MOTOR_I2C);
  Wire.write("baffrr");
  Wire.write((int)(55 * a));  //motor 0 50
  Wire.write(0);
  Wire.write((int)(55 * a));  //motor 1 50
  Wire.write(0);
  Wire.write((int)(50 * a));  //motor 2 57
  Wire.write(0);
  Wire.write((int)(50 * a));  //motor 3 60
  Wire.write(0);
  Wire.endTransmission();
}

void stop() {
  Wire.beginTransmission(MOTOR_I2C);
  Wire.write("ha");
  Wire.endTransmission();
}
