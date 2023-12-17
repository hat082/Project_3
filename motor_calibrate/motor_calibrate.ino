#include <Wire.h>
#define I2C_ADDRESS (42)
#define ENCODER_DATA_SIZE (4)
#define NUM_OF_MOTORS (4)


long unsigned int encoderValues[4];
long unsigned int e[4];
int motors[4] = {50, 50, 50, 50};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  readEncoder();
  for (int i = 0; i < 4; i++)
  {
    e[i] = encoderValues[i];
  }
  moveForward();
  delay(1000);
  stop();
  int delta[4];
  readEncoder();
  for (int i = 0; i < 4; i++)
  {
    delta[i] = encoderValues[i] - e[i];
    float k = (float)(delta[0] / motors[0]);
    motors[i] = delta[i] / k;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  moveForward();
  Serial.println(encoderValues[1], HEX);
}

void moveForward() {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write("baffff");
  for (int i = 0; i < 4; i++) {
  Wire.write((int)motors[i]);  
  Wire.write(0);
  }
  Wire.endTransmission();
}

void turn() {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write("baffrr");
  for (int i = 0; i < 4; i++) {
  Wire.write((int)motors[i]);  
  Wire.write(0);
  }
  Wire.endTransmission();
}

void stop() {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write("ha");
  Wire.endTransmission();
}

void readEncoder() {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write("i6");
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADDRESS, NUM_OF_MOTORS * ENCODER_DATA_SIZE);

  while (Wire.available() < NUM_OF_MOTORS * ENCODER_DATA_SIZE) {
    // Wait for data to be available
  }

  // Read data and update encoder value
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < ENCODER_DATA_SIZE; j++) {
      encoderValues[i] |= ((long unsigned int)Wire.read() << (8 * j));
    }
  }
}

