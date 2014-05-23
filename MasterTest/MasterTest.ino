// Modified from example at http://arduino.cc/en/Tutorial/MasterWriter

#include <Wire.h>

#define SLAVE_ID 4

int c = 0;
int i = 0;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

byte x = 0;

void loop()
{
  //Type in the terminal, each character will increment the
  //  counter by one which should step the slave through its
  //  available patterns one by one.
  if(Serial.available() > 0) {
    //read incoming byte, drop it on the floor
    c = Serial.read();
   
    Wire.beginTransmission(SLAVE_ID);
    Wire.write(i%16);
    Wire.endTransmission();
    
    Serial.print(i%16, HEX);
    
    i = i++;
  }
  
  delay(100);
}
