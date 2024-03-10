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
  
  // Pin modes for L298N
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set initial motor direction to forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  // Motors 0 speed initially
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

}

void loop(){
  crsf.GetCrsfPacket();

  // Turn indicator light on.
  Serial.println(crsf.failsafe_status);
  // If signal is good, start processing the channels for motor control
  digitalWrite(LED_BUILTIN, 0);
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