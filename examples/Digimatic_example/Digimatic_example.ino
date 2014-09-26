/************************************************************************************************
 * Arduino Digimatic library - Version 1.0
 *
 * Copyright (c) 2014 Trevor Bruns.  All rights reserved.
 *
 * This uses the Digimatic library to communicate with Mititoyo brand calipers
 * Modified code from:  http://www.instructables.com/id/Interfacing-a-Digital-Micrometer-to-a-Microcontrol/?ALLSTEPS
 *
 * This code is licensed under a GPLv3 License.
 *
 *
 **********************************************************************************************/

#include <Digimatic.h>

// define pins
uint8_t req_pin = 5;
uint8_t data_pin = 2;
uint8_t clk_pin = 3;

// initialize Digimatic
Digimatic caliper = Digimatic(clk_pin, data_pin, req_pin);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  // request current measurement
  double curr_measurement = caliper.fetch();
  Serial.print(curr_measurement,caliper.decimal_places());
  
  // display units
  if(caliper.units_mm())
    Serial.print(" mm");
  else if(caliper.units_in())
    Serial.print(" in");
    
  Serial.print("; looptime = ");
  Serial.print(caliper.looptime());
  Serial.println("ms");
    
   
}
  
