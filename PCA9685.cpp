#include "PCA9685.h"
#include <Wire.h>

enum controlRegisters {
    MODE1                = 0x00,
    MODE2                = 0x01,
    SUBADR1                = 0x02,
    SUBADR2                = 0x03,
    SUBADR3                = 0x04,
    ALLCALLADR            = 0x05,
    SERVO_BASE_ADDRESS    = 0x06,
    PRE_SCALE            = 0xFE };

union {
    unsigned char mode1;
    struct mode1str {
        byte ALLCALL:1;
        byte SUB3:1;
        byte SUB2:1;
        byte SUB1:1;
        byte SLEEP:1;
        byte AI:1;
        byte EXTCLK:1;
        byte RESTART:1;
    }; 
} Union;

struct mode2 {
    byte OUTNE1:1; 
    byte OUTNE2:1;
    byte OUTDRV:1;
    byte OCH:1;
    byte INVRT:1; };

PCA9685::PCA9685() {;}

// prescale value = (25MHZ / 4096 * 60) - 1;

void PCA9685::init(byte _address) {
    address = _address; 
    //mode1.AI = 1;
    //mode1.SLEEP = 1;

    Wire.beginTransmission(address);
    Wire.write(MODE1);
    Wire.write(Union.mode1); 
    Wire.endTransmission();

    // PRESCALER = (25000000 / (4096 * 60)) - 1; // = 100
    byte prescaler = 100;
    Wire.beginTransmission(address);
    Wire.write(PRE_SCALE);
    Wire.write(prescaler); 
    Wire.endTransmission();

    //mode1.SLEEP = 0;
    Wire.beginTransmission(address);
    Wire.write(MODE1);
    Wire.write(Union.mode1); 
    Wire.endTransmission(); }


//#define LOW(x) (x&0x00FF)
//#define HIGH(x) (x>>8)

void PCA9685::setServo(byte ID, byte position) {
    unsigned int onTime, offTime, ms, pulseDuration, pulseStart;
    unsigned char servoAddress;

    servoAddress = SERVO_BASE_ADDRESS + (4 * ID);
    ms = map(position, 0, 180, 1000, 2000);  // map degrees to milliseconds

    pulseStart = 0;
    //pulseDuration = map();

    Wire.beginTransmission(address);
    Wire.write(servoAddress);
    Wire.write(pulseStart);
    Wire.write(pulseStart >> 8);
    Wire.write(pulseDuration);
    Wire.write(pulseDuration >> 8);
    Wire.endTransmission(); }
//#undef LOW
//#undef HIGH