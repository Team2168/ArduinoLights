#include "FastLED.h"
#include <Wire.h>

#define NUM_LEDS 15
#define DATA_PIN 4
#define I2C_ID 10
#define RANGE_COUNT 3

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

int ranges[RANGE_COUNT][2] = {{0,2},{3,8},{9,14}};

byte lightStates[RANGE_COUNT][4];

CRGB curCol = CRGB::Red;

CRGB leds[NUM_LEDS];

void setup() {
  
  
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
  
  for(int i=0; i<RANGE_COUNT;i++){
    lightStates[i][0] = 0;
    lightStates[i][1] = 0;
    lightStates[i][2] = 0;
    lightStates[i][3] = 0;
    Serial.print(i, " ");
  }

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
  for(int x= 0; x < RANGE_COUNT; x++){
    makePatterns(lightStates[x][0], lightStates[x][1], lightStates[x][2], lightStates[x][3], ranges[x][0], ranges[x][1]);
  }
  
  
  FastLED.show();
  delay(10);
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
  EVERY_N_MILLISECONDS( 10 ) { IncrementCounter(); }
}

void makePatterns(int r1, int g1, int b1, int pattern, int startLED, int endLED) { //<----- God, this thing is more of a bloat problem than most American customers.
  switch(pattern) {
    case SOLID_PATTERN_ID:
      Fill(CRGB(r1, g1, b1), startLED, endLED);
      break;
    case FAST_BLINK_PATTERN_ID:
      Blink(CRGB(r1, g1, b1), startLED, endLED, 10);
      break;
    case SLOW_BLINK_PATTERN_ID:
      Blink(CRGB(r1, g1, b1), startLED, endLED, 50);
      break;
    case FADE_PATTERN_ID:
      ColorFadeInOut(CRGB(r1, g1, b1), startLED, endLED);
      break;
    case CHASE_PATTERN_ID:
      ChaseIn(CRGB(r1, g1, b1), startLED, endLED);
      break;
    case RAINBOW_PATTERN_ID:
      Rainbow(gHue);
      break;
    case CHASE_ALL_PATTERN_ID:
      ChaseAll(startLED, endLED);
      break;
    case CHASE_OUT_PATTERN_ID:
      ChaseOut(CRGB(r1,g1,b1), startLED, endLED);
      break;
    default: //OFF_PATTERN_ID:
      Off(startLED, endLED);
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
  while(Wire.available() >= RANGE_COUNT * 4) { //<---- This gon be fixed next u just w8 m8.
    for(int x=0; x <= RANGE_COUNT; x++){
     for(int i=0; i < 4; i++) { 
        lightStates[x][i] = Wire.read();
     } 
    }
   }
}

//That's all folks!
