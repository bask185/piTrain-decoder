#include <Arduino.h>
#include "../modules/mcp23017.h"
#include "../modules/Adafruit_PWMServoDriver.h"

extern void beginTransmission();

extern byte debugMode;

extern Mcp23017 mcp[];
// extern PCA9685  pca9685[];
extern byte nMcp, nServoDrivers;

extern Adafruit_PWMServoDriver servoDriver;

extern void setOutput(byte, byte);
extern void setServo(byte, byte);


#define transmissionDir 2
#define debugPin 3
#define ledPin 13


extern void initIO(void);