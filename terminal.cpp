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
		Serial.println();
		Serial.write(12);
		Serial.println("Enter type");
		Serial.println("1 = turnout");
		Serial.println("2 = memory");
		Serial.println("3 = detector");
		Serial.println("4 = signal");
		Serial.println("5 = decoupler");
		Serial.println("6 = light Circuit"); 
		Serial.println("7 = remove item"); }

	switch(serialByte) {
		case '1': Serial.println("\rturnoutObject      selected");	type = turnoutObject; 		return 0;
		case '3': Serial.println("\rdetectorObject     selected");	type = detectorObject; 		return 0;
		case '2': Serial.println("\rmemoryObject       selected");	type = memoryObject;		return 0;
		case '5': Serial.println("\rdecouplerObject    selected");	type = decouplerObject; 	return 0;
		case '4': Serial.println("\rsignalObject       selected");	type = signalObject; 		return 0;
		case '7': Serial.println("\rremoveDevice       selected");	type = removeDevice;  		return 0;
		case '6': Serial.println("\rlightCircuitObject selected");	type = lightCircuitObject;	return 0;
		case '\r': return 1;
		default: return 0; } }



terminalCommand(getID) {
	if(firstEntry) { firstEntry = 0;
		ID = 0;
		Serial.println("select ID 1-254, press ENTER when ready");}
	if(serialByte && makeNumber(&ID,serialByte,1,255,'\n')) return 1; 
	return 0; }

terminalCommand(getIO) {
	if(firstEntry) { firstEntry = 0;
		IO = 0;
		Serial.println("select IO 0-63, press ENTER when ready"); }
	if(serialByte && makeNumber(&IO,serialByte,0,63,'\n')) {
		return 1; }
	return 0;}

terminalCommand(getDecouplerIO) {
    if(firstEntry) { firstEntry = 0;
        Serial.println("On which output is the decoupler?"); }
    if(serialByte && makeNumber(&outputIO,serialByte,0,63,'\n')) {
        return 1; }
    return 0;}

terminalCommand(hasLed) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("Does it have an LED [Y/n]\r\n"); }
	switch(serialByte) {
		case 'Y': case 'y': hasLedIO = YES; Serial.println("  YES\r"); return 0; 
		case 'n': case 'N': hasLedIO = NO; Serial.println("  NO \r");  return 0; 
		case '\r': return 1;
		default  : return 0; } }

terminalCommand(getLedIO) {
    if(firstEntry) { firstEntry = 0;
        Serial.println("What is IO of LED"); }
    if(serialByte && makeNumber(&ledIO,serialByte,0,63,'\n')) {
        return 1; }
    return 0; }

terminalCommand(getSignalType){
	if(firstEntry) { firstEntry = 0;
		Serial.println("What kind of turnout is this one?");
		Serial.println("1 = Servo");
		Serial.println("2 = 2 LED");
		Serial.println("3 = 3 LED"); }
	if(serialByte && makeNumber(&signalType,serialByte,1,3,'\n')) {
		return 1; }
	return 0; } 

terminalCommand(getGreenIO) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("What is IO of green LED"); }
	if(serialByte && makeNumber(&greenIO,serialByte,0,63,'\n')) {
		return 1; }
	return 0; }

terminalCommand(getYellowIO) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("What is IO of yellow LED"); }
	if(serialByte && makeNumber(&yellowIO,serialByte,0,63,'\n')) {
		return 1; }
	return 0; }

terminalCommand(getRedIO) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("What is IO of red LED"); }
	if(serialByte && makeNumber(&redIO,serialByte,0,63,'\n')) {
		return 1; }
	return 0; }

terminalCommand(getSwitchType) {
    if(firstEntry) { firstEntry = 0;
        Serial.println("What kind of turnout is this one?");
        Serial.println("1 = Servo");
        Serial.println("2 = Relay");
        Serial.println("3 = 2 Coils"); }
    if(serialByte && makeNumber(&switchType,serialByte,1,3,'\n')) {
        return 1; }
    return 0; } 

terminalCommand(curvedOrStraight) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("Turnout is now set at straight");
		Serial.println("Is it curved?"); }
	switch(serialByte) {
		case 'Y': case 'y': hasLedIO = YES; Serial.println("  YES\r"); break;
		case 'n': case 'N': hasLedIO = NO;  Serial.println("  NO \r");break;
		case '\r': return 1;
		default : return 0; } }

terminalCommand(adjustCurvedPosition) {
	if(firstEntry) { firstEntry = 0;
		curvedPos = 90;
		Serial.println("adjust curved position with '-' and '+', press ENTER when ready"); }
	if( serialByte == '+' ) curvedPos++;
	if( serialByte == '-' ) curvedPos--;
	curvedPos = constrain( curvedPos, 6 , 170);
	Serial.println(curvedPos);Serial.println('\r');
	if( serialByte == '\n' || serialByte == '\r' ) return 1;
	//if(serialByte && makeNumber(&curvedPos,serialByte,0,180,'\n')) return 1; obsolete, only use + and - to adjust position
	unsigned int us = map(curvedPos, 0, 180, 120, 490);
	servoDriver.setPWM(IO, 0, us); // update motor at once
	return 0; }

terminalCommand(adjustStraightPosition) {
	if(firstEntry) { firstEntry = 0;
		straightPos = 90;
		Serial.println("adjust straight position with '-' and '+', press ENTER when ready"); }
	if( serialByte == '+' ) straightPos++;
	if( serialByte == '-' ) straightPos--;
	straightPos = constrain( straightPos, 6 , 170);
	Serial.println(straightPos);Serial.println('\r');
	if( serialByte == '\n' || serialByte == '\r'  ) return 1;
	//if(serialByte && makeNumber(&straightPos,serialByte,0,180,'\n')) return 1;  obsolete, only use + and - to adjust position
	unsigned int us = map(straightPos, 0, 180, 120, 490);
	servoDriver.setPWM(IO, 0, us); // update motor at once
	return 0; }

terminalCommand(getLightCircuitIO) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("On which output is the light circuit relay?"); }
	if(serialByte && makeNumber(&outputIO,serialByte,0,63,'\n')) {
		return 1; }
	return 0;}

terminalCommand(removeDevice) {
    if(firstEntry) { firstEntry = 0;
        Serial.println("enter IO of to be removed device"); }
    if(serialByte && makeNumber(&IO,serialByte,0,255,'\n')) return 1;
    return 0; }

terminalCommand(storeObject) {
	static byte response = 0;
	if(firstEntry) { firstEntry = 0;
		Serial.println("STORE OBJECT? press [Y/n]"); }
	switch(serialByte) {
		case 'Y': case 'y': response = YES; Serial.println("  YES\r"); break;
		case 'n': case 'N': response = NO;  Serial.println("  NO \r");break;
		case '\r': 
		if(response == YES) {
			Serial.println("object SAVED");
			store(); }
		else {
			Serial.println("object DISCARDED"); }
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
	menuF(); 
	if(!subCommand) Serial.write(12); } // take note this is a recursive call, it was the easiest way to print the new texts during state transition

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
	if( debugMode ) Serial.println("IO ");Serial.println(IO);
	unsigned int eeAddress = IO * 8;  // the physical IO is linked with it's position the EEPROM

	if( (type == turnoutObject && switchType == SERVO) 
	||	 type == signalObject  && signalType == SERVO_SIGNAL) {
		eeAddress += (elementAmmount * 8); } // servo motors can have same IO as not servo devices
	EEPROM.put(eeAddress, Array); }



extern void loadEEPROM(byte *nMcp, unsigned int *iodir){ // returns ammount of requied MCP23017 slaves depending on taught in IO
	byte j, i, highestIO = 0, highestTurnoutIO = 0, element;
	unsigned int eeAddress;

	for(element = 0; element < elementAmmount * 2; element++) { // 64x
		byte nMcp = element / 16;
		byte pin = element % 16;
		
		eeAddress = element * 8;
		EEPROM.get(eeAddress, Array); // fetches rail item type, we need inputs, these are 
		if(type != 255) {
			if(element > highestIO) highestIO = element; 			// this line must record the highest IO
																	// as element is linked to IO, element is used
			if(type == decouplerObject && outputIO > highestIO) highestIO = outputIO;

			if(type == signalObject && signalType != SERVO_SIGNAL ) highestIO = outputIO;

			if(type == memoryObject || type == detectorObject || type == decouplerObject) {
				iodir += nMcp; 									// match iodir's address to corresponding mcp23017
				*iodir |= 0x01 << pin; 							// flag pin as input
				iodir -= nMcp; 									// set address back

				//if(type == decouplerObject && outputIO > highestIO) highestIO = outputIO;
				if(hasLedIO && ledIO > highestIO) highestIO = ledIO; } } }// keep the highest IO counter up to date
				// N.B. iodir 0 means that pin is an output, therefor we don't need to alter iodir
				// as it is defaulted to 0

	if( debugMode ) Serial.println("highest IO "); Serial.println(highestIO);
	highestIO = highestIO / 16 + 1;
	if( debugMode ) Serial.println("ammount of mcp devices  ");Serial.println(highestIO);						
	//highestTurnoutIO = highestTurnoutIO / 16 + 1; 	// highest existing IO is 31 which means 2 pca drivers 
	*nMcp = highestIO; 
	if( debugMode ) Serial.println( highestTurnoutIO ); }
