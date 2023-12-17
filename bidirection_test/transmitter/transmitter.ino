#include <SPI.h>
#include "RF24.h"
#include <LiquidCrystal.h>


RF24 rf24(9, 10); // CE, CSN

const byte addr[] = "1Node";
const byte addr2[] = "2Node";
const byte pipe = 1;

struct DataPacket {
  int xVal;
  int yVal;
  int buttonVal;
  char command[8];
} data; 

struct displayPakDef
{
  int deltaDistance;
  int inclination;
  int angle;
} displayPak;

int Xin = A0; 
int Yin = A1; 
int KEYin = 6; 

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  rf24.begin();
  rf24.setChannel(101);
  rf24.openWritingPipe(addr);
  rf24.openReadingPipe(pipe, addr2);
  rf24.setPALevel(RF24_PA_MAX);
  rf24.setDataRate(RF24_2MBPS);
  rf24.stopListening();
  Serial.begin(9600);
  lcd.begin(16,2);
}

float distance = 0; 

void loop() {
  data.xVal = analogRead(Xin);
  data.yVal = analogRead(Yin);
  data.buttonVal = digitalRead(KEYin);

  // 逻辑判断部分
  // if (data.yVal >= 800 && data.yVal <= 1023) {
  //   strcpy(data.command, "baffff"); // 前进
  // } else if (data.yVal >= 0 && data.yVal <= 200) {
  //   strcpy(data.command, "barrrr"); // 后退
  // } 
  //   else if (data.xVal >= 800 && data.xVal <= 1023) {
  //   strcpy(data.command, "baffrr"); // 左旋转
  // } else if (data.xVal >= 0 && data.xVal <= 200) {
  //   strcpy(data.command, "barrff"); // 右旋转
  // } else {
  //   strcpy(data.command, "ha"); // 其他情况停止
  // }


  //mpu


  //run stop velocity distance


  rf24.write(&data, sizeof(data));

  // test
  // rf24.startListening();
  // delay(500);
  // // while(!rf24.available(&pipe));
  // rf24.read(&displayPak, sizeof(displayPak));

  // Serial.println(displayPak.angle);
  // Debugging info
  Serial.print("X = "); Serial.println(data.xVal);
  Serial.print("Y = "); Serial.println(data.yVal);
  Serial.print("Button is "); Serial.println(data.buttonVal == HIGH ? "not pressed" : "PRESSED");
  Serial.print("Command: "); Serial.println(data.command);

  // if (strcmp(data.command, "barrrr") == 0)
  // {
  //   distance -= 2.5 * 0.01 * (5 + rand() % 10);
  // }
  // else if (strcmp(data.command, "ha") == 0)
  // {

  // }
  // else
  // {
  //   distance += 2.5 * 0.01 * (5 + rand() % 10);
  // }

  lcd.setCursor(0, 0);
  lcd.print("Command: "); lcd.print(data.command); lcd.print("     ");
  lcd.setCursor(0, 1);
  lcd.print("Distance: "); lcd.print(distance); lcd.print("    ");
  delay(500);
}
