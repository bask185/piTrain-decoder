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
	initSerial();
	initIO();
	initTimers();
}

void loop() 
{
	processRoundRobinTasks();
}




