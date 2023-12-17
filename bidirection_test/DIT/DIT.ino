#include <SPI.h>
#include "RF24.h"

RF24 rf24(9,10); // // 创建 RF24 类的实例 rf24 CE, CSN
const byte addr[] = "1NODE";  // 数组中的每个元素都是一个字节, 配置地址
// 定义引脚
int Xin= A0; // X Input Pin
int Yin = A1; // Y Input Pin
int KEYin = 3; // Push Button
// 定义变量
struct JoyStickData {
    int xVal;
    int yVal;
    int buttonVal;
} joystickData;

void setup() {
  Serial.begin (9600); 

  rf24.begin();
  rf24.setChannel(90);  // initial 83
  rf24.openWritingPipe(addr);  
  rf24.setPALevel(RF24_PA_MAX);  // 以最大功率广播信号
  rf24.setDataRate(RF24_2MBPS);  // 设置数据传输速率为2Mbps
  rf24.stopListening();  // 停止接收数据，准备发送数据。  

  pinMode (KEYin, INPUT_PULLUP); // 输入上拉
  
}

void loop() {
  joyStick();
  rf24.write(&joystickData, sizeof(joystickData));  // Success 1
  delay(100);

}

void joyStick()
{
  joystickData.xVal = analogRead(Xin);
  joystickData.yVal = analogRead(Yin);
  joystickData.buttonVal = digitalRead(KEYin);
  
  Serial.print("X = ");
  Serial.println (joystickData.xVal, DEC);  
  
  Serial.print ("Y = ");
  Serial.println (joystickData.yVal, DEC);
  
  Serial.print("Button is ");
  if (joystickData.buttonVal == HIGH){
  Serial.println ("not pressed");
  }
  else{
  Serial.println ("PRESSED");
  }
  delay (100);
}