//Note, the TM1803V2.0 strip I'm using looks like it takes values
//  in the form 0xRRBBGG, doesn't look like FAST LED is working
//  correctly out of the box w/ appropriate colors on this strip.

#include <SPI.h>
#include <Wire.h>
#include <FastLED.h>

#define STRIP_LENGTH 4
#define SLAVE_ID 4
#define FLASH_DELAY 500

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

  Wire.begin(SLAVE_ID);         // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(Wire.available()) { // loop through all but the last
    retValue = Wire.read();
    Serial.println(retValue, HEX);
  }
}

void loop() {
  
  //===========================================
  //  Hex        AUTO MODE               Bits#
  // Values                             3,2,1,0
  //===========================================
  //  1       HotGoalLeft           // 0 0 0 1
  //  5       HotGoalLeft2Ball      // 0 1 0 1
  //  2       HotGoalRight          // 0 0 1 0
  //  6       HotGoalRight2Ball     // 0 1 1 0
  //  3       UnknownTarget         // 0 0 1 1
  //  7       UnknownTarget2Ball    // 0 1 1 1
  //===========================================
  //          TELEOP MODE             Bits
  //===========================================
  //  0       Off                   // 0 0 0 0
  //  8       TuskExtendedPosition  // 1 0 0 0 
  //  4       TuskIntermediate      // 0 1 0 0
  //  C       TuskRetractPosition   // 1 1 0 0
  //===========================================

  if(retValue == 12) {           // TuskRetractPosition
    TuskRetractPosition();
  } else if(retValue == 8) {     // TuskExtendedPosition
    TuskExtendedPosition();
  } else if(retValue == 7) {     // Unknown target - flash
    UnknownTarget();
    FastLED.show();
    delay(FLASH_DELAY);
    TurnOffLights();
    FastLED.show();
    delay(FLASH_DELAY);  
  } else if(retValue == 6) {    // Hot goal right - flash
    HotGoalRight();
    FastLED.show();
    delay(FLASH_DELAY);
    TurnOffLights();
    FastLED.show();
    delay(FLASH_DELAY);
  } else if(retValue == 5) {    // Hot goal left - flash
    HotGoalLeft();
    FastLED.show();
    delay(FLASH_DELAY);
    TurnOffLights();
    FastLED.show();
    delay(FLASH_DELAY);
  } else if(retValue == 4) {     // TuskIntermediatePosition
    TuskIntermediatePosition();
  } else if(retValue == 3 ) {    // UnknownTarget
    UnknownTarget();
  } else if(retValue == 2) {     // HotGoalRight
    HotGoalRight();
  } else if(retValue == 1) {     // HotGoalLeft 
    HotGoalLeft();
  } else {
    TurnOffLights(); 
  }
   
  FastLED.show();
}

//======================================================//
// Tusk Retracted
//======================================================//
void TuskRetractPosition() {
  for(int x = 0; x < STRIP_LENGTH; x++) {
    leds[x] = 0x00FF00; //Blue
  }
}

//======================================================//
// Tusk Intermediate Position
//======================================================//
void TuskIntermediatePosition() {  
  for(int q = 0; q < STRIP_LENGTH; q++){
    leds[q] = 0xFF00FF; //Yellow
  } 
}

//======================================================//
// Tusk Extended Position
//======================================================//
void TuskExtendedPosition() {
  for(int q = 0; q < STRIP_LENGTH; q++){
    leds[q] = 0xFF0000;
  }
}

//======================================================//
// Auto Mode - Hot Goal Left
//======================================================// 
void HotGoalLeft() {
  //Set left half of strip green
  for(int q = 0; q < (STRIP_LENGTH)/2; q++){
    leds[q] = 0x0000FF;
  }
  //Turn off the right half
  for(int q = STRIP_LENGTH/2; q < STRIP_LENGTH; q++){
    leds[q] = 0x000000;
  }
}

//======================================================//
// Auto Mode - Hot Goal Right
//======================================================// 
void HotGoalRight() {
  //Set the right half of the strip red.
  for(int q = STRIP_LENGTH/2; q < STRIP_LENGTH; q++){
    leds[q] = 0xFF0000;
  }
  //Turn off the left half.
  for(int q = 0; q < STRIP_LENGTH/2; q++){
    leds[q] = 0x000000;
  }
}

//======================================================//
// Auto Mode - Unknown Hot Goal Target
//======================================================//
void UnknownTarget() {
  for(int q = 0; q < STRIP_LENGTH; q++) {
    leds[q] = 0xFF00FF;
  }
}

//======================================================//
// TurnOffLights
//======================================================//
void TurnOffLights() {
  for(int q = 0; q < STRIP_LENGTH; q++){
    leds[q] = 0x000000;
  }  
}

