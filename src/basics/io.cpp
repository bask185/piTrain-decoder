#include <Arduino.h>
#include "io.h"
extern void initIO(void) {
	pinMode(serialTransmitt, OUTPUT);
}