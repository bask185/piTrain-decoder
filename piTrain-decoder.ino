/* TODO
*	develop code for the PCA servo drivers
*	add separate servo signal and light signal objects
*	add timer software, may be ISR or smart use of millis()
*	implement timing and debounce for reading out inputs
*	make struct for decouplers (maybe also servo type?)
*/

#include "src/modules/"
#include <EEPROM.h>

unsigned int ioDir[4] = {0,0,0,0};

byte nMcp, nServoDrivers, caseSelector;

void setup() {
	byte j;
	
	Serial.begin(115200);

	loadEEPROM(&nMcp, &nServoDrivers, ioDir);
	initRoundRobinTasks();
	initIO();
	initTimers();
}

void loop() 
{
	processRoundRobinTasks();
}




