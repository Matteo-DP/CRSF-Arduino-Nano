#ifndef CONFIG_H
#define CONFIG_H

// Enable debug mode: no PWM output, only serial output
#define DEBUG
// Enable CRSF debug mode: show raw CRSF ch values
// #define CRSFDEBUG

#define ENA 6
#define ENB 5
#define IN1 4
#define IN2 3
#define IN3 2
#define IN4 7

#define SENSITIVITY 1
#define MOTORLEFTPIN ENA
#define MOTORRIGHTPIN ENB
#define MAGNETPIN 0

#define DIVIDE 1

#endif