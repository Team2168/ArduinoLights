#include <Wire.h>
#define I2C_ID 10

byte pattern1 = 0;
byte r1 = 255;
byte g1 = 0;
byte b1 = 0;
byte startLED1 = 0;
byte endLED1 = 19;

byte pattern2 = 0; 
byte r2 = 0;
byte g2 = 0;
byte b2 = 255;
byte startLED2 = 20;
byte endLED2 = 39;

boolean test = true;

void setup() {
  Wire.begin();
}

void loop() {
  
  
  Wire.beginTransmission(I2C_ID);
 
  if(test) {
    Wire.write(pattern1);
    Wire.write(r1);
    Wire.write(g1);
    Wire.write(b1);
    Wire.write(startLED1);
    Wire.write(endLED1);
  }
  else {
    Wire.write(pattern2);
    Wire.write(r2);
    Wire.write(g2);
    Wire.write(b2);
    Wire.write(startLED2);
    Wire.write(endLED2);
  }
  
  delay(2000);
  test = !test;
  Wire.endTransmission();
}


