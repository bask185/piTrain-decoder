
#include <Arduino.h>
#include "io.h"
#include <EEPROM.h>
#include <Wire.h>
#include "../../terminal.h"
//#include "../modules/Adafruit_PWMServoDriver.h"

Mcp23017 mcp[] {	// 2 max
	Mcp23017(),
	Mcp23017(),
	Mcp23017(),
	Mcp23017() };

byte nMcp;

Adafruit_PWMServoDriver servoDriver = Adafruit_PWMServoDriver();


void setServo(byte pin, byte _state) {
	unsigned int us;

	state &= 0b1;	// and mask to so only the 1st bit is examined
	if(_state) { _state = curvedPos;/* Serial.print("curvedPos "); Serial.println(curvedPos);*/}
	else 	   { _state = straightPos;/*Serial.print("straightPos "); Serial.println(straightPos);*/}

	us = map(_state, 0, 180, 120, 490);
//	Serial.print("us ");Serial.println(us);

	servoDriver.setPWM(pin, 0, us);
}

void setOutput(byte output, byte state) {	//  ID prev is to be cleared, ID is to be set
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

	state &= 0b1;	// and mask to so only the 1st bit is examined
	if(state)	input |=  (1 << pin);
	else		input &= ~(1 << pin);

	mcp[xMcp].setPort(port, input); }

extern void initIO(void) {
	Wire.begin();
	
	servoDriver.begin();
	servoDriver.setOscillatorFrequency(27000000);
	servoDriver.setPWMFreq(50);  // Analog servos run at ~50 Hz updates

	unsigned int ioDir[4] = {0,0,0,0};

	loadEEPROM(&nMcp, ioDir);

	 for(byte j = 0 ; j < nMcp ; j++ ) {
	 	mcp[j].init(mcpBaseAddress + j , ioDir[j]);
	 }

	pinMode(transmissionDir, OUTPUT);
	pinMode(ledPin, OUTPUT);
}