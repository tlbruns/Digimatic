/************************************************************************************************
 * Arduino Digimatic library - Version 1.0
 *
 * Copyright (c) 2014 Trevor Bruns.  All rights reserved.
 *
 * This library communicates with Mititoyo brand calipers via the Digimatic protol
 * Modified code from:  http://www.instructables.com/id/Interfacing-a-Digital-Micrometer-to-a-Microcontrol/?ALLSTEPS
 * 
 * Changelog 1.0:
 * 9/25/14
 *	initial version
 *
 * This code is licensed under a GPLv3 License.
 *
 *
 **********************************************************************************************/
 
#ifndef Digimatic_h
#define Digimatic_h

#include <stdint.h>

// Separate the ARM Due headers we use
#ifdef ARDUINO_SAM_DUE
#include <sam.h>
#include <sam3xa/include/component/component_trng.h>
#endif

// Teensy required headers
#ifdef TEENSYDUINO
#include <util/atomic.h>
#endif

//  Separate AVR headers from ARM headers
#ifdef __AVR__  
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>
#endif

#define byte uint8_t

class Digimatic
{

public:
	//initialize
	Digimatic(byte clk_pin, byte data_pin, byte req_pin);
	// requests and returns current measurement
	double fetch(void);
	bool units_mm(void); // true = mm; false = in
	bool units_in(void);    // true = in; false = mm
	byte decimal_places(void); 	 // digits after the decimal point
	double looptime(void);

protected:
	byte rawdata[14];
	byte req_pin;
	byte clk_pin;
	byte data_pin;
	bool units;
	byte decimal;
	double looptime_ms;
	
  
};

#endif
