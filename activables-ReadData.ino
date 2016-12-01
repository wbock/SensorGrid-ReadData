/*
   Read pressure data via shift register & angle from gyro
   Based on: https://www.arduino.cc/en/Tutorial/ShftOut11
        and: https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/wiring-and-test


   @author wbock

*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 10;
////Pin connected to DS of 74HC595
int dataPin = 9;

// Analog pins for reading input
int row1 = 0; // lower row (closest to output ports)
int row2 = 1; // middle row
int row3 = 2; // upper row

int chanels[] = {2, 4, 8, 16, 32, 64};

// Temp vars
int value = 0; //Current read value
String values = ""; //Set of all values

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  Serial.begin(9600);
}

void loop () {

  for (int i = 0; i < 3; i++) {

    // First value = row number
    values = "";
    values += i;
    values += " ";

    for (int ii = 0; ii < 6; ii++) {
      // take the latchPin low so
      // the LEDs don't change while you're sending in bits:
      digitalWrite(latchPin, LOW);
      
      // shift out the bits:
      shiftOut(dataPin, clockPin, MSBFIRST, chanels[ii]);

      //take the latch pin high so the LEDs will light up:
      digitalWrite(latchPin, HIGH);
      // pause before next value:
      delay(20);
      values += analogRead(i);

      // Spacing between values
      if(ii < 5) {
        values +=  " ";
      }
    }
    // Send values row-by-row
    Serial.println(values);
  }
}

