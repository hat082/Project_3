/*
* Arduino Wireless Communication Tutorial
*     Example 2 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address0[] = "Node0";
const byte address1[] = "Node1";

typedef enum {
  controller,
  vehicle
} roleDef;
roleDef role;

typedef struct
{
  int x = 0;
  int y = 0;
}
controlDef;
controlDef controlPak;

typedef struct
{
  unsigned int distance = 0;
  int inclination = 0;
}
displayDef;
displayDef displayPak;

int Xin = A0;   // X Input Pin
int Yin = A1;   // Y Input Pin

void setup() {
  pinMode(3, INPUT);
  Wire.begin();
  if (digitalRead(3) == HIGH)
  {
    role = vehicle;
  }
  else
  {
    role = controller;
  }
  Serial.begin(115200);
  
  radio.begin();
  radio.setChannel(101); 
  if ( role == vehicle )
  {
    radio.openWritingPipe(address0);
    radio.openReadingPipe(1, address1);
  }
  else
  {
    radio.openWritingPipe(address1);
    radio.openReadingPipe(1, address0);
  }
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x34);
  radio.enableDynamicPayloads();
  radio.setDataRate(RF24_2MBPS);
  radio.enableAckPayload();               // not used here
  radio.setRetries(0, 15);                // Smallest time between retries, max no. of retries
  // radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  radio.powerUp();
}

int motors[4] = {0, 0, 0, 0};
int prevMotors[4] = {0, 0, 0, 0};
void loop()
{
  if (role == controller)
  {
    unsigned long loop_start = millis();
    controlPak.x = analogRead(Xin);
    controlPak.y = analogRead(Yin);
    // Serial.println(F("control pak written."));
    // First, stop listening so we can talk.
    radio.stopListening();
    if (!radio.write(&controlPak, sizeof(controlPak))) {
      // Serial.println(F("Send failed."));
    }
    else
    {
      // Serial.println("Sent: ");
    }
    // const char msg[] = "Hello";
    // if (!radio.write(&msg, sizeof(msg)))
    // {
    //   Serial.println("Nope");
    // }
    radio.startListening();
    // Serial.println(F("delivery success."));
    // Serial.println(millis() - loop_start);
    //  && (millis() - loop_start) < 200
    // get the telemetry data
    uint8_t pipe;
    if (radio.available(&pipe))
    {
      radio.read(&displayPak, sizeof(displayPak));
      Serial.print("Reponse from vehicle: ");
      Serial.println(displayPak.distance);
    }

  }
  if (role == vehicle)
  {
    unsigned long loop_start = millis();
    displayPak.distance = 40;
    // Serial.println(F("display pak written."));
    // First, stop listening so we can talk.
    radio.stopListening();
    if (!radio.write(&displayPak, sizeof(displayPak))) {
      // Serial.println(F("Send failed."));
    }
    else
    {
      Serial.print("Sent: ");
      Serial.println(displayPak.distance);
    }
    radio.startListening();
    // Serial.println(F("delivery success."));
    // Serial.println(millis() - loop_start);
    radio.read(&controlPak, sizeof(controlPak));
    uint8_t pipe;
    if (radio.available(&pipe))
    {
      Serial.print("Command from controller: ");    
      Serial.print(controlPak.x); 
      Serial.print("\t");
      Serial.println(controlPak.y); 
    }

    // for (int i = 0; i < 4; i++)
    // {
    //   motors[i] = (float) (controlPak.x - 512) / 512 * 80 ;
    //   if (abs(prevMotors[i] - motors[i]) > 20)
    //   {
    //     for (int j = 0; j < 4; j++)
    //     {
    //       prevMotors[j] = motors[j];
    //     }
    //     delay(10);
    //   }
    //   Serial.print(motors[i]);
    //   Serial.print("\t");
    // }
    // Serial.println("");
  }
}

void moveForward() {
  Wire.beginTransmission(42);
  Wire.write("baffff");
  for (int i = 0; i < 4; i++) {
  Wire.write((int)(motors[i]));  
  Wire.write(0);
  }
  Wire.endTransmission();
}