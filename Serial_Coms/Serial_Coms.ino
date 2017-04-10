#include <Wire.h>
#define I2C_ID 10
#define RANGECOUNT 1

byte data[RANGECOUNT][5];

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  Wire.beginTransmission(I2C_ID);
  for(int i=0; i<RANGECOUNT; i++){
    data[i][0] = 100; //Red
    data[i][1] = 0;  //Green
    data[i][2] = 0;  //Blue
    data[i][3] = 4;  //Pattern
    data[i][4] = 2;  //PatternData(if neccessary)
    for(int x=0; x<4; i++){
      Wire.write(data[i][x]);
      Serial.print(i);
    }
  }
  
  
  //delay(500);
  

}


