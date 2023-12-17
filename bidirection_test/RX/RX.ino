#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <MPU6050_light.h>


MPU6050 mpu(Wire);
// define the robot states
typedef enum {
  IDLE,
  CALIBRATE_MOTORS,
  CALIBRATE_MPU,
  EXECUTE,
} robotState;

// for nrf transmission
typedef struct {
  int x;
  int y;
} controlDef;
controlDef controlPak = { 
  0, 
  0 
};

typedef struct
{
  robotState state;
  int deltaDistance;
  int inclination;
  int angle;
} displayDef;
displayDef displayPak = { 
  IDLE, 
  0, 
  0, 
  0 
};

RF24 radio(9, 10); // CE, CSN
const byte addresses[][6] = {"1Node", "2Node"};

uint8_t pipe = 1;

void setup() {
  Serial.begin(115200);

  radio.begin();
  radio.setChannel(101);
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(pipe, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);

  mpu.begin();
  mpu.calcOffsets();
  radio.startListening();
  delay(100);
}

void loop() {

  RControlPak();

  Serial.print("ControlPak: ");
  Serial.println(controlPak.x);
}

void RControlPak() {
  delay(5);

  if (radio.available(&pipe))
  {
    radio.read(&controlPak, sizeof(controlPak));
    // updateDisplayPak();
    // TDisplayPak();
  }
}

void updateDisplayPak() {
  if (displayPak.deltaDistance > 200)
  {
    displayPak.deltaDistance = 0;
  }
  displayPak.deltaDistance += 1;
  displayPak.angle = mpu.getAngleZ();
}

void TDisplayPak() {
  delay(5);
  radio.stopListening();
  radio.write(&displayPak, sizeof(displayPak));
}