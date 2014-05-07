#include <SPI.h>
#include <FastLED.h>

#define STRIP_LENGTH 18

// Wiring (signals from cRIO)
#define inputPin1 2
#define inputPin2 3
#define inputPin3 4
#define inputPin4 5

//Strip communication pins
#define DATA_PIN  9

//Set up LED strip
CRGB leds[STRIP_LENGTH];

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
  FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, STRIP_LENGTH);
  
  //light all pixels on the strip to verify they're working
  for(int q = 0; q < STRIP_LENGTH; q++) {
    leds[q].setRGB(127,127,127);
  }
  FastLED.show();
  delay(1500);
   
  //Turn the LEDs back off
  TurnOffLights();
  FastLED.show();

  // Note, pins 10 - 13 are in use by the SPI library. 
  pinMode(inputPin1, INPUT); 
  pinMode(inputPin2, INPUT); 
  pinMode(inputPin3, INPUT);  
  pinMode(inputPin4, INPUT);  

  Serial.begin(9600); // Serial Monitor
}

void loop() {
  
  //=======================================
  //  Hex        AUTO MODE               Bits#
  // Values                             3,2,1,0
  //=======================================            
  //  1       HotGoalLeft           // 0 0 0 1
  //  5       HotGoalLeft2Ball      // 0 1 0 1
  //  2       HotGoalRight          // 0 0 1 0
  //  6       HotGoalRight2Ball     // 0 1 1 0
  //  3       UnknownTarget         // 0 0 1 1
  //  7       UnknownTarget2Ball    // 0 1 1 1
  //======================================= 
  //          TELEOP MODE             Bits
  //=======================================
  //  0       Off                   // 0 0 0 0 
  //  8       TuskExtendedPosition  // 1 0 0 0  
  //  4       TuskIntermediate      // 0 1 0 0
  //  C       TuskRetractPosition   // 1 1 0 0
  //======================================= 
  //                 PIN MAP
  //       Bits ->  3 2 1 0
  // INPUT PINS ->  4 3 2 1
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
 
  if(retValue== 0x0003 ) {    // UnknownTarget
    UnknownTarget();
  }
  if(retValue == 2) {         // HotGoalRight
    HotGoalRight();
  }
  
  if(retValue == 6) {
    HotGoalRight();
    FastLED.show();
    delay(100);
    TurnOffLights();
    FastLED.show();
    delay(100);
  }
  if(retValue == 5) {
    HotGoalLeft();
    FastLED.show();
    delay(100);
    TurnOffLights();
    FastLED.show();
    delay(100);
  }
  if(retValue == 1) {            // HotGoalLeft 
    HotGoalLeft();
  } 
  if(retValue == 12) {           // TuskRetractPosition
    TuskRetractPosition();
  }
  if(retValue == 4) {            // TuskIntermediatePosition
    TuskIntermediatePosition();
  }
  if(retValue == 8) {            // TuskExtendedPosition
    TuskExtendedPosition();
  }
  if(retValue == 0) {
    TurnOffLights(); 
  }
  if(retValue == 7) {
    UnknownTarget();
    FastLED.show();
    delay(100);
    TurnOffLights();
    FastLED.show();
    delay(100);  
  }
  
  FastLED.show();
}

//======================================================//
// Tusk Retracted
//======================================================//
void TuskRetractPosition() {
  for(int x = 0; x < STRIP_LENGTH; x++) {
    leds[x].setRGB(0, 0, 255); // Blue
  }
}

//======================================================//
// Tusk Intermediate Position
//======================================================//
void TuskIntermediatePosition() {  
  for(int q = 0; q < STRIP_LENGTH; q++){
    leds[q].setRGB(255, 255, 0); // Yellow    
  } 
}

//======================================================//
// Tusk Extended Position
//======================================================//
void TuskExtendedPosition() {
  for(int q = 0; q < STRIP_LENGTH; q++){
    leds[q].setRGB(255, 0, 0); // Red
  }
}

//======================================================//
// Auto Mode - Hot Goal Left
//======================================================// 
void HotGoalLeft() {
  //Set left half of strip green
  for(int q = 0; q < (STRIP_LENGTH)/2; q++){
    leds[q].setRGB(0, 255, 0);
  }
  //Turn off the right half
  for(int q = STRIP_LENGTH/2; q < STRIP_LENGTH; q++){
    leds[q].setRGB(0, 0, 0);
  }
}

//======================================================//
// Auto Mode - Hot Goal Right
//======================================================// 
void HotGoalRight() {
  //Set the right half of the strip red.
  for(int q = STRIP_LENGTH/2; q < STRIP_LENGTH; q++){
    leds[q].setRGB(255, 0, 0);
  }
  //Turn off the left half.
  for(int q = 0; q < STRIP_LENGTH/2; q++){
    leds[q].setRGB(0, 0, 0);
  }
}

//======================================================//
// Auto Mode - Unknown Hot Goal Target
//======================================================//
void UnknownTarget() {
  for(int q = 0; q < STRIP_LENGTH; q++) {
    leds[q].setRGB(255, 120, 0); // Orange
  }
}

//======================================================//
// TurnOffLights
//======================================================//
void TurnOffLights() {
  for(int q = 0; q < STRIP_LENGTH; q++){
    leds[q].setRGB(0, 0, 0);
  }  
}

