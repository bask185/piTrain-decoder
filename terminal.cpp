#include "terminal.h"
#include "makeNumber.h"
#include <EEPROM.h>






static byte subCommand = 0;
byte Array[8]={255,255,255,255,255,255,255,255}, serialByte ,IO, firstEntry;


#define terminalCommand(x) static byte x##F()
terminalCommand(getType) {
	if(firstEntry) { firstEntry = 0;
	for(byte j=0;j<8;j++) Array[j] = 255;
		Serial.println("Enter type");
		Serial.println("1 = turnout");
		Serial.println("2 = memory");
		Serial.println("3 = detector");
		Serial.println("4 = signal");
		Serial.println("5 = decoupler");
		Serial.println("6 = railCrossing"); }
	switch(serialByte) {
		case '1': type = turnoutObject; return 1;
		case '2': type = memoryObject; return 1;
		case '3': type = detectorObject; return 1;
		case '4': type = signalObject; return 1;
		case '5': type = decouplerObject; return 1;
		//case '6': type = Object; return 1;
		default: return 0; } }

terminalCommand(getID) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("select ID 1-254, press ENTER when ready");}
	if(serialByte && makeNumber(&ID,serialByte,1,255,'\n')) return 1; 
	return 0; }

terminalCommand(getIO) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("select IO 0-63, press ENTER when ready"); }
	if(serialByte && makeNumber(&IO,serialByte,0,63,'\n')) {
		return 1; }
	return 0;}

terminalCommand(getDecouplerIO) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("On which output is the decoupler?"); }
	if(serialByte && makeNumber(&IO,serialByte,0,63,'\n')) {
		return 1; }
	return 0;}

terminalCommand(hasLed) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("Does it have an LED [Y/n]\r\n"); }
	switch(serialByte) {
		case 'Y': case 'y': hasLedIO = YES; Serial.print("  YES\r"); break;
		case 'n': case 'N': hasLedIO = NO; Serial.print("  NO \r");break;
		case '\r': return 1;
		default : return 0; } }

terminalCommand(getLedIO) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("What is IO of LED"); }
	if(serialByte && makeNumber(&IO,serialByte,0,63,'\n')) {
		return 1; }
	return 0; }

terminalCommand(getSwitchType) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("What kind of turnout is this one?");
		Serial.println("1 = Servo");
		Serial.println("2 = Relay");
		Serial.println("3 = 2 Coils"); }
	if(serialByte && makeNumber(&switchType,serialByte,1,3,'\n')) {
		return 1; }
	return 0; } 

terminalCommand(curvedOrStraight) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("Turnout is now set at straight");
		Serial.println("Is it curved?"); }
	switch(serialByte) {
		case 'Y': case 'y': hasLedIO = YES; Serial.print("  YES\r"); break;
		case 'n': case 'N': hasLedIO = NO;  Serial.print("  NO \r");break;
		case '\r': return 1;
		default : return 0; } }

terminalCommand(adjustCurvedPosition) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("adjust curved position, 0-180, press ENTER when ready"); }
	if(serialByte && makeNumber(&curvedPos,serialByte,0,180,'\n')) return 1;
	return 0; }

terminalCommand(adjustStraightPosition) {
	if(firstEntry) { firstEntry = 0;
		Serial.println("adjust straight position, 0-180, press ENTER when ready"); }
	if(serialByte && makeNumber(&straightPos,serialByte,0,180,'\n')) return 1;
	return 0; }

terminalCommand(storeObject) {
	static byte response = 0;
	if(firstEntry) { firstEntry = 0;
		Serial.println("STORE OBJECT? press [Y/n]"); }
	switch(serialByte) {
		case 'Y': case 'y': response = YES; Serial.print("  YES\r"); break;
		case 'n': case 'N': response = NO;  Serial.print("  NO \r");break;
		case '\r': 
		if(response == YES) {
			Serial.println("object SAVED");
			store(); }
		else {
			Serial.println("object DISCARDED"); }
		delay(3000);
		return 1;
		default : return 0; } }
#undef terminalCommand


static void nextCommand(byte x) {
	Serial.write(12);
	serialByte = 0;
	subCommand = x; 
	firstEntry = 1;
	menuF(); 
	if(!subCommand)Serial.write(12);} // take note this is a recursive call, it was the easiest way to print the new texts during state transition

#define terminalCommand(x) break; case x: if(x##F())
extern byte menuF() { // called from main
	switch(subCommand){
		default: {
			nextCommand(getType); }
		
		terminalCommand(getType) {
			nextCommand(getID); }
		
		terminalCommand(getID) 	{
			nextCommand(getIO); }
		
		terminalCommand(getIO) {		
			if		(type == turnoutObject)		nextCommand(getSwitchType);
			else if (type == decouplerObject)	nextCommand(getDecouplerIO);
			else 								nextCommand(hasLed); }

		terminalCommand(curvedOrStraight) {
			nextCommand(storeObject); }

		terminalCommand(hasLed) {	
			if(hasLedIO == YES) nextCommand(getLedIO);	
			else 				nextCommand(storeObject); }

		terminalCommand(getLedIO) {		
			nextCommand(storeObject); }

		terminalCommand(getSwitchType) {
			if(switchType == COILS 
			|| switchType == RELAY) nextCommand(curvedOrStraight);
			else nextCommand(adjustCurvedPosition); }

		terminalCommand(adjustCurvedPosition) {
			nextCommand(adjustStraightPosition); }

		terminalCommand(adjustStraightPosition) {
			nextCommand(storeObject); }
		
		terminalCommand(storeObject) {
			nextCommand(0);
			return 1; }
		break; }
	return 0; }
#undef terminalCommand

static void store() {
	byte j;
	//Serial.print("IO ");Serial.println(IO);
	unsigned int eeAddress = IO * 8;  // the physical IO is linked with it's position the EEPORM
	//Serial.print("eeAddress ");Serial.println(eeAddress);
	for(j = 0; j < 8; j++) { 
		EEPROM.write(eeAddress++, Array[j]); } }


extern void loadEEPROM(byte *nMcp, byte *nservoDrivers, unsigned int *iodir){ // returns ammount of requied MCP23017 slaves depending on taught in IO
	byte j, i, highestIO = 0, highestTurnoutIO = 0, element;
	unsigned int eeAddress = 0;

	for(element = 0; element < elementAmmount; j++) { // 64x
		byte nMcp = element / 16;
		byte pin = element % 16;
		
		eeAddress = element * 8;
		type = EEPROM.read(eeAddress + 1); // fetches rail item type, we need inputs, these are 

		if(type != 255 && element > highestIO) highestIO = element; // this line must record the highest IO
																	// as element is linked to IO, element is used

		if(type == memoryObject || type == detectorObject || type == decouplerObject) {
			iodir += nMcp; 									// match iodir's address to corresponding mcp23017
			*iodir |= 0x01 << pin; 							// flag pin as input
			iodir -= nMcp; 									// set address back

			hasLedIO = EEPROM.read(eeAddress + 2); 			// check if the input element has an LED attached?

			if(hasLedIO) { 						 			// yes
				ledIO = EEPROM.read(eeAddress + 3); 		// fetch the IO
				if(ledIO > highestIO) highestIO = ledIO; } } }// keep the highest IO counter up to date
			// N.B. iodir 0 means that pin is an output, therefor we don't need to alter iodir
			// as it is defaulted to 0

	Serial.print("highest IO "); Serial.println(highestIO);
	highestIO = highestIO / 16 + 1;
	Serial.print("highest IO after calculation  ");Serial.println(highestIO);						
	highestTurnoutIO = highestTurnoutIO / 16 + 1; 	// highest existing IO is 31 which means 2 pca drivers 
	*nMcp = highestIO; }
	//*nservoDrivers= highestTurnoutIO; }