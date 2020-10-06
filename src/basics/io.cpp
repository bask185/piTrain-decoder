
#include <Arduino.h>
#include "io.h"
#include <EEPROM.h>
#include <Wire.h>
#include "../../terminal.h"
#include "timers.h"

//#include "../modules/Adafruit_PWMServoDriver.h"

byte debugMode = 0;

Mcp23017 mcp[] {	// 4 max CONSTRUCT 4 MCP OPBJECTS AND PUT THEM IN AN ARRAY
	Mcp23017(),
	Mcp23017(),
	Mcp23017(),
	Mcp23017() };
byte nMcp = 4;		// We assume first that all 4 slaves are present, we check later on

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
	Serial.println(F("INITIALIZING SERVO DRIVER"));
	Wire.begin();
	
	//servoDriver.begin();
	//servoDriver.setOscillatorFrequency(27000000);
	//servoDriver.setPWMFreq(50);  // Analog servos run at ~50 Hz updates
	Serial.println(F("SERVO DRIVER INITIALIZED"));

	unsigned int ioDir[4] = {0,0,0,0};
	Serial.println(F("LOADING EEPROM"));
	loadEEPROM(&ioDir); // this functions reads the eeprom and fills the ioDir array to set inputs and outputs
	Serial.println(F("EEPROM LOADED"));


	Serial.println(F("INITIALIZING MCP230127 DEVICES"));
	for(byte j = 0 ; j < nMcp ; j++ ) {						// check if all 4 slaves are present and set their IO dir registers
		if( mcp[j].init(mcpBaseAddress + j , ioDir[j]) ) {  // if function returns true, the slave did NOT respond
			nMcp = i ;
			break ; 
		}
	}
	if( nMcp ) {
		Serial.print( nMcp ) ;
		Serial.println(F(" MCP DEVICES DETECTED"));
	} else {
		Serial.println(F("NO MCP SLAVES DETECTED"));
	}
}