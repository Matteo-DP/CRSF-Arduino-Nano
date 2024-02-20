#include <Arduino.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "config.h"
#include "CRSF.h"

class Controller
{
    // TODO: Change some values to private
    public:
        int magnetState = 0;
        int speedLeft = 0; // Unmapped left motor speed from axis
        int speedRight = 0; // Unmapped right motor speed from axis
        int sensitivity = SENSITIVITY;
        int motorLeft; // Left motor speed (0 - 255)
        int motorRight; // Right motor speed (0 - 255)

        // Difference calculations
        int lastThrottleValue = 0;
        int lastSteerLeftValue = 0;
        int lastSteerRightValue = 0;

        void throttle(int throttleValue) {
            int difference = throttleValue - lastThrottleValue;
            if(difference == 0) return; // Dont do anything when nothing changed

            speedLeft += difference;
            speedRight += difference;

            lastThrottleValue = throttleValue;
        };
        void steerLeft(int steerLeftValue) {
            // Assumes that steerLeftValue is positive
            int difference = steerLeftValue - lastSteerLeftValue;
            if(difference == 0) return; // Dont do anything when nothing changed

            if(speedLeft > lastThrottleValue) {
                speedLeft -= abs(difference * sensitivity);
            } else if(speedLeft < lastThrottleValue) {
                speedLeft += abs(difference * sensitivity);
            };
            speedRight += difference * sensitivity;

            lastSteerLeftValue = steerLeftValue;
        };
        void steerRight(int steerRightValue) {
            // Assumes that steerRightValue is positive
            int difference = steerRightValue - lastSteerRightValue;
            if(difference == 0) return; // Dont do anything when nothing changed

            speedLeft += difference * sensitivity;
            if(speedRight < lastThrottleValue) {
                speedRight += abs(difference * sensitivity);
            } else if(speedRight > lastThrottleValue) {
                speedRight -= abs(difference * sensitivity);
            };
            lastSteerRightValue = steerRightValue;
        };
        void steer(int steerValue) {
            // Redundant check
            if(steerValue < CRSF_CHANNEL_MIN) {
                steerValue = CRSF_CHANNEL_MIN;
            } else if(steerValue > CRSF_CHANNEL_MAX) {
                steerValue = CRSF_CHANNEL_MAX;
            };

            if(steerValue < CRSF_CHANNEL_MID) {
                steerLeft(abs(steerValue - 991));
            } else if(steerValue > CRSF_CHANNEL_MID) {
                steerRight(steerValue - 991);
            }
        };
        void update() {
            if(speedLeft < CRSF_CHANNEL_MIN) {
                speedLeft = CRSF_CHANNEL_MIN;
            } else if(speedLeft > CRSF_CHANNEL_MAX) {
                speedLeft = CRSF_CHANNEL_MAX;
            };
            if(speedRight < CRSF_CHANNEL_MIN) {
                speedRight = CRSF_CHANNEL_MIN;
            } else if(speedRight > CRSF_CHANNEL_MAX) {
                speedRight = CRSF_CHANNEL_MAX;
            };

            motorLeft = map(speedLeft, CRSF_CHANNEL_MIN, CRSF_CHANNEL_MAX, 0, 255);
            motorRight = map(speedRight, CRSF_CHANNEL_MIN, CRSF_CHANNEL_MAX, 0, 255);
            analogWrite(MOTORLEFTPIN, motorLeft);
            analogWrite(MOTORRIGHTPIN, motorRight);
        };
        void toggleMagnet() {
            // Toggle relay
            if(magnetState == 0) {
                magnetState = 1;
                digitalWrite(MAGNETPIN, HIGH);
            } else {
                magnetState = 0;
                digitalWrite(MAGNETPIN, LOW);
            }
        };
        void writeMagnet(int state) {
            // Relay on or off
            if(state == 0) {
                digitalWrite(MAGNETPIN, LOW);
            } else if(state == 1) {
                digitalWrite(MAGNETPIN, HIGH);
            } else {
                return; // Error
            }
        }
};

#endif