#include "terminal.h"
#include "src/modules/makeNumber.h"
#include "src/basics/io.h"
#include "src/basics/timers.h"
#include <EEPROM.h>


static byte subCommand = 0;
byte Array[8]={255,255,255,255,255,255,255,255}, serialByte ,IO, firstEntry;

#define terminalCommand(x) static byte x##F()

terminalCommand(getType) {
	if(firstEntry) { firstEntry = 0;
		for(byte j = 0 ; j < 8 ; j++ ) Array[j] = 255; // initialize array before we fill in
		Serial.write(12);
		Serial.println(F("Enter type"));
		Serial.println(F("1 = turnout"));
		Serial.println(F("2 = memory"));
		Serial.println(F("3 = detector"));
		Serial.println(F("4 = signal"));
		Serial.println(F("5 = decoupler"));
		Serial.println(F("6 = light Circuit")); 
		Serial.println(F("7 = remove item")); }

	switch(serialByte) {
		case '1': Serial.println(F("\rturnoutObject      selected"));	type = turnoutObject; 		return 0;
		case '3': Serial.println(F("\rdetectorObject     selected"));	type = detectorObject; 		return 0;
		case '2': Serial.println(F("\rmemoryObject       selected"));	type = memoryObject;		return 0;
		case '5': Serial.println(F("\rdecouplerObject    selected"));	type = decouplerObject; 	return 0;
		case '4': Serial.println(F("\rsignalObject       selected"));	type = signalObject; 		return 0;
		case '7': Serial.println(F("\rremoveDevice       selected"));	type = removeDevice;  		return 0;
		case '6': Serial.println(F("\rlightCircuitObject selected"));	type = lightCircuitObject;	return 0;
		case '\r': return 1;
		default: return 0; } } 



terminalCommand(getID) {
	if(firstEntry) { firstEntry = 0;
		ID = 0;
		Serial.println(F("select ID 1-254, press ENTER when ready"));}
	if(serialByte && makeNumber(&ID,serialByte,1,255,'\n')) return 1; 
	return 0; }

terminalCommand(getIO) {
	if(firstEntry) { firstEntry = 0;
		IO = 0;
		Serial.println(F("select IO 0-63, press ENTER when ready")); }
	if(serialByte && makeNumber(&IO,serialByte,0,63,'\n')) {
		return 1; }
	return 0;}

terminalCommand(getDecouplerIO) {
    if(firstEntry) { firstEntry = 0;
        Serial.println(F("On which output is the decoupler?")); }
    if(serialByte && makeNumber(&outputIO,serialByte,0,63,'\n')) {
        return 1; }
    return 0;}

terminalCommand(hasLed) {
	if(firstEntry) { firstEntry = 0;
		Serial.println(F("Does it have an LED [Y/n]\r\n")); }
	switch(serialByte) {
		case 'Y': case 'y': hasLedIO = YES; Serial.println(F("  YES\r")); return 0; 
		case 'n': case 'N': hasLedIO = NO; Serial.println(F("  NO \r"));  return 0; 
		case '\r': return 1;
		default  : return 0; } }

terminalCommand(getLedIO) {
    if(firstEntry) { firstEntry = 0;
        Serial.println(F("What is IO of LED")); }
    if(serialByte && makeNumber(&ledIO,serialByte,0,63,'\n')) {
        return 1; }
    return 0; }

terminalCommand(getSignalType){
	if(firstEntry) { firstEntry = 0;
		Serial.println(F("What kind of turnout is this one?"));
		Serial.println(F("1 = Servo"));
		Serial.println(F("2 = 2 LED"));
		Serial.println(F("3 = 3 LED")); }
	if(serialByte && makeNumber(&signalType,serialByte,1,3,'\n')) {
		return 1; }
	return 0; } 

terminalCommand(getGreenIO) {
	if(firstEntry) { firstEntry = 0;
		Serial.println(F("What is IO of green LED")); }
	if(serialByte && makeNumber(&greenIO,serialByte,0,63,'\n')) {
		return 1; }
	return 0; }

terminalCommand(getYellowIO) {
	if(firstEntry) { firstEntry = 0;
		Serial.println(F("What is IO of yellow LED")); }
	if(serialByte && makeNumber(&yellowIO,serialByte,0,63,'\n')) {
		return 1; }
	return 0; }

terminalCommand(getRedIO) {
	if(firstEntry) { firstEntry = 0;
		Serial.println(F("What is IO of red LED")); }
	if(serialByte && makeNumber(&redIO,serialByte,0,63,'\n')) {
		return 1; }
	return 0; }

terminalCommand(getSwitchType) {
    if(firstEntry) { firstEntry = 0;
        Serial.println(F("What kind of turnout is this one?"));
        Serial.println(F("1 = Servo"));
        Serial.println(F("2 = Relay"));
        Serial.println(F("3 = 2 Coils")); }
    if(serialByte && makeNumber(&switchType,serialByte,1,3,'\n')) {
        return 1; }
    return 0; } 

terminalCommand(curvedOrStraight) {
	if(firstEntry) { firstEntry = 0;
		Serial.println(F("Turnout is now set at straight"));
		Serial.println(F("Is it curved?")); }
	switch(serialByte) {
		case 'Y': case 'y': hasLedIO = YES; Serial.println(F("  YES\r")); break;
		case 'n': case 'N': hasLedIO = NO;  Serial.println(F("  NO \r"));break;
		case '\r': return 1;
		default : return 0; } }

terminalCommand(adjustCurvedPosition) {
	if(firstEntry) { firstEntry = 0;
		curvedPos = 90;
		Serial.println(F("adjust curved position with '-' and '+', press ENTER when ready"));
	}
	if( serialByte == '+' ) curvedPos++;
	if( serialByte == '-' ) curvedPos--;
	curvedPos = constrain( curvedPos, 6 , 170);
	Serial.print('\r'); Serial.println(curvedPos);

	unsigned int us = map(curvedPos, 0, 180, 120, 490);
	//servoDriver.setPWM(IO, 0, us); // update motor at once
	if( serialByte == '\n' || serialByte == '\r' ) return 1;
	return 0; }

terminalCommand(adjustStraightPosition) {
	if(firstEntry) { firstEntry = 0;
		straightPos = 90;
		Serial.println(F("adjust straight position with '-' and '+', press ENTER when ready"));
	}
	if( serialByte == '+' ) straightPos++;
	if( serialByte == '-' ) straightPos--;
	straightPos = constrain( straightPos, 6 , 170);
	Serial.print(straightPos);Serial.println('\r');

	unsigned int us = map(straightPos, 0, 180, 120, 490);
	//servoDriver.setPWM(IO, 0, us); // update motor at once

	if( serialByte == '\n' || serialByte == '\r'  ) return 1;
	return 0; }

terminalCommand(getLightCircuitIO) {
	if(firstEntry) { firstEntry = 0;
		Serial.println(F("On which output is the light circuit relay?")); }
	if(serialByte && makeNumber(&outputIO,serialByte,0,63,'\n')) {
		return 1; }
	return 0;}

terminalCommand(removeDevice) {
    if(firstEntry) { firstEntry = 0;
        Serial.println(F("enter IO of to be removed device")); }
    if(serialByte && makeNumber(&IO,serialByte,0,255,'\n')) {
		Serial.println(F("object deleted")); 
		return 1;}
    return 0; }

terminalCommand(storeObject) {
	static byte response = 0;
	if(firstEntry) { firstEntry = 0;
		Serial.println(F("STORE OBJECT? press [Y/n]")); }
	switch(serialByte) {
		case 'Y': case 'y': response = YES; Serial.println(F("  YES\r")); break;
		case 'n': case 'N': response = NO;  Serial.println(F("  NO \r"));break;
		case '\r': 
		if(response == YES) {
			Serial.println(F("object SAVED"));
			store(); }
		else {
			Serial.println(F("object DISCARDED")); }
		delay(3000);
		return 1;
		default : return 0; } }
#undef terminalCommand

extern byte getSubCommand() {
    return subCommand;
}

static void nextCommand(byte x) {
	Serial.println();
	Serial.write(12);
	serialByte = 0;
	subCommand = x; 
	firstEntry = 1;
	menuF(); // take note this is a recursive call, it was the easiest way to print the new texts during state transition
	if(!subCommand) Serial.write(12); } 

#define terminalCommand(x) break; case x: if(x##F())
extern byte menuF() { 		// called from main
	beginTransmission();	// upon every received byte, we are most likely going to send stuff back

	switch(subCommand){
		default: {
			nextCommand(getType); }
		
		terminalCommand(getType) {
			if(type == removeDevice)	nextCommand(removeDevice);
			else						nextCommand(getID); }
		
		terminalCommand(getID) 	{
			nextCommand(getIO); }
		
		terminalCommand(getIO) {		
			if	   ( type == turnoutObject)			nextCommand(getSwitchType);
			else if( type == decouplerObject)		nextCommand(getDecouplerIO);
			else if( type == signalObject)			nextCommand(getSignalType);
			else if( type == lightCircuitObject)	nextCommand(getLightCircuitIO);
			else 									nextCommand(hasLed); }

		terminalCommand(getSignalType) {
			if( signalType == SERVO_SIGNAL ) nextCommand(adjustCurvedPosition);
			else 					 		 nextCommand(getGreenIO); }

		terminalCommand(getGreenIO) {
			if(signalType == _2LED ) nextCommand(getRedIO);
			else 					 nextCommand(getYellowIO); }

		terminalCommand(getYellowIO) {
			nextCommand(getRedIO);	}

		terminalCommand(getRedIO) {
			nextCommand(storeObject); }

		terminalCommand(curvedOrStraight) {
			nextCommand(storeObject); }

		terminalCommand(hasLed) {	
			if(hasLedIO == YES) nextCommand(getLedIO);	
			else 				nextCommand(storeObject); }

		terminalCommand(getLedIO) {		
			nextCommand(storeObject); }

		terminalCommand(getSwitchType) {
			if( switchType == COILS 
			||  switchType == RELAY ) {
				 nextCommand(curvedOrStraight); }
			else nextCommand(adjustCurvedPosition); }

		terminalCommand(getLightCircuitIO) {
			nextCommand(hasLed); }

		terminalCommand(getDecouplerIO) {
			nextCommand(hasLed); }

		terminalCommand(adjustCurvedPosition) {
			nextCommand(adjustStraightPosition); }

		terminalCommand(adjustStraightPosition) {
			nextCommand(storeObject); }

		terminalCommand(removeDevice) {
			nextCommand(0);  
			return 1; }
		
		terminalCommand(storeObject) {
			nextCommand(0);
			return 1; }
		break; }
	return 0; }
#undef terminalCommand

static void store() {
	byte j;
	if( debugMode ) Serial.println(F("IO "));Serial.println(IO);
	uint16_t eeAddress = IO * 8;  // the physical IO is linked with it's position the EEPROM

	if( (type == turnoutObject && switchType == SERVO) 
	||	 type == signalObject  && signalType == SERVO_SIGNAL) {
		eeAddress += (elementAmmount * 8); } // servo motors can have same IO as not servo devices
	EEPROM.put(eeAddress, Array); }



extern void loadEEPROM(uint16_t *iodir){ // returns ammount of requied MCP23017 slaves depending on taught in IO
	byte j, i, highestIO = 0, highestTurnoutIO = 0, element;
	uint16_t eeAddress;

	for(element = 0; element < elementAmmount ; element++) { // 64x
		byte xMcp = element / 16;
		byte pin = element % 16;
		
		eeAddress = element * 8;
		EEPROM.get(eeAddress, Array);

		// N.B all IO is an OUTPUT by default. Therefor we only need to set ioDir bits for INPUTS
		if(type == memoryObject || type == detectorObject || type == decouplerObject) {
			iodir += xMcp; 									// match iodir's address to corresponding mcp23017
			*iodir |= 0x01 << pin; 							// flag pin as input
			iodir -= xMcp; 									// set address back
		}
	}
}
