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
	pinMode( transmissionDir, OUTPUT );
	pinMode( ledPin, OUTPUT );
    pinMode( debugPin, INPUT_PULLUP );

	initSerial();

	if( digitalRead( debugPin ) ){
		debugMode = 0 ;
		Serial.println(F("booting in normal mode"));
		delay(1000);
	} else {
		debugMode =  1 ; // jumper is applied
		digitalWrite( transmissionDir , HIGH ) ; // when using the USB cable, this line must be high or the communication to PC will fail
		digitalWrite( ledPin, HIGH );
		Serial.println(F("booting in config mode"));
		delay(1000);
	}

	initIO();
}

void loop() 
{
	processRoundRobinTasks();
}




