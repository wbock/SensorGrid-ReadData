/* 
 * Read pressure data via multiplexer
 * Based on: http://playground.arduino.cc/Learning/4051
 * 
 * @author wbock
 *
 */

int r0 = 0;      //value of select pin at the multiplexer
int r1 = 0;      //value of select pin at the multiplexer
int r2 = 0;      //value of select pin at the multiplexer
int pinEN = 1; 
int listenPin = 0;


int count = 0;   //which y pin we are selecting
int value = 0;
String values = "";
 
void setup(){
  pinMode(pinEN, OUTPUT);    // EN  

  pinMode(2, OUTPUT); // keltainen
  pinMode(3, OUTPUT); // oranssi
  pinMode(4, OUTPUT); // punainen
  pinMode(5, OUTPUT); // ruskea
  
  pinMode(7, OUTPUT);    // r0
  pinMode(8, OUTPUT);    // r1
  pinMode(9, OUTPUT);    // r2
  digitalWrite(pinEN,LOW);

  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);

  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  
  Serial.begin(9600);
  analogRead(listenPin);
}

void loop () {
  values = "";
  for(int column = 0; column < 4; column++){
    setReadChannel(column);
    for(int row = 2; row < 6; row++) {
      digitalWrite(row, HIGH);
      delay(15);
      value = analogRead(listenPin);
      values += value;
      if(column < 4 || row < 5) {
        values += " ";
      }
      digitalWrite(row, LOW);
      delay(15);
    }
  }
  Serial.println(values);
}

// switches are from 0 to 3
int setReadChannel(int chanNr){
  r0 = bitRead(chanNr, 0);    // use this with arduino 0013 (and newer versions)     
  r1 = bitRead(chanNr, 1);    // use this with arduino 0013 (and newer versions)     
  r2 = bitRead(chanNr, 2);    // use this with arduino 0013 (and newer versions) 
    
  digitalWrite(7, r0);
  digitalWrite(8, r1);
  digitalWrite(9, r2);
}

