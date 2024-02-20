#include <Arduino.h>
#include "CRSF.h"
#include "controller.cpp"
#include "config.h"

CRSF crsf;
Controller controller;

void setup(){
  crsf.begin();
  Serial.begin(420000);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){
  crsf.GetCrsfPacket();

  // Turn indicator light on.
  if(crsf.failsafe_status == CRSF_SIGNAL_LOST){
    digitalWrite(LED_BUILTIN, 1);
  }else{
    digitalWrite(LED_BUILTIN, 0);
  }

  crsf.UpdateChannels();

  #ifdef CRSFDEBUG
    Serial.print(crsf.channels[0]); 
    Serial.print(" ");
    Serial.print(crsf.channels[1]); 
    Serial.print(" ");
    Serial.print(crsf.channels[2]); 
    Serial.println();
  #endif

  controller.throttle(crsf.channels[2]);
  controller.steer(crsf.channels[0]);
  controller.update();

  #ifdef DEBUG
    Serial.print(controller.motorLeft); 
    Serial.print(" ");
    Serial.print(controller.motorRight); 
    Serial.println();
  #endif
  
  delay(5);
}