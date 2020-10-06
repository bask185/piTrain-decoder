#include "serial.h"
#include "terminal.h"
#include "src/basics/io.h"
#include "src/basics/timers.h"
#include <EEPROM.h>

byte command = 0, caseSelector;

#define serialCommand(x) byte x##F()
serialCommand(help) {
	beginTransmission();
	Serial.write(12);		// delete putty screen
	Serial.println(F("help menu\r\n"
					"h = help\r\n"
					"a = add item\r\n"
					"S = signalInstruction\r\n"
					"D = decouplerInstruction\r\n"
					"T = turnoutInstruction\r\n"
					"d = detectorInstruction\r\n"
					"M = memoryInstruction\r\n"
					"E = printEEpromInstruction\r\n"
					"W = whipeEEpromInstruction\r\n"
					));
	return 1; }


serialCommand(signalInstruction) {
	static byte signalID, element;
	unsigned int eeAddres;
	switch(caseSelector) {
		case 0: return 0;	// discard first byte
		case 1: signalID = serialByte; return 0;
		case 2:		
		for(element = 0; element < elementAmmount*2; element++) {
			unsigned int eeAddress = element * 8;
			EEPROM.get(eeAddress, Array);

			if(type == signalObject && ID == signalID) { // if a switchType is found and it's ID matches
				byte state = serialByte;

				switch(signalType) {
					case SERVO_SIGNAL : setServo(      IO,  state);		  break; 
					case _2LED: 		setOutput(greenIO,  state);
										setOutput(  redIO, ~state);   break;
					case _3LED: break; } } }
		return 1; } }


serialCommand(turnoutInstruction) {
	static byte switchID, element;
	unsigned int eeAddres;
	switch(caseSelector) {
		case 0: return 0; // ignore first byte
		case 1: switchID = serialByte; return 0;
		case 2:		
		for( element = 0; element < elementAmmount * 2; element++ ) {
			unsigned int eeAddress = element * 8;
			EEPROM.get(eeAddress, Array);

			if(ID == switchID && type == turnoutObject) {
				byte state = serialByte;
				switch(switchType) {
					case SERVO: setServo(ID,  state);    break;
					case RELAY: setOutput(IO, state);    break;
					case COILS: setOutput(IO, state);   
								setOutput(IO+1, !state); break; } } }
		return 1; } }


serialCommand(printEEpromInstruction){
	for(unsigned int j=0;j<1024;j++) {
		beginTransmission();
		byte b;
		b = EEPROM.read(j);
		Serial.print(j); Serial.print(F(" "));Serial.println(b); } 
	return 1; }

serialCommand(whipeEEpromInstruction) {
	beginTransmission();
	if(serialByte != 'y' && serialByte != 'n') {
		Serial.println(F("this will whipe the eeprom, continue? [y/n]"));
		return 0; }
	else if(serialByte == 'y') {
		Serial.println(F("WHIPING EEPROM, STANDBYE..."));
		for(unsigned int j=0;j<1024;j++) {
			EEPROM.write(j,255); } 
		Serial.println(F("EEPROM WHIPED!"));
		return 1; }
	else if(serialByte == 'n') {
		Serial.println(F("ABORTED"));
		return 1; } }

serialCommand( instruction4cental ) {
	switch(caseSelector) {
		case 0: return 0;	// discard first byte
		default:
		if( serialByte == instruction4cental ) return 1 ; 
		return 0 ;
	}

}

// serialCommand(toggleDebugInstruction){ OBSOLETE
// 	beginTransmission();
// 	debug ^= 1;
// 	if(debug) {
// 		Serial.println(F("debug on")); }
// 	else {
// 		Serial.println(F("debug off")); } 
// 	return 1; }
	

#undef serialCommand 

#define discard serialCommand // same thing really..
#define serialCommand1(x) case x: if(x##F()) { command=0; firstEntry=1;} break;



/* serial protocol
OUTGOING COMMANDS TO CENTRAL: // Are also received by other slave and are to be ignored by them
- 0xFF + 5 + ID + state + 0xFF // memoryInstruction
- 0xFF + 4 + ID + state + 0xFF // detectorInstruction
- 0xFF + 2 + ID + state + 0xFF // decouplerInstruction

INCOMMING COMMANDS FROM CENTRAL:

signalInstruction
turnoutInstruction



*/

void readSerialBus() {
	if(Serial.available() > 0 ){
		serialByte = Serial.read(); 

		caseSelector++;
		if(!command) {
			caseSelector = 0;
			command = serialByte; }			// if not yet a command was received, fetch new command

		switch(command) {
			default: 
			beginTransmission();
			if( debugMode) Serial.println(F("command not recognized, press 'h' for help menu"));
			command=0; 
			break;  // if command is not recognized print this text

			discard( instruction4cental ) ;	// may be transmitted by other slaves
			
			serialCommand(help); 
			serialCommand(menu); // <-- in terminal.cpp
			serialCommand(signalInstruction);
			serialCommand(turnoutInstruction);
			serialCommand(printEEpromInstruction);
			serialCommand(whipeEEpromInstruction); } }
			//serialCommand2(toggleDebugInstruction); } } OBSOLETE
	else serialByte = 0; }
#undef serialCommand


void flushSerialBus() {
	if( debugMode == 0 ) {
		Serial.flush();
		digitalWrite(transmissionDir, LOW); // go to receiving mode 
		digitalWrite(ledPin, LOW ); 
	}
}

void initSerial(){
    Serial.begin(9600);
	if( debugMode  ) helpF();
}