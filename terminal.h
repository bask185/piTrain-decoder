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

turnout:
      byte 0 = ID
      byte 1 = type
      byte 2 = switchType
    servo:
      byte 3 = curvedPos
      byte 4 = straightPos
    coil/relay:
      byte 5 = invertedDirection

decoupler:
      byte 0 = ID
      byte 1 = type
      byte 2 = hasLed  // mix has LED with ledIO? and if no IO default it to 255?
      byte 3 = ledIO 
      byte 4 = outputIO */
/*

memory:
      byte 0 = ID
      byte 1 = type
      byte 2 = hasLed
      byte 3 = ledIO */



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
	menu = 'a',
	printEEpromInstruction = 'E',
	whipeEEpromInstruction = 'W',
	instruction4cental = 0xff,

	signalInstruction = 1,
	decouplerInstruction = 2,
	turnoutInstruction = 3,
	detectorInstruction = 4,
	memoryInstruction = 5,
	lightCircuitInstruction = 6,
};

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


extern uint8_t menuF();
extern uint8_t getSubCommand();
static uint8_t getIDF(uint8_t);
static uint8_t adjustLowLimitF(uint8_t);
static uint8_t adjustHighLimitF(uint8_t);
static uint8_t getTypeF(uint8_t); 
static uint8_t getIOF(uint8_t);
static uint8_t getObjectIDF(uint8_t);
static uint8_t storeObjectF(uint8_t);
static void printLines();
static uint8_t getTurnoutIndex(uint8_t _ID);
static void store();
extern void loadEEPROM( uint16_t* );
static uint8_t getIndex();
