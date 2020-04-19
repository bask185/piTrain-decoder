
#include <Arduino.h>
#include "io.h"
#include <EEPROM.h>
#include <Wire.h>
#include "../../terminal.h"
#include "../modules/Adafruit_PWMServoDriver.h"

//ServoDriver servo;
Mcp23017 mcp[] {	// 2 max
	Mcp23017(),
	Mcp23017(),
	Mcp23017(),
	Mcp23017() };

PCA9685 pca9685[] {
	PCA9685(),
	PCA9685()
};

byte nMcp, nServoDrivers;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();



void setServo(byte pin, byte _state) {
	if(_state) _state = curvedPos;
	else 	   _state = straightPos;

	if(pin < 16) pca9685[0].setServo(ID, _state);
	else 		 pca9685[1].setServo(ID, _state);
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
	if(state)	input |=  (1 << pin);
	else		input &= ~(1 << pin);

	mcp[xMcp].setPort(port, input); }

extern void initIO(void) {
	Wire.begin();

	pwm.begin();
	pwm.setOscillatorFrequency(27000000);
	pwm.setPWMFreq(50);  // Analog servos run at ~50 Hz updates

	unsigned int ioDir[4] = {0,0,0,0};

	loadEEPROM(&nMcp, &nServoDrivers, ioDir);


	// EEPROM.write(512, 1);
	// EEPROM.write(513, turnoutObject);
	// EEPROM.write(514, SERVO);
	// EEPROM.write(515, 45);
	// EEPROM.write(516, 135);

	for(byte j = 0 ; j < nMcp ; j++ ) {
		mcp[j].init(mcpBaseAddress + j , ioDir[j]);
	}
	nServoDrivers = 1;
	for(byte j = 0 ; j < nServoDrivers ; j++ ) {
		pca9685[j].init(mcpBaseAddress + j);
	}

	pinMode(transmissionDir, OUTPUT);
	pinMode(ledPin, OUTPUT);

	pca9685[0].setServo(1, 90);
}