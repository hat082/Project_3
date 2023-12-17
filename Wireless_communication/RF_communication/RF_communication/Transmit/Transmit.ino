#include <SPI.h>
#include "RF24.h"

RF24 rf24(7,8); // CE, CSN

const byte addr[] = "1Node";
const char msg[] = "Happy Hacking!";

void setup() {
  Serial.begin(9600);
  rf24.begin();
  rf24.setChannel(83);       
  rf24.openWritingPipe(addr); 
  rf24.setPALevel(RF24_PA_MAX);   
  rf24.setDataRate(RF24_2MBPS); 
  rf24.stopListening();       
}

void loop() {
  if (!rf24.write(&msg, sizeof(msg)))  
  {
    Serial.println("Fail");
  }
  delay(1000);
}