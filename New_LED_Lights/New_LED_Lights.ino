#include "FastLED.h"
#include <Wire.h>

#define NUM_LEDS 54
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
bool hasChanged = false;
int gHue = 0;

int curChase = 0;

int intakeRange[2] = {0,14};
int shooterRange[2] = {52, 53};

byte lightStates[8] = {0, 0, 0, 7, 0, 0, 0, 0}; //range1(RGB pattern) range2(RGB pattern)

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
               lightStates[4], lightStates[5], lightStates[6], lightStates[7]);
  
  for(int i=0;i<8;i++) {
    //Serial.print((String)lightStates[i]);
  }  
 // Serial.print("\n");
  
  FastLED.show();
  delay(10);
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
  EVERY_N_MILLISECONDS( 10 ) { IncrementCounter(); }
}

void makePatterns(int r1, int g1, int b1, int pattern1, int r2, int g2, int b2, int pattern2) {
  switch(pattern1) {
    case SOLID_PATTERN_ID:
      Fill(CRGB(r1, g1, b1), intakeRange[0], intakeRange[1]);
      break;
    case FAST_BLINK_PATTERN_ID:
      Blink(CRGB(r1, g1, b1), intakeRange[0], intakeRange[1], 10);
      break;
    case SLOW_BLINK_PATTERN_ID:
      Blink(CRGB(r1, g1, b1), intakeRange[0], intakeRange[1], 50);
      break;
    case FADE_PATTERN_ID:
      ColorFadeInOut(CRGB(r1, g1, b1), intakeRange[0], intakeRange[1]);
      break;
    case CHASE_PATTERN_ID:
      ChaseIn(CRGB(r1, g1, b1), intakeRange[0], intakeRange[1]);
      break;
    case RAINBOW_PATTERN_ID:
      Rainbow(gHue);
      break;
    case CHASE_ALL_PATTERN_ID:
      ChaseAll(intakeRange[0], intakeRange[1]);
      break;
    case CHASE_OUT_PATTERN_ID:
      ChaseOut(CRGB(r1,g1,b1), intakeRange[0], intakeRange[1]);
      break;
    default: //OFF_PATTERN_ID:
      Off(intakeRange[0], intakeRange[1]);
      break;
  }
  switch(pattern2) {
    case SOLID_PATTERN_ID:
      Fill(CRGB(r2, g2, b2), shooterRange[0], shooterRange[1]);
      break;
    case FAST_BLINK_PATTERN_ID:
      Blink(CRGB(r2, g2, b2), shooterRange[0], shooterRange[1], 10); //blink at 5 Hz
      break;
    case SLOW_BLINK_PATTERN_ID:
      Blink(CRGB(r2, g2, b2), shooterRange[0], shooterRange[1], 50); //blink at 1 Hz
      break;
    case FADE_PATTERN_ID:
      ColorFadeInOut(CRGB(r2, g2, b2), shooterRange[0], shooterRange[1]);
      break;
    case CHASE_PATTERN_ID:
      ChaseIn(CRGB(r2, g2, b2), shooterRange[0], shooterRange[1]);
      break;
    case RAINBOW_PATTERN_ID:
      Rainbow(gHue);
      break;
    case CHASE_ALL_PATTERN_ID:
      ChaseAll(shooterRange[0], shooterRange[1]);
      break;
    case CHASE_OUT_PATTERN_ID:
      ChaseOut(CRGB(r2,g2,b2), shooterRange[0], shooterRange[1]);
      break;
    default: //OFF_PATTERN_ID:
      Off(shooterRange[0], shooterRange[1]);
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
  int i = (int) ( ( (double)counter / (double)100) * int(((endLED - startLED + 1) / 2) + 0.5));
  if (i == 0) {
    Off(startLED, endLED);
  }
  leds[endLED + i] = color;
  leds[startLED - i] = color;
}

void ChaseOut(CRGB color, int startLED, int endLED) {
  int i = (int) ( ( (double)counter / (double)100) * int(((endLED - startLED + 1) / 2) + 0.5));
  if (i == 0) {
    Off(startLED, endLED);
  }
  leds[endLED / 2 - i] = color;
  leds[endLED / 2 + i] = color;
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
  
  

  
   if(i == 0){
      if(curChase == 0 && !hasChanged){
        curCol = CRGB::Green;  
        curChase ++;
        hasChanged = true;
        Serial.print("changed To Green");
      }else if(curChase == 1 && !hasChanged){
        curCol = CRGB::Blue;
        curChase ++;
        hasChanged = true;
        Serial.print("changed To Blue");
      }else if(!hasChanged){
        curCol = CRGB::Red;
        curChase = 0;
        hasChanged = true;
        Serial.print("changed To Red");
      }
    }
    
    leds[startLED + i] = curCol;
    leds[endLED - i] = curCol;
    if(hasChanged && i != 0){
      hasChanged = false;
    }
  
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
