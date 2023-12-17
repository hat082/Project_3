#include <Wire.h>
#include <SPI.h>
#include "RF24.h"

RF24 rf24(9, 10);  // CE, CSN

const byte addr[] = "1Node";
const byte addr2[] = "2Node";
const byte pipe = 1;

struct DataPacket {
  int xVal;
  int yVal;
  int buttonVal;
  char command[8];
} receivedData;

struct displayPakDef
{
  int deltaDistance;
  int inclination;
  int angle;
} displayPak;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rf24.begin();
  rf24.setChannel(101);
  rf24.setPALevel(RF24_PA_MAX);
  rf24.setDataRate(RF24_2MBPS);
  rf24.openReadingPipe(pipe, addr);
  rf24.openWritingPipe(addr2);
  rf24.startListening();
  Serial.println("nRF24L01 ready!");
  delay(1000);
}

void loop() {
  rf24.startListening();
  delay(5);
  if (rf24.available(&pipe)) {
    
    rf24.read(&receivedData, sizeof(receivedData));
    
    
    // 串口显示接收到的值
    Serial.print("Received X = ");
    Serial.println(receivedData.xVal);
    Serial.print("Received Y = ");
    Serial.println(receivedData.yVal);
    Serial.print("Button state: ");
    Serial.println(receivedData.buttonVal == HIGH ? "Not Pressed" : "Pressed");
    Serial.print("Received command: ");
    Serial.println(receivedData.command);

    // rf24.stopListening();
    // delay(5);
    // displayPak.angle += 1; 
    // rf24.write(&displayPak, sizeof(displayPak));


    executeCmd();
  }
}

void executeCmd() {
  // 发送指令到小车
  Wire.beginTransmission(42);
  Wire.write(receivedData.command);
  if (strcmp(receivedData.command, "baffff") == 0 || strcmp(receivedData.command, "barrrr") == 0) {
    Wire.write(40);
    Wire.write(0);
    Wire.write(40);
    Wire.write(0);
    Wire.write(43);
    Wire.write(0);
    Wire.write(43);
    Wire.write(0);
  } else if (strcmp(receivedData.command, "barrff") == 0 || strcmp(receivedData.command, "baffrr") == 0) {
    Wire.write(40);
    Wire.write(0);
    Wire.write(40);
    Wire.write(0);
    Wire.write(40);
    Wire.write(0);
    Wire.write(40);
    Wire.write(0);
  }
  Wire.endTransmission();
}
