#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <MPU6050_light.h>
#include <LiquidCrystal.h>


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

// controller sends commands to car and recieve feedback from car

RF24 radio(9, 10); // CE, CSN
const byte addresses[][6] = {"1Node", "2Node"};
boolean buttonState = 0;
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
uint8_t pipe = 1;

void setup() {
  Serial.begin(115200);

  lcd.begin(16, 2);

  radio.begin();
  radio.setChannel(101);
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(pipe, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  lcd.print("Hello");
  radio.stopListening();
}

void loop() {
  delay(1000);
  updateControlPak();
  TControlPak();
  // RDisplayPak();
  lcd.setCursor(0, 0);
  // lcd.print("DisplayPak: ");
  // lcd.print(displayPak.deltaDistance);
  // Serial.print("DisplayPak: ");
  // Serial.println(displayPak.deltaDistance);
}

void updateControlPak() {
  controlPak.x = (controlPak.x + 1) % 200;
}

void TControlPak() {
  delay(5);

  if (!radio.write(&controlPak, sizeof(controlPak)))
  {
    Serial.println("Failed");
  }
  else
  {
    Serial.println("Success");
  }
}


void RDisplayPak() {
  delay(5);
  radio.startListening();
  while (!radio.available(&pipe));
  radio.read(&displayPak, sizeof(displayPak));
}