#include <SPI.h>
#include "RF24.h"
#include <Wire.h>

RF24 rf24(9,10); // CE, CSN

// 接收管道1 地址
const byte addr[] = "1NODE";
const byte pipe = 1;
// 定义变量
int joyButton = 0;
struct JoyStickData {
    int xVal;
    int yVal;
    int buttonVal;
} joystickData;

void setup() {
  Serial.begin(9600);

  Wire.begin();
  delay(100);

  rf24.begin();
  rf24.setChannel(90);  // 129 - (2 * GN) 95
  rf24.setPALevel(RF24_PA_MAX);
  rf24.setDataRate(RF24_2MBPS);
  rf24.openReadingPipe(pipe, addr);  // 对于管道1
  rf24.startListening();
}

void loop() {
  RjoyStick();
  controlCar();
}

void RjoyStick()
{
  if (rf24.available(&pipe)) {
    rf24.read(&joystickData, sizeof(joystickData)); 
  }
  if (joystickData.buttonVal == 0)
  {
    joyButton = 1;
  }
  Serial.print("X: ");
  Serial.println(joystickData.xVal);
  Serial.print("Y: ");
  Serial.println(joystickData.yVal);
  Serial.print("Button: ");
  Serial.println(joystickData.buttonVal);
  delay(50);
}

void controlCar()
{
  const int threshold = 512; // 摇杆中心位置大约是 512
  const int range = 100;     // 用于判断摇杆偏移的范围, 612 圆外

  if (abs(joystickData.xVal - threshold) < range && abs(joystickData.yVal - threshold) < range) 
  {
    stop();  // 如果摇杆在中心位置，停止小车
  }

  if (joystickData.yVal < (threshold - range) ) 
  {
    goStraight("barrrr", 30);  // 后退
  }
  else if (joystickData.yVal > (threshold + range)) 
  {
    goStraight("baffff", 30);  // 前进 
  }
  else if (joystickData.xVal < (threshold - range))
  {
    turn("bafrrf", 30); // 右转
  }
  else if (joystickData.xVal > (threshold + range)) 
  {
    turn("barffr", 30);  // 左转
  }
  delay(50);
}

void goStraight(char str[], int num)  
{
  Wire.beginTransmission(42);
  Wire.write(str);

  int just = 5;
  Wire.write(num + just);
  Wire.write(0);

  Wire.write(num + just);
  Wire.write(0);

  Wire.write(num);
  Wire.write(0);

  Wire.write(num);
  Wire.write(0);

  Wire.endTransmission();
}

void turn(char str[], int num)
{
  Wire.beginTransmission(42);
  Wire.write(str);  // bafrrf right

  Wire.write(num);
  Wire.write(0);

  Wire.write(num);
  Wire.write(0);

  Wire.write(num);
  Wire.write(0);

  Wire.write(num);
  Wire.write(0);

  Wire.endTransmission();
}

void stop()
{
  Wire.beginTransmission(42);
  Wire.write("ha");
  Wire.endTransmission();
  delay(500);
}