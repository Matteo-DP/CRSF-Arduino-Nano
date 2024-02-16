#include <Arduino.h>
#include "config.h"

class Controller {
    public:
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

            speedLeft -= difference * sensitivity;
            speedRight += difference * sensitivity;

            lastSteerLeftValue = steerLeftValue;
        };
        void steerRight(int steerRightValue) {
            // Assumes that steerRightValue is positive
            int difference = steerRightValue - lastSteerRightValue;
            if(difference == 0) return; // Dont do anything when nothing changed

            speedLeft += steerRightValue * sensitivity;
            speedRight -= steerRightValue * sensitivity;

            lastSteerRightValue = steerRightValue;
        };
        void update() {
            if(speedLeft < STEERMIN) {
                speedLeft = STEERMIN;
            } else if(speedLeft > STEERMAX) {
                speedLeft = STEERMAX;
            };

            motorLeft = map(speedLeft, STEERMIN, STEERMAX, 0, 255);
            motorRight = map(speedRight, STEERMIN, STEERMAX, 0, 255);
            analogWrite(MOTORLEFTPIN, motorLeft);
            analogWrite(MOTORRIGHTPIN, motorRight);
        };
} controller;