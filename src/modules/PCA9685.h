#ifndef PCA9685_H
#define PCA9685_H

#include <Arduino.h>

class PCA9685 {
	public:
        PCA9685();
        void init(byte);
        void setServo(byte, byte);
	
	private:
	    byte address; };

#endif