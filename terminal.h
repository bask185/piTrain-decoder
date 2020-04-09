#include <Arduino.h>
//#define memoryAddress 0x100
//#define detectorAddress 0x180

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
      byte 0 = ID
      byte 1 = type
      byte 2 = hasLed
      byte 3 = ledIO */

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



enum serialInstructions {
    idle = 255,
    help = 'h',
    menu = 't',
    signalInstruction = 1,
    decouplerInstruction = 2,
    turnoutInstruction = 3,
    detectorInstruction = 4,
    memoryInstruction = 5,
    printEEpromInstruction = 'E',
    whipeEEpromInstruction = 'W',
    toggleDebugInstruction = 'd' };



enum railItems {
    turnoutObject = 1,
    memoryObject,
    detectorObject,
    signalObject,
    decouplerObject,
    railCrossingObject};

enum commands {
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
    removeDevice };

enum turnoutTypes {
    SERVO = 1,
    RELAY,
    COILS };

enum signalTypes {
    ARM_SIGNAL,
    ARM_SIGNAL_AND_LIGHT,
    _2LIGHTS,
    _3LIGHTS};

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
extern void loadEEPROM(byte*, byte*, unsigned int*);
static byte getIndex();
