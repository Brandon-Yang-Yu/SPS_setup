//-------------------------------------------------------------------------------------
// HX711_ADC.h
// Arduino master library for HX711 24-Bit Analog-to-Digital Converter for Weigh Scales
// Tested with MCU: Arduino Nano
//-------------------------------------------------------------------------------------
/* This is an example sketch on how to find the correct calibration factor for your HX711:

Power up the scale and open Arduino serial terminal

After stabilizing and tare is complete, put a known weight on the load cell

Observe values on a serial terminal

Adjust the calibration factor until output value is same as your known weight:

Sending 'l' from the serial terminal decrease factor by 1.0

Sending 'L' from the serial terminal decrease factor by 10.0

Sending 'h' from the serial terminal increase factor by 1.0

Sending 'H' from the serial terminal increase factor by 10.0

Sending 't' from the serial terminal call tare function

Observe and note the value of the new calibration factor

Use this new calibration factor in your sketch*/

#include <HX711_ADC.h>

const int LoadCell_1_dout = 10; //mcu > HX711 dout pin, must be external interrupt capable!
const int LoadCell_1_sck = 11; //mcu > HX711 sck pin

// === Definitions =================================================
//HX711 constructor (dout pin, sck pin)
HX711_ADC LoadCell_1(LoadCell_1_dout, LoadCell_1_sck);

long t;

// === Setup =======================================================

void setup() {
  
  Serial.begin(9600);

  // Set pins 
  LoadCell_1.begin();
  LoadCell_1.setGain(64);
  long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time

  LoadCell_1.start(stabilisingtime);

  int calibration_factor = 17140;
  LoadCell_1.setCalFactor(calibration_factor); // user set calibration factor (float)

}

// === Main loop ===================================================

void loop() {

//update() should be called at least as often as HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS//longer delay in scetch will reduce effective sample rate (be carefull with delay() in loop)
  LoadCell_1.update();

const int serialPrintInterval = 500;
  
  if (millis() > t + serialPrintInterval) {
  //get smoothed value from data set + current calibration factor
  float nw = LoadCell_1.getData();
  Serial.println(nw);
  t=millis();

}

//receive from serial terminal
if (Serial.available() > 0) {
  float i = 0;
  char inByte = Serial.read();
  if (inByte == 'l') i = -1.0;
  else if (inByte == 'L') i = -10.0;
  else if (inByte == 'h') i = 1.0;
  else if (inByte == 'H') i = 10.0;
  else if (inByte == 't'){
    LoadCell_1.tare();

} 

if (inByte != 't') {   

float v = LoadCell_1.getCalFactor() + i;     

LoadCell_1.setCalFactor(v);

 }

}

//check if last tare operation is complete
if (LoadCell_1.getTareStatus() == true) 
{Serial.println("Load_Cells Tare complete");

}

}
