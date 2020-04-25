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

	
	initIO();


	initSerial();
}

void loop() 
{
	processRoundRobinTasks();
}




