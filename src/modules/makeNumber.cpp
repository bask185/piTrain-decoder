#include "makeNumber.h"
#include "../basics/io.h"
#include "../basics/timers.h"

static unsigned int numberTmp;
static byte *numberToReturn;
static byte firstEntry = 1;

// makeNumber, function used to assemble an integer number, function returns 1 when provided end character is received
byte makeNumber(byte *adress, byte serialByte, byte min, byte max, char end) {
	numberToReturn = adress;					// numberToReturn is global pointer which points to the given adress
	
	if(firstEntry) {							// first entry
		firstEntry = 0;
		numberTmp = *adress; }

	if(serialByte == '-' || serialByte == '+') { // dec or inc
		if(serialByte == '-')  numberTmp--;		// dec
		else				   numberTmp++; }	// inc

	if(serialByte >= '0' && serialByte <= '9') {// after 0-9 more bytes will follow to alter this number.
		numberTmp *= 10;						// multiply the number by 10
		numberTmp += (serialByte-'0'); }  		// add the new number

	if(serialByte == 127) {						//backspace in putty  devide number by 10, effecively removes the previously added number
		numberTmp /= 10; }

	if(serialByte == '\r') {						// if end character ...
		firstEntry = 1; } 						// ... signal we are ready

	if(numberTmp >= 1000) numberTmp = serialByte - '0';
	*numberToReturn = numberTmp;				// return the number
	Serial.print('\r');
	Serial.print(numberTmp);

	if(firstEntry) numberTmp = constrain(numberTmp, min, max);
	return firstEntry; }

