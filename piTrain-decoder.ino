/* TODO
*	add separate servo signal and light signal objects
*	implement timing and debounce for reading out inputs
*	make struct for decouplers (maybe also servo type?)
*/

#include "roundRobinTasks.h"
#include "serial.h"
#include "src/basics/timers.h"
#include "src/basics/io.h"

void setup() {

	cli();
	initTimers();
	sei();

	/*N.B. usually the following lines are done from initIO where they are supposed to be. 
	But serial init needs io init and vice versa because of rs485 and debugging. It has become a bit messy unfortunately
	*/
	pinMode(transmissionDir, OUTPUT);
	pinMode(ledPin, OUTPUT);
	
	Serial.println();
	initSerial();
	
	initIO();

	Serial.println("initialized");

	
}

void loop() 
{
	processRoundRobinTasks();
}




