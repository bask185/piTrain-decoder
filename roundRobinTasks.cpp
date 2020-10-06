#include "roundRobinTasks.h"
#include "src/modules/mcp23017.h"
#include "terminal.h"
#include "src/basics/io.h"
#include "src/basics/timers.h"
#include "serial.h"
#include <EEPROM.h>



void sendState(byte state) {
	beginTransmission();

	Serial.write( instruction4cental ) ; // flag message that it is ment for the central and not other slave

	switch(type) {
		case memoryObject: if(state)	Serial.write(memoryInstruction);	break; // only true states for memories
		case detectorObject: 			Serial.write(detectorInstruction);	break;
		case decouplerObject: 			Serial.write(decouplerInstruction);	break; }

	if(type == memoryObject) {
		if(!state) return; 		// memories' low states are irrelevant to everything else, so we return
		else /*clrMemoryLeds()*/; } // only 1 memory LED is to be set at the time

	Serial.write(ID);
	Serial.write(state);

	Serial.write( instruction4cental ) ; // mark end of message

	if(hasLedIO == YES) setLED(ledIO, state); // if there is an LED to be set/cleared, make it so!
}


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
						if(!debugMode) 				sendState(state);  
						if(type == decouplerObject)	setOutput(outputIO, state);
						if(hasLedIO == YES) 		setOutput(ledIO, state);
						if(type == signalObject) {
							switch (signalType) {
							case SERVO_SIGNAL: 
								setServo(IO, state);
								break;
							case _2LED: 
								setOutput(greenIO,   state);
								setOutput(yellowIO, ~state);
								break;
							case _3LED: break; // no solution for the yellow LED exist yet
							}
						}
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
