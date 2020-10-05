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
/*

detector:
<<<<<<< HEAD
	  byte 2 = hasLed
	  byte 3 = ledIO */
/*
=======
      byte 0 = ID
      byte 1 = type
      byte 2 = hasLed
	  byte 7 = redIO
/*
<<<<<<< HEAD

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

/*
railCrossing:  // does not yet exists. As more than 1 of these objects may exist, 
               // a class is required for this object. each crossing also need their own state machines in order to operate
      byte 0 = ID            // rail crossing in general don't need an ID, but it might be usefull to tranmit it's state
      byte 1 = trackAmount
      byte 2 = detectorIO    // 2 or 4 following IO depending on track ammount
      byte 3 = servoIO        // used 2 following IO pins
      byte 4 = servo1low
      byte 5 = servo1high
      byte 6 = servo2low
      byte 7 = servo2high
*/
#define         ID                   Array[0]
#define         type                 Array[1]
//#define         trackAmount            Array[1]
#define         switchType          Array[2]
#define         detectorIO          Array[2]
#define         hasLedIO            Array[2]
#define         ledIO               Array[3]
#define         servoIO             Array[3]    
#define         curvedPos           Array[3]
#define         outputIO            Array[4]
#define         servo1low           Array[4]
#define         straightPos         Array[4]
#define         invertedDirection   Array[5]
#define         servo1high          Array[5]
#define         servo2low           Array[6]
#define         servo2high          Array[7]
extern byte Array[8], serialByte ,IO, firstEntry;



enum serialInstructions { // the physical objects
	idle = 255,
	help = 'h',
	menu = 'a', // a for "add"
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
extern byte getSubCommand();
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
