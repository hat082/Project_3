#include <Wire.h>


int motors[4] = {51,51,50,50};

void setup() {
  Wire.begin();
  move("baffff");
  while(analogRead(A0) < 380);
  move("barrff");
  delay(450);
  move("baffff");
  while(analogRead(A0) < 380);
  move("barrff");
  delay(420);
  motors[0] = 51;
  motors[1] = 50;
  motors[2] = 50;
  motors[3] = 50;
  move("baffff");
  delay(2000);
  motors[0] = 40;
  motors[1] = 40;
  motors[2] = 45;
  motors[3] = 45;
  move("baffff");
  delay(2100);
  stopMotors();
  delay(1800);
  motors[0] = 51;
  motors[1] = 50;
  motors[2] = 50;
  motors[3] = 50;
  move("barrff");
  delay(1670);
  motors[0] = 20;
  motors[1] = 21;
  motors[2] = 20;
  motors[3] = 20;
  move("baffff");
  delay(3100);
  motors[0] = 51;
  motors[1] = 50;
  motors[2] = 50;
  motors[3] = 50;
  move("baffff");
  while(analogRead(A0) < 380); // 380
  move("barrff");
  delay(420);
  move("baffff");
  while(analogRead(A0) < 380);
  move("barrff");
  delay(420);
  move("baffff");
  while(analogRead(A0) < 380);
  move("barrff");
  delay(170);
  move("baffff");
  while(analogRead(A6) < 850);
  move("baffrr");
  delay(350);
  move("baffff");
  while(analogRead(A0) < 380);
  move("barrff");
  delay(240);
  move("baffff");
  delay(1000);
  stopMotors();
}

void loop() {


}

void move(char cmd[]) {
  Wire.beginTransmission(42);
  Wire.write(cmd);
  for (int i = 0; i < 4; i++) {
    Wire.write(motors[i]);
    Wire.write(0);
  }
  Wire.endTransmission();
}

void stopMotors() {
  Wire.beginTransmission(42);
  Wire.write("ha");
  Wire.endTransmission();
}