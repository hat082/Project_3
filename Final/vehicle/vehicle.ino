// define the robot states
typedef enum {
  turn90,
  turn90b,
  turn45,
  turn45b,
  turn360,
  a,
  b,
  c,
  d,
  e,
  f,
  g,
  h,
  i,
  j
} robotState; 

robotState prevState; 

robotState nextState(robotState currentState) {
  switch (currentState) {
    case a:
      prevState = a;
      return turn90;
    case b:
      prevState = b;
      return turn90;
    case c:
      return d;
    case d:
      return turn360;
    case turn360:
      return e;
    case e:
      return f;
    case f: 
      prevState = f;
      return turn90;
    case g:
      prevState = g;  
      return turn90;
    case h:
      return turn45;
    case turn45:
      return i;
    case i:
      return turn90b;
    case turn90b:
      return j;
    case j:
      return turn45b;
    case turn90:
      switch (prevState) {
        case a:
          return b;
        case b:
          return c;
        case f:
          return g;
        case g:
          return h;
      }
  }
}



// // for nrf transmission
// typedef struct {
//   int x;
//   int y;
// } controlDef;
// controlDef controlPak = { 
//   0, 
//   0 
// };

// typedef struct
// {
//   robotState state;
//   int deltaDistance;
//   int inclination;
//   int angle;
// } displayDef;

#include <Wire.h>
#define I2C_ADDRESS (42)
#define ENCODER_DATA_SIZE (4)
#define NUM_OF_MOTORS (4)

// define the robot structure and its attributes
typedef struct {
  robotState state;
  long unsigned int encoderValues[4];
  bool up;
  bool down;
} robotDef;

robotDef robot = {
  .state = a,
  .encoderValues = {0, 0, 0, 0},
  .up = false,
  .down = false,
};

#include <MPU6050_light.h>

MPU6050 mpu(Wire);

void setup() {
  Wire.begin(); 
  Serial.begin(9600);

  mpu.begin();
  delay(1000);
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Setup finished");
  robot.state = a;
}
int motors[4] = {36,36,35,35};
int initial;
int interval = 200;
void loop() {
  // while (true) {
  //   Serial.println(analogRead(A0));
  // }

  Serial.println(robot.state);
  switch (robot.state) {
    case turn90:
      initial = mpu.getAngleZ();
      move("barrff");
      while (abs(mpu.getAngleZ() - initial) <= 91) { mpu.update(); }
      // delay(690);
      stopMotors();
      delay(interval);
      robot.state = nextState(robot.state);
      break;
    case turn90b:
      initial = mpu.getAngleZ();
      move("baffrr");
      while (abs(mpu.getAngleZ() - initial) <= 89) { mpu.update(); }
      stopMotors();
      delay(interval);
      robot.state = nextState(robot.state);
      break;
    case turn45:
      initial = mpu.getAngleZ();
      move("barrff");
      while (abs(mpu.getAngleZ() - initial) <= 43.8) { mpu.update(); }
      stopMotors();
      delay(interval);
      robot.state = nextState(robot.state);
      break;
    case turn45b:
      initial = mpu.getAngleZ();
      move("baffrr");
      while (abs(mpu.getAngleZ() - initial) <= 43.8) { mpu.update(); }
      stopMotors();
      move("baffrr");
      delay(1000);
      stopMotors();
      delay(interval);
      break;
    case turn360:
      motors[0] = 40;
      motors[1] = 40;
      motors[2] = 40;
      motors[3] = 41;
      initial = mpu.getAngleZ();
      stopMotors();
      delay(2000);
      move("barrff");
      while (abs(mpu.getAngleZ() - initial) < 350) { mpu.update(); }
      stopMotors();
      delay(interval);
      robot.state = nextState(robot.state);
      break;
    case a:
    case b:
    case f:
    case g:
    case h:
    case j:
      move("baffff");
      while(analogRead(A0) < 380);
      robot.state = nextState(robot.state);
      stopMotors();
      delay(interval);
      break;
    case c:
      mpu.update();
      initial = mpu.getAngleY();
      move("baffff");
      while (abs(mpu.getAngleY() - initial) < 20) { mpu.update(); }
      delay(interval);
      robot.state = nextState(robot.state);
    case d:
      motors[0] = 40;
      motors[1] = 40;
      motors[2] = 40;
      motors[3] = 40;
      move("baffff");
      delay(200);
      while (abs(mpu.getAngleY() - initial) > 5) { mpu.update(); }
      delay(200);
      robot.state = nextState(robot.state);
      break;
    case i:
      move("baffff");
      while(analogRead(A6) < 850);
      robot.state = nextState(robot.state);
      break;
    case e:
      motors[0] = 20;
      motors[1] = 20;
      motors[2] = 20;
      motors[3] = 21;
      initial = mpu.getAngleY();
      move("baffff");
      delay(3000);
      robot.state = nextState(robot.state);
      stopMotors();
      delay(1500);
      motors[0] = 40;
      motors[1] = 40;
      motors[2] = 40;
      motors[3] = 41;
      break;
  }
}

void move(char cmd[]) {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(cmd);
  for (int i = 0; i < 4; i++) {
    Wire.write(motors[i]);
    Wire.write(0);
  }
  Wire.endTransmission();
}

void stopMotors() {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write("ha");
  Wire.endTransmission();
}

// void calibrate_motors() {
//   readEncoder();
//   long unsigned int e[4];
//   for (int i = 0; i < 4; i++)
//   {
//     e[i] = robot.encoderValues[i];
//   }
//   moveForward();
//   delay(1000);
//   stopMotors();
//   int delta[4];
//   readEncoder();
//   for (int i = 0; i < 4; i++)
//   {
//     delta[i] = robot.encoderValues[i] - e[i];
//     float k = (float)(delta[0] / motors[0]);
//     motors[i] = delta[i] / k;
//   }
// }

void readEncoder() {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write("i6");
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADDRESS, NUM_OF_MOTORS * ENCODER_DATA_SIZE);

  while (Wire.available() < NUM_OF_MOTORS * ENCODER_DATA_SIZE) {
    // Wait for data to be available
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < ENCODER_DATA_SIZE; j++) {
      robot.encoderValues[i] |= ((long unsigned int)Wire.read() << (8 * j));
    }
  }
}  