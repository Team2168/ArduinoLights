#include "FastLED.h"
#include <Wire.h>

#define NUM_LEDS 15
#define DATA_PIN 4
#define I2C_ID 10

#define OFF_PATTERN_ID 0
#define SOLID_PATTERN_ID 1
#define FAST_BLINK_PATTERN_ID 2
#define SLOW_BLINK_PATTERN_ID 3
#define FADE_PATTERN_ID 4
#define CHASE_PATTERN_ID 5
#define RAINBOW_PATTERN_ID 6
#define CHASE_ALL_PATTERN_ID 7
#define CHASE_OUT_PATTERN_ID 8
int counter = 0;
boolean fadeIn = true;
boolean hasChanged = false;
int gHue = 0;

int curChase = 0;

/*int gearIntakeRange[2] = {0,2}; //<---- Gross old fangled way of doing things, yick!
int shooterRange[2] = {3, 8};
int driveTrainRange[2] = {9,14};*/

int ranges[3][2] = {{0,2},{3,8},{9,14}};



byte lightStates[12] = {100, 100, 100, 1, 255, 0, 0, 7, 0, 0, 255, 1}; //range1(RGB pattern) range2(RGB pattern)

CRGB curCol = CRGB::Red;

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);

  for (int i = 0; i < NUM_LEDS; i++){
      leds[i] = CRGB::White;
    }
  FastLED.show();
  delay(1000);
  Off(0, NUM_LEDS-1);
  
  //begins the i2c connection.
  Wire.begin(I2C_ID);
  //Sets the Recieve Event Function
  Wire.onReceive(receiveEvent);
}

void loop() {
  makePatterns(lightStates[0], lightStates[1], lightStates[2], lightStates[3],
               lightStates[4], lightStates[5], lightStates[6], lightStates[7],
               lightStates[8], lightStates[9], lightStates[10], lightStates[11]);
  
  for(int i=0;i<8;i++) {
    //Serial.print((String)lightStates[i]);
  }  
 // Serial.print("\n");
  
  FastLED.show();
  delay(10);
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
  EVERY_N_MILLISECONDS( 10 ) { IncrementCounter(); }
}

void makePatterns(int r1, int g1, int b1, int pattern1, int r2, int g2, int b2, int pattern2, int r3, int g3, int b3, int pattern3) {
  switch(pattern1) {
    case SOLID_PATTERN_ID:
      Fill(CRGB(r1, g1, b1), ranges[0][0], ranges[0][1]);
      break;
    case FAST_BLINK_PATTERN_ID:
      Blink(CRGB(r1, g1, b1), ranges[0], ranges[0][1], 10);
      break;
    case SLOW_BLINK_PATTERN_ID:
      Blink(CRGB(r1, g1, b1), ranges[0][0], ranges[0][1], 50);
      break;
    case FADE_PATTERN_ID:
      ColorFadeInOut(CRGB(r1, g1, b1), ranges[0][0], ranges[0][1]);
      break;
    case CHASE_PATTERN_ID:
      ChaseIn(CRGB(r1, g1, b1), ranges[0][0], ranges[0][1]);
      break;
    case RAINBOW_PATTERN_ID:
      Rainbow(gHue);
      break;
    case CHASE_ALL_PATTERN_ID:
      ChaseAll(ranges[0][0], ranges[0][1]);
      break;
    case CHASE_OUT_PATTERN_ID:
      ChaseOut(CRGB(r1,g1,b1), ranges[0][0], ranges[0][1]);
      break;
    default: //OFF_PATTERN_ID:
      Off(ranges[0][0], ranges[0][1]);
      break;
  }
  switch(pattern2) {
    case SOLID_PATTERN_ID:
      Fill(CRGB(r2, g2, b2), ranges[1][0], ranges[1][1]);
      break;
    case FAST_BLINK_PATTERN_ID:
      Blink(CRGB(r2, g2, b2), ranges[1][0], ranges[1][1], 10); //blink at 5 Hz
      break;
    case SLOW_BLINK_PATTERN_ID:
      Blink(CRGB(r2, g2, b2), ranges[1][0], ranges[1][1], 50); //blink at 1 Hz
      break;
    case FADE_PATTERN_ID:
      ColorFadeInOut(CRGB(r2, g2, b2), ranges[1][0], ranges[1][1]);
      break;
    case CHASE_PATTERN_ID:
      ChaseIn(CRGB(r2, g2, b2), ranges[1][0], ranges[1][1]);
      break;
    case RAINBOW_PATTERN_ID:
      Rainbow(gHue);
      break;
    case CHASE_ALL_PATTERN_ID:
      ChaseAll(ranges[1][0], ranges[1][1]);
      break;
    case CHASE_OUT_PATTERN_ID:
      ChaseOut(CRGB(r2,g2,b2), ranges[1][0], ranges[1][1]);
      break;
    default: //OFF_PATTERN_ID:
      Off(ranges[1][0], ranges[1][1]);
      break;
  }
  switch(pattern3) {
    case SOLID_PATTERN_ID:
      Fill(CRGB(r3, g3, b3), ranges[2][0], ranges[2][1]);
      break;
    case FAST_BLINK_PATTERN_ID:
      Blink(CRGB(r3, g3, b3), ranges[2][0],ranges[2][1], 10); //blink at 5 Hz
      break;
    case SLOW_BLINK_PATTERN_ID:
      Blink(CRGB(r3, g3, b3), ranges[2][0], ranges[2][1], 50); //blink at 1 Hz
      break;
    case FADE_PATTERN_ID:
      ColorFadeInOut(CRGB(r3, g3, b3), ranges[2][0], ranges[2][1]);
      break;
    case CHASE_PATTERN_ID:
      ChaseIn(CRGB(r3, g3, b3), ranges[2][0],ranges[2][1]);
      break;
    case RAINBOW_PATTERN_ID:
      Rainbow(gHue);
      break;
    case CHASE_ALL_PATTERN_ID:
      ChaseAll(ranges[2][0], ranges[2][1]);
      break;
    case CHASE_OUT_PATTERN_ID:
      ChaseOut(CRGB(r3,g3,b3), ranges[2][0], ranges[2][1]);
      break;
    default: //OFF_PATTERN_ID:
      Off(ranges[2][0], ranges[2][1]);
      break;
  }
}

void IncrementCounter() {
  counter++;
  if (counter == 100) {
    counter = 0;
    fadeIn = !fadeIn;
  }
}

void ColorFadeInOut(CRGB color, int startLED, int endLED) {
  if (fadeIn) {
    double factor = ((double)counter)/((double)100);
    color.r = factor*color.r;
    color.g = factor*color.g;
    color.b = factor*color.b;
  }
  else {
    double factor = ((double)(100-counter))/((double)100);
    color.r = factor*color.r;
    color.g = factor*color.g;
    color.b = factor*color.b;
  }
  Fill(color, startLED, endLED);
}

void Fill(CRGB color, int startLED, int endLED){
  for (int i=startLED; i<=endLED; i++){
    leds[i] = color;
  }
}

void ChaseIn(CRGB color, int startLED, int endLED) {
  int i = (int) ( ( (double)counter / (double)100) * int(((endLED - startLED + 1)) + 0.5));
  if (i == 0) {
    Off(startLED, endLED);
  }
  leds[startLED + i] = color;
  leds[endLED - i] = color;
}

void ChaseOut(CRGB color, int startLED, int endLED) {
int i = (int) ( ( (double)counter / (double)100) * int(((endLED - startLED + 1)) + 0.5));
  if (i == 0) {
    Off(startLED, endLED);
  }

  leds[((startLED + endLED) / 2) - i - 1] = color;
  leds[((startLED + endLED) / 2) + i + 1] = color;
}

void Rainbow(int gHue) 
{
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void Blink(CRGB color, int startLED, int endLED, int freq) {
  if ((counter % freq) == 0) {
    if ( leds[startLED] ) {                     //test one LED in range to see if it is not black
      Off(startLED, endLED);
    }
  else {
      Fill(color, startLED, endLED);
    }
  }
}

void ChaseAll(int startLED, int endLED){
  int i = (int) ( ( (double)counter / (double)100) * int(((endLED - startLED + 1) / 2) + 0.5));

  

  
   if(i == 0 && !hasChanged){
      if(curChase == 0){
        curCol = CRGB::Green;  
        curChase ++;
        hasChanged = true;
      }else if(curChase ==1 && !hasChanged){
        curCol = CRGB::Blue;
        curChase ++;
        hasChanged = true;
      }else if(!hasChanged){
        curCol = CRGB::Red;
        curChase = 0;
        hasChanged = true;
      }
    }

    if(hasChanged && i != 0){
      hasChanged = false;  
    }
    
    leds[startLED + i] = curCol;
    leds[endLED - i] = curCol;
  
}

void Off(int startLED, int endLED){
  for (int i = startLED; i <= endLED; i++){
      leds[i] = 0;
    }
}

void receiveEvent(int numBytes){
  while(Wire.available() >= 8) { 
    for(int i=0; i < 8; i++) { 
      lightStates[i] = Wire.read();
    } 
  }
}

//That's all folks!
