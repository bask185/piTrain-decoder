/* TODO
*	develop code for the PCA servo drivers
*	add separate servo signal and light signal objects
*	add timer software, may be ISR or smart use of millis()
*	implement timing and debounce for reading out inputs
*	make struct for decouplers (maybe also servo type?)
*/



//#include <I2Cdev.h>
#include "mcp23017.h"
//#include <PCA9685.h>
#include <Wire.h> 
#include "terminal.h"
#include <EEPROM.h>


byte nMcp, nServoDrivers;

//ServoDriver servo;
Mcp23017 mcp[] {	// 2 max
	Mcp23017(),
	Mcp23017(),
	Mcp23017(),
	Mcp23017(),
	Mcp23017(),
	Mcp23017(),
	Mcp23017(),
	Mcp23017() } ;

// PCA9685 pca[] {	// 2 max
// 	PCA9685(),
// 	PCA9685() } ;

char command = 0, mcpAmmount;

byte debug = false;


void setup() {
	byte j;
	Wire.begin();
	Serial.begin(115200);

	// EEPROM.write(377, 255);

	// while(1);
	Serial.println("program booting");
	//printEEprom();
	// fetch data from EEPROM and commit them to the struct types
	unsigned int ioDir[8] = {0,0,0,0,0,0,0,0};						// create local array for IOdir registers of 8 mcp devices
	delay(2000);
	loadEEPROM(&nMcp, &nServoDrivers, ioDir); 	// determen what the highest IO is of all object, to determen the ammount of Mcp23017 devices and fill structs
	delay(2000);
	Serial.print(nMcp);Serial.println(" mcp slaves");
	Serial.print(nServoDrivers);Serial.println(" servo drivers");
	Serial.println(ioDir[0],HEX);
	Serial.println(ioDir[1],HEX);
	Serial.println(ioDir[2],BIN);
	// Serial.println(ioDir[3],BIN);
	// Serial.println(ioDir[4],BIN);
	// Serial.println(ioDir[5],BIN);
	// Serial.println(ioDir[6],BIN);
	// Serial.println(ioDir[7],BIN);

	//ioDirPtr = &ioDir;
	//ioDir[0] = 0x00ff;
	//ioDir[1] = 0x00ff;
	//ioDir[2] = 0xffff;

	for(j=0;j<nMcp;j++) {
		Serial.print("slave #");Serial.print(j);Serial.print(" has address #");Serial.print(mcpBaseAddress + j);
		Serial.print(" and it's IO state = "); Serial.println(ioDir[j],BIN);
		mcp[j].init(mcpBaseAddress + j, ioDir[j]); }

	Serial.println("starting program, press 'h' for help");
	command = 0; }

void loop() {
	delay(20);				    // this is just temporarily
	
	readInputs();
	readSerialBus();
	//menu();
}
																//  V debug thing ... //

#define serialCommand(x) byte x##F()
serialCommand(help) {
	Serial.println("help menu");
	Serial.println(	"help = 'h'\r\n"
					"add item = 't'\r\n"
					"signalInstruction = 'S'\r\n"
					"decouplerInstruction = 'D'\r\n"
					"turnoutInstruction = 'T'\r\n"
					"detectorInstruction = 'd'\r\n"
					"memoryInstruction = 'M'\r\n"
					"printEEpromInstruction = 'E'\r\n"
					"whipeEEpromInstruction = 'W'\r\n"
					"toggleDebug = 'd'");
	return 1; }

serialCommand(signalInstruction) {
	static byte signalID, caseSelector, element;
	unsigned int eeAddres;
	switch(caseSelector++) {
		case 0: signalID = serialByte; return 0;
		case 1:		
		for(element = 0; element < elementAmmount; element++) {
			unsigned int eeAddress = element * 8;
			//byte IO = element;
			//byte ID = EEPROM.read(eeAddress);
			//byte type = EEPROM.read(eeAddres + 1);
			//byte state = serialByte;
			if(type == signalObject/* && ID == switchID*/) { // if matching ID is found
				/*setSwitch(IO, state);*/ } } 
		return 1; } }

serialCommand(turnoutInstruction) {
	static byte switchID, caseSelector, element;
	unsigned int eeAddres;
	switch(caseSelector++) {
		case 0: switchID = serialByte; return 0;
		case 1:		
			for(element = 0; element < elementAmmount; element++) {
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
		if(!command) {
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

// end of serial functions 

void readInputs() {
	byte slave, pin, element, /*IO,*/ state;
	for(slave=0; slave<nMcp; slave++){  										// for all MCP23017 devices
		static unsigned int inputPrev[8] = {0,0,0,0,0,0,0,0}, input = 0;
		input = mcp[slave].getInput(portB) | (mcp[slave].getInput(portA) << 8);	// read both I/O ports
		for(pin=0;pin<16;pin++) {												// for all 16 I/O pins
			if((input & (1 << pin)) != (inputPrev[slave] & (1 << pin))) {		// if an input (detector or memory) has changed...
				inputPrev[slave] = input;
				if(input & (1<<pin))	state = 0;					  			// store the state of the changed I/O (not pressed = HIGH)
				else					state = 1;
				IO = pin + slave * 16;											// calculate which IO has changed
				for(element=0; element<elementAmmount; element++){				// for every rail item check if the changed IO matches
					unsigned int eeAddress = IO * 8 ;
					EEPROM.get(eeAddress, Array);								// fetch ID from EEPROM
					if(type != 255) {
						if(!debug) sendState(state);  
						if(hasLedIO == YES) setLED(state);
						return; } } } } } }				// if type = 255, the device is not defined

//railCrossing // yet to be made, will prob be a combo of inputs for detection servo's and LEDs
//will need a state machine

void setLED(byte state) {	//  ID prev is to be cleared, ID is to be set
	static byte pinPrev, xMcpPrev;
	byte element, xMcp, pin, LEDport, input;
	//unsigned int input;
	xMcp = ledIO / 16;
	pin = ledIO % 16; // < works
	if(pin < 8) {
		LEDport = portB; }
	else {
		LEDport = portA;
		pin -= 8; }

	input = mcp[xMcp].getInput(LEDport);
	if(state)	input |=  (1 << pin);
	else		input &= ~(1 << pin);

	mcp[xMcp].setPort(LEDport, input); }


void sendState(byte state) {
	switch(type) {
		case memoryObject: if(state)	Serial.write(memoryInstruction);	break; // only true states for memories
		case detectorObject: 			Serial.write(detectorInstruction);	break;
		case decouplerObject: 			Serial.write(decouplerInstruction);	break; }

	if(type == memoryObject) {
		if(!state) return; 		// memories' low states are irrelevant to everything else, so we return
		else /*clrMemoryLeds()*/; } // only 1 memory LED is to be set at the time

	//if(hasLedIO == YES) setLED(ledIO, state); // if there is an LED to be set/cleared, make it so!
	Serial.write(ID);
	Serial.write(state); }

byte reverse(byte b) {								// not my invention
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;}
