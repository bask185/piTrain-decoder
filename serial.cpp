#include "serial.h"
#include "terminal.h"
#include "src/basics/io.h"
#include <EEPROM.h>

byte command = 0, caseSelector, debug;

#define serialCommand(x) byte x##F()
serialCommand(help) {
	Serial.write(12);
	Serial.println("help menu\r\n"
					"h = help\r\n"
					"t = add item\r\n"
					"S = signalInstruction\r\n"
					"D = decouplerInstruction\r\n"
					"T = turnoutInstruction\r\n"
					"d = detectorInstruction\r\n"
					"M = memoryInstruction\r\n"
					"E = printEEpromInstruction\r\n"
					"W = whipeEEpromInstruction\r\n"
					"d = toggleDebug");
	return 1; }

serialCommand(signalInstruction) {
	static byte signalID, element;
	unsigned int eeAddres;
	switch(caseSelector) {
		case 0: return 0;
		case 1: signalID = serialByte; return 0;
		case 2:		
		for(element = 0; element < elementAmmount*2; element++) {
			unsigned int eeAddress = element * 8;

			if(type == signalObject && ID == signalID) { // if a switchType is found and it's ID matches
				switch(switchType) {
					case SERVO :/*setSwitch(IO, state);*/ break; } } }
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

				if(switchType == SERVO) {
					setServo(ID, state);} 

				else if(switchType == RELAY ) {
					setOutput(IO, state); } 
					
				else if( switchType == COILS ) {
					setOutput(IO,    state);
					setOutput(IO+1, !state); } } }
		return 1; } }


serialCommand(printEEpromInstruction){
	for(unsigned int j=0;j<1024;j++) {
		byte b;
		b = EEPROM.read(j);
		Serial.print(j); Serial.print(" ");Serial.println(b); } 
	return 1; }

serialCommand(whipeEEpromInstruction) {
	if(serialByte != 'y' && serialByte != 'n') {
		Serial.println("this will whipe the eeprom, continue? [y/n]");
		return 0; }
	else if(serialByte == 'y') {
		Serial.println("WHIPING EEPROM, STANDBYE...");
		for(unsigned int j=0;j<1024;j++) {
			EEPROM.write(j,255); } 
		Serial.println("EEPROM WHIPED!"	);
		return 1; }
	else if(serialByte == 'n') {
		Serial.println("ABORTED");
		return 1; } }

serialCommand(toggleDebugInstruction){
	debug ^= 1;
	if(debug) {
		Serial.println("debug on"); }
	else{
		Serial.println("debug off"); } 
	return 1; }
	

#undef serialCommand

#define serialCommand(x) case x:  if(x##F()) {command=0;firstEntry=1;helpF();} break;
void readSerialBus() {
	if(Serial.available() > 0 ){
		
		serialByte = Serial.read(); 
		//Serial.println(serialByte);
		caseSelector++;
		if(!command) {
			caseSelector = 0;
			command = serialByte; }			// if not yet a command was received, fetch new command
		switch(command) {
			default: Serial.println("command not recognized, press 'h' for help menu");command=0; break;  // if command is not recognized print this text
			serialCommand(help);
			serialCommand(menu); // <-- in terminal.cpp
			serialCommand(signalInstruction);
			serialCommand(turnoutInstruction);
			serialCommand(printEEpromInstruction);
			serialCommand(whipeEEpromInstruction);
			serialCommand(toggleDebugInstruction); } }
	else serialByte = 0; }
#undef serialCommand


void flushSerialBus() {
	if(!Serial.availableForWrite()) { // if all bytes are transmitted
		digitalWrite(transmissionDir, LOW);
	}
}

void initSerial(){
    Serial.begin(115200);
	Serial.println("piTrainDecoder");
	helpF();
}