/* 
 * Read pressure data via multiplexer
 * Based on: http://playground.arduino.cc/Learning/4051
 * 
 * @author wbock
 *
 */

 #include <Adafruit_NeoPixel.h>

// Hardware pin numbers
int r0 = 0;      //value of select pin at the multiplexer
int r1 = 0;      //value of select pin at the multiplexer
int r2 = 0;      //value of select pin at the multiplexer
int pinEN = 1; 
int listenPin = 0;

// Temp vars
int value = 0; //Current read value
float valueCol[] = {0.0f, 0.0f, 0.0f, 0.0f}; // Values grouped by column
String values = ""; //Set of all values

// LED-related variables
int LED_COUNT = 8;
int LED_PIN = 10;
// Using leds with WS2811 controller
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
 
void setup(){
  pinMode(pinEN, OUTPUT);    //low to activate mux

  pinMode(2, OUTPUT); // keltainen
  pinMode(3, OUTPUT); // oranssi
  pinMode(4, OUTPUT); // punainen
  pinMode(5, OUTPUT); // ruskea

  pinMode(6, OUTPUT); // r0
  pinMode(7, OUTPUT); // r1
  pinMode(8, OUTPUT); // r2
  digitalWrite(pinEN,LOW);

  digitalWrite(2, LOW); // Row 1
  digitalWrite(3, LOW); // Row 2
  digitalWrite(4, LOW); // Row 3
  digitalWrite(5, LOW); // Row 4

  digitalWrite(6, LOW); // Multiplexer select pin, bit 0
  digitalWrite(7, LOW); // Multiplexer select pin, bit 1
  digitalWrite(8, LOW); // Multiplexer select pin, bit 2

  //strip.setBrightness(55); // Maximum brightness 0..255
  //strip.begin();
  //strip.show(); // Set all leds to 0
  
  Serial.begin(9600);
  analogRead(listenPin);
}

void loop () {
  
  values = ""; //Reset chain of values

  // Loop through columns and rows to get pressure values from grid
  for(int column = 0; column < 4; column++){
    
    setReadChannel(column);
    
    for(int row = 2; row < 5; row++) {
      
      digitalWrite(row, HIGH); // put each row to 5V
      delay(10);
      
      value = analogRead(listenPin); // Read pressure
      valueCol[row-2] = value; // Add to group
      
      if(column < 4 || row < 5) {
        values += " "; // separate values with space
      }
      // Add to chain of values
      values += value;
      digitalWrite(row, LOW); // remove 5V
      delay(10);
    }
    // Apply colours to LEDs
    //setLedColors(column);
  }
  // Send set of 16 values over serial
  Serial.println(values);
  //strip.show();
}

// Set the multiplexer to read from specific channel. Current switches 0...3
void setReadChannel(int chanNr){
  // Convert channel / column number to bit form
  r0 = bitRead(chanNr, 0);
  r1 = bitRead(chanNr, 1); 
  r2 = bitRead(chanNr, 2);

  // Set current output channel / column
  digitalWrite(6, r0);
  digitalWrite(7, r1);
  digitalWrite(8, r2);
}

// Set the LED colors based on pressure values
void setLedColors(int setNr) {
  // setPixelColor(nr, R, G, B, W) RGBW: 0..255, Values: 0..1024
  // LED nr 1, normalize RGBW first
  strip.setPixelColor(setNr * 2, valueCol[0] / 1024 * 255, valueCol[1] / 1024 * 255, valueCol[2] / 1024 * 255, valueCol[3] / 1024 * 255);
  // LED nr 2
  strip.setPixelColor(setNr * 2 + 1, valueCol[0] / 1024 * 255, valueCol[1] / 1024 * 255, valueCol[2] / 1024 * 255, valueCol[3] / 1024 * 255);
}

