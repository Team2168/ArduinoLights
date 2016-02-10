#include <Wire.h>
#define I2C_ID 10

void setup() {
  Wire.begin();
}

void loop() {
  byte pattern = 0;
  byte red = 255;
  byte green = 0;
  byte blue = 0;
  byte startLED = 0;
  byte endLED = 19;
  
  Wire.beginTransmission(I2C_ID);
  Wire.write(pattern);
  Wire.write(red);
  Wire.write(green);
  Wire.write(blue);
  Wire.write(startLED);
  Wire.write(endLED);
  Wire.endTransmission();
}
