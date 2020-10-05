
#include <Arduino.h>
#include "io.h"
#include <EEPROM.h>
#include <Wire.h>
#include "../../terminal.h"
#include "timers.h"

//#include "../modules/Adafruit_PWMServoDriver.h"

byte debugMode = 0;

Mcp23017 mcp[] {	// 2 max
	Mcp23017(),
	Mcp23017(),
	Mcp23017(),
	Mcp23017() };

byte nMcp;

Adafruit_PWMServoDriver servoDriver = Adafruit_PWMServoDriver();


void beginTransmission() {
	digitalWrite(transmissionDir, HIGH);
	digitalWrite(ledPin, HIGH);
};

void setServo(byte pin, byte _state) {
	unsigned int us;

	_state &= 0b1;	// and mask to so only the 1st bit is examined
	if(_state) { _state = curvedPos;/* Serial.println(F("curvedPos ")); Serial.println(curvedPos);*/}
	else 	   { _state = straightPos;/*Serial.println(F("straightPos ")); Serial.println(straightPos);*/}

	us = map(_state, 0, 180, 120, 490);
//	Serial.println(F("us "));Serial.println(us);

	servoDriver.setPWM(pin, 0, us);
}

void setOutput(byte output, byte _state) {	//  ID prev is to be cleared, ID is to be set
	static byte pinPrev, xMcpPrev;

	byte element, xMcp, pin, port, input;
	//unsigned int input;
	xMcp = output / 16;
	pin = output % 16; // < works

	if(pin < 8) {
		port = portB; }
	else {
		port = portA;
		pin -= 8; }

	input = mcp[xMcp].getInput(port);

	_state &= 0b1;	// and mask to so only the 1st bit is examined
	if(_state)	input |=  (1 << pin);
	else		input &= ~(1 << pin);

	mcp[xMcp].setPort(port, input); }

void initIO(void) {
	beginTransmission();
	Serial.println(F("BOOTING I2C"));
	Wire.begin();
	
	//servoDriver.begin();
	//servoDriver.setOscillatorFrequency(27000000);
	//servoDriver.setPWMFreq(50);  // Analog servos run at ~50 Hz updates
	Serial.println(F("I2C BOOTED"));

	unsigned int ioDir[4] = {0,0,0,0};
	Serial.println(F("LOADING EEPROM"));
	loadEEPROM(&nMcp, ioDir);

	// for(byte j = 0 ; j < nMcp ; j++ ) {
	 //	mcp[j].init(mcpBaseAddress + j , ioDir[j]);
	// }
	 Serial.println(F("EEPROM LOADED"));
}