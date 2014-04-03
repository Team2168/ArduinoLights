#include <SPI.h>
#include <LPD8806.h>

// Wiring
int inputPin1 = 2;
int inputPin2 = 3;
int inputPin3 = 4;
int inputPin4 = 5;

//Strip Length Data
int IntakeStripLength = 18;

int i = 0,
    j = 0;

//Set strip to use SPI. On an UNO pin 11 is MISO and 13 is SCK.
//Standard ICSP Pinout is as follows:
// 1 - MISO  |o|o| +Vcc - 2
// 3 - SCK   |o|o| MOSI - 4
// 5 - Reset |o|o| GND  - 6
//           ICSP
LPD8806 strip(IntakeStripLength);

void TuskRetractPosition();
void TuskIntermediatePosition();
void TuskExtendedPosition();
void HotGoalLeft();
void HotGoalRight();
void UnknownTarget();
void TurnOffLights();

int retValue = 0;
int lastretValue = 0;
    
void setup() {
  strip.begin();
  
  //light all pixels on the strip to verify they're working
  for(int q = 0; q < strip.numPixels(); q++) {
    strip.setPixelColor(q, strip.Color(70,70,70));
  }
  strip.show();
  delay(1500);
   
  //Turn the LEDs back off
  TurnOffLights();
  strip.show();

  // Note, pins 10 - 13 are in use by the SPI library. 
  pinMode(inputPin1, INPUT); 
  pinMode(inputPin2, INPUT); 
  pinMode(inputPin3, INPUT);  
  pinMode(inputPin4, INPUT);  

  Serial.begin(9600); // Serial Monitor
}

void loop() {
  //=======================================
  //          AUTO MODE               Bits
  //=======================================            
  //          HotGoalLeft           // xx01
  //          HotGoalRight          // xx10
  //          UnknownTarget         // xx11
  //---------------------------------------
  //          Not in Auto**         // xx00 check teleop
  //======================================= 
  //          TELEOP MODE             Bits
  //=======================================
  //          Off                   // 0000 
  //          TuskExtendedPosition  // 1000
  //          TuskIntermediate      // 0100
  //          TuskRetractPosition   // 1100
  //======================================= 
  //                 PIN MAP
  //     TELEOP ->  X X | X X  <- AUTO
  // INPUT PINS ->  4 3   2 1
  //=======================================
  
  retValue = 0;
  //Read signals for cRIO
  if(digitalRead(inputPin1) == HIGH) { // if pin is high, return value is 1 (0001)
     retValue = retValue + 1;
  }
  
  if(digitalRead(inputPin2) == HIGH) { // if pin is high, return value is 2 (0010)
     retValue = retValue + 2;
  }

  if(digitalRead(inputPin3) == HIGH) { // if pin is high, return value is 4 (0100)
     retValue = retValue + 4;
  }
  
  if(digitalRead(inputPin4) == HIGH) { // if pin is high, return value is 8 (1000)
     retValue = retValue + 8;
  }
  
  i = 0; //If new data, restart the loop
 
  if((retValue & 0x0003) == 0x0003 ) {    // UnknownTarget
     UnknownTarget();
  }
  else if(retValue & 0x0001) {            // HotGoalLeft
    HotGoalLeft();
  }
  else if(retValue & 0x0002) {            // HotGoalRight
    HotGoalRight();
  } 
  else if((retValue & 0x000C) == 0x000C) {// TuskRetractPosition
    TuskRetractPosition();
  }
  else if(retValue & 0x0004) {            // TuskIntermediatePosition
    TuskIntermediatePosition();
  }
  else if(retValue & 0x0008) {            // TuskExtendedPosition
    TuskExtendedPosition();
  }
  else {
    TurnOffLights();
  }
  
  strip.show();
}

//======================================================//
// Tusk Retracted
//======================================================//
void TuskRetractPosition() {
  for(int x = 0; x < strip.numPixels(); x++) {
    strip.setPixelColor(x, strip.Color(0,0,127)); // Blue
  }
}

//======================================================//
// Tusk Intermediate Position
//======================================================//
void TuskIntermediatePosition() {  
  for(int q = 0; q < strip.numPixels(); q++){
    strip.setPixelColor(q, strip.Color(127,127,0)); // Yellow    
  } 
}

//======================================================//
// Tusk Extended Position
//======================================================//
void TuskExtendedPosition() {
  for(int q = 0; q < strip.numPixels(); q++){
    strip.setPixelColor(q, strip.Color(127,0,0)); // Red
  }
}

//======================================================//
// Auto Mode - Hot Goal Left
//======================================================// 
void HotGoalLeft() {
  //Set left half of strip red
  for(int q = 0; q < (strip.numPixels())/2; q++){
    strip.setPixelColor(q, strip.Color(127,0,0));
  }
  //Turn off the right half
  for(int q = strip.numPixels()/2; q < (strip.numPixels()); q++){
    strip.setPixelColor(q, strip.Color(0,0,0)); 
  }
}

//======================================================//
// Auto Mode - Hot Goal Right
//======================================================// 
void HotGoalRight() {
  //Set the right half of the strip green.
  for(int q = strip.numPixels()/2; q < (strip.numPixels()); q++){
    strip.setPixelColor(q, strip.Color(0,127,0)); 
  }
  //Turn off the left half.
  for(int q = 0; q < strip.numPixels()/2; q++){
    strip.setPixelColor(q, strip.Color(0,0,0));
  }
}

//======================================================//
// Auto Mode - Unknown Hot Goal Target
//======================================================//
void UnknownTarget() {
  for(int q = 0; q < strip.numPixels(); q++) {
    strip.setPixelColor(q, strip.Color(127,60,0)); // Orange
  }
}

//======================================================//
// TurnOffLights
//======================================================//
void TurnOffLights() {
  for(int q = 0; q < strip.numPixels(); q++){
    strip.setPixelColor(q, strip.Color(0,0,0)); // Off
  }  
}

