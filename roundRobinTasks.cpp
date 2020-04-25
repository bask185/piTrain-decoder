#include "roundRobinTasks.h"
#include "src/modules/mcp23017.h"
#include "terminal.h"
#include "src/basics/io.h"
#include "src/basics/timers.h"
#include "serial.h"
#include <EEPROM.h>
//#include "src/modules/PCA9685.h"



void sendState(byte state) {
	digitalWrite(transmissionDir, HIGH);
	//PORTB ^= ( 1 << 5 );

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

// byte reverse(byte b) {								// not my invention
// 	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
// 	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
// 	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
// 	return b;}

void readInputs() {
	byte slave, pin, element, state;
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
						if(!debug) 					sendState(state);  
						if(type == decouplerObject)	setOutput(outputIO, state);
						if(hasLedIO == YES) 		setOutput(ledIO, state);
						return; } } } } } }				// if type = 255, the device is not defined

//railCrossing // yet to be made, will prob be a combo of inputs for detection servo's and LEDs
//will need a state machine



void processRoundRobinTasks() {    
	static byte taskCounter = 0;

    switch(++taskCounter) {
        default: taskCounter = 0;

        case 0:
        repeat(&debounceT, 20, readInputs);
        break;

        case 1:
        readSerialBus();
        break;

        case 2:
        flushSerialBus();
        break;
    }
}
