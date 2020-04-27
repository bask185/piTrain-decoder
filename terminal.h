#include <Arduino.h>
//#define memoryAddress 0x100
//#define detectorAddress 0x180

extern byte serialByte;

#define elementAmmount 64

#define YES 1
#define NO 0


/* DATA LAYOUT
* all rail items contain 8 bytes, the EEPRM address is dependend on the given IO
* an empty place in the EEPROM is defined as 255.

all:
	  byte 0 = ID
	  byte 1 = type

turnout:
	  byte 2 = switchType
	servo:
	  byte 3 = curvedPos
	  byte 4 = straightPos
	coil/relay:
	  byte 5 = invertedDirection
/*

decoupler:
	  byte 2 = hasLed  // mix has LED with ledIO? and if no IO default it to 255?
	  byte 3 = ledIO 
	  byte 4 = outputIO */
/*

memory:
	  byte 2 = hasLed
	  byte 3 = ledIO */
/*

detector:
	  byte 2 = hasLed
	  byte 3 = ledIO */
/*

signal:
	  byte 2 = signalType
	  byte 3 = curvedPos
	  byte 4 = straightPos
	  byte 5 = greenIO
	  byte 6 = yellowIO
	  byte 7 = redIO
/*

lightCircuit"
	  byte 2 = hasLed
	  byte 3 = ledIO 
	  byte 4 = outputIO
*/

#define 		ID 					Array[0]
#define 		type 				Array[1]
#define 		switchType 			Array[2]
#define 		detectorIO			Array[2]
#define 		hasLedIO			Array[2]
#define			signalType			Array[2]
#define 		ledIO	 			Array[3]
#define 		servoIO				Array[3]	
#define 		curvedPos 			Array[3]
#define 		outputIO	 		Array[4]
#define 		straightPos 		Array[4]
#define 		invertedDirection 	Array[5]
#define			greenIO				Array[5]
#define			yellowIO			Array[6]
#define			redIO				Array[7]

extern byte Array[8], serialByte ,IO, firstEntry;



enum serialInstructions { // the physical objects
	idle = 255,
	help = 'h',
	menu = 't',
	signalInstruction = 1,
	decouplerInstruction = 2,
	turnoutInstruction = 3,
	detectorInstruction = 4,
	memoryInstruction = 5,
	lightCircuitInstruction = 6,
	printEEpromInstruction = 'E',
	whipeEEpromInstruction = 'W',
	toggleDebugInstruction = 'd' };

enum RAIL_ITEMS { // the physical objects
	turnoutObject = 1,
	memoryObject,
	detectorObject,
	signalObject,
	decouplerObject,
	lightCircuitObject};

enum STATE_MACHINE_STATES { // states of the state machines
	getType = 1,
	getID,
	getIO,
	hasLed,
	getLedIO,
	getSwitchType,
	getDecouplerIO,
	curvedOrStraight,
	adjustCurvedPosition,
	adjustStraightPosition,
	storeObject,
	getSignalType,
	getGreenIO,
	getYellowIO,
	getRedIO,
	getLightCircuitIO,
	removeDevice };

enum TURNOUT_TYPES {
	SERVO = 1,
	RELAY,
	COILS };

enum SIGNAL_TYPES {
	SERVO_SIGNAL = 1,
	_2LED,
	_3LED };

extern byte menuF();
static byte getIDF(byte);
static byte adjustLowLimitF(byte);
static byte adjustHighLimitF(byte);
static byte getTypeF(byte); 
static byte getIOF(byte);
static byte getObjectIDF(byte);
static byte storeObjectF(byte);
static void printLines();
static byte getTurnoutIndex(byte _ID);
static void store();
extern void loadEEPROM(byte*, unsigned int*);
static byte getIndex();
