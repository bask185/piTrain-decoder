#include "serial.h"

#define serialCommand(x) byte x##F()
serialCommand(help) {
	Serial.println("help menu");
	Serial.println(	"h = help\r\n"
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
		case 0: signalID = serialByte; return 0;
		case 1:		
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
		case 0: switchID = serialByte; return 0;
		case 1:		
			for(element = 0; element < elementAmmount*2; element++) {
				unsigned int eeAddress = element * 8;
				byte IO = element;
				//byte ID = EEPROM.read(eeAddress);
				//byte type = EEPROM.read(eeAddres + 1);
				byte state = serialByte;
				if(type == turnoutObject/* && ID == switchID*/) { // if matching ID is found
					/*setSwitch(IO, state);*/ } } } }


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

#define serialCommand(x) case x:  if(x##F()) {command=0;} break;
void readSerialBus() {
	if(Serial.available()>0){
		serialByte = Serial.read(); 
		caseSelector++;
		if(!command) {
			caseSelector = 0;
			Serial.write(12);
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
		digitalWrite(serialTransmitt, LOW);
	}
}