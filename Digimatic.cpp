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
#include <Arduino.h>
#include <Digimatic.h>
#include <stdint.h>

 Digimatic::Digimatic(uint8_t clk, uint8_t data, uint8_t req)
 {
	clk_pin = clk;
	data_pin = data;
	req_pin = req;
	pinMode(clk_pin, INPUT);
	pinMode(data_pin, INPUT);
	pinMode(req_pin, OUTPUT);
	digitalWrite(clk_pin, HIGH);     // enable internal pull ups
	digitalWrite(data_pin, HIGH);  // enable internal pull ups
	digitalWrite(req_pin,LOW);     // set request at LOW
 }
 
double Digimatic::fetch()
{

	long looptime_l = micros();
	int timeout = 40; // should take <35ms total to fetch data
	// trigger request
	digitalWrite(req_pin,LOW);
	delay(5); 
	digitalWrite(req_pin, HIGH);
	
	unsigned long lastrequest = millis();
	
	for(int i = 0; i < 13; i++ ) {
		int k = 0;
		for (int j = 0; j < 4; j++) {
		  while( digitalRead(clk_pin) == LOW && lastrequest>(millis()-timeout) ) {
		  } // hold until clock is high or timeout
		  while( digitalRead(clk_pin) == HIGH && lastrequest>(millis()-timeout)) { 
		  } // hold until clock is low
		  bitWrite(k, j, (digitalRead(data_pin) & 0x1)); // read data bits, and reverse order )
		}

		// store data
		rawdata[i] = k;    
	}
	
	// convert from raw byte to double
	//  sign = rawdata[4]; 
	//  decimal = rawdata[11];
	//  units = rawdata[12];
	  
	char buf[7];
	for(int lp=0;lp<6;lp++){
		buf[lp]=rawdata[lp+5]+'0';
	}

	long cur_value_int =atol(buf); //assembled measurement, sans decimal point and sign

	// convert to double w/decimal point
	char buf_dec[2];
	buf_dec[0] = rawdata[11]+'0';
	decimal = atol(buf_dec);
	double cur_value = (double)cur_value_int*(double)pow(10,-decimal); 

	// check sign
	char buf_sign[2];
	buf_sign[0] = rawdata[4]+'0';
	int temp = atol(buf_sign);
	if(temp==8)
		cur_value = -cur_value;
		
	// units
	char buf_unit[2];
	buf_unit[0] = rawdata[12]+'0';
	temp = atol(buf_unit);
	units = (bool)temp;
		
	looptime_l = micros() - looptime_l;
	looptime_ms = (double)looptime_l/1000.0;
	
	return cur_value;
}

bool Digimatic::units_mm()
{
	return !units;
}

bool Digimatic::units_in()
{
	return units;
}

byte Digimatic::decimal_places()
{
	return decimal;
}

double Digimatic::looptime()
{
	return looptime_ms;
}