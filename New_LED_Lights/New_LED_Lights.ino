#include "FastLED.h"
#include <Wire.h>

#define NUM_LEDS 41
#define DATA_PIN 4
#define I2C_ID 10
#define RANGE_COUNT 3 //<--- make sure you change this to the amount of ranges being used! It's responsible for a few loops and 2D array amounts! 

#define OFF_PATTERN_ID 0 //PatData = None.
#define SOLID_PATTERN_ID 1 //PatData = None.
#define BLINK_PATTERN_ID 2 //PatData = Speed in hertz.
#define FADE_PATTERN_ID 3 //PatData = none.
#define CHASE_PATTERN_ID 4 //patData = 0 for chase in, 1 for chase out, 2 for chase all colors..
#define RAINBOW_PATTERN_ID 5 //patData = none.
int counter = 0;
boolean fadeIn = true;
boolean hasChanged = false;
int gHue = 0;

int curChase = 0;

/*int gearIntakeRange[2] = {0,2}; //<---- Gross old fangled way of doing things, yick!
int shooterRange[2] = {3, 8};
int driveTrainRange[2] = {9,14};*/

int ranges[RANGE_COUNT][2] = {{0,12},
                              {13,26},
                              {27,40}};

byte lightStates[RANGE_COUNT][5];

CRGB curCol = CRGB::Red;

CRGB leds[NUM_LEDS];

void setup() {
  
  
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
  
  for(int i=0; i<RANGE_COUNT;i++){
    lightStates[i][0] = 100; //Red
    lightStates[i][1] = 0;  //Green
    lightStates[i][2] = 0;  //Blue
    lightStates[i][3] = 6;  //Pattern
    lightStates[i][4] = 0;  //PatternData(if neccessary)
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
    makePatterns(lightStates[x][0], lightStates[x][1], lightStates[x][2], lightStates[x][3], lightStates[x][4],ranges[x][0], ranges[x][1]);
  }
  
  
  FastLED.show();
  delay(10);
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
  EVERY_N_MILLISECONDS( 10 ) { IncrementCounter(); }
}

void makePatterns(int r1, int g1, int b1, int pattern, int patData, int startLED, int endLED) {//Its so pretty now!
  switch(pattern) {
    case SOLID_PATTERN_ID:
      Fill(CRGB(r1, g1, b1), startLED, endLED);
      break;
    case BLINK_PATTERN_ID:
      Blink(CRGB(r1, g1, b1), startLED, endLED, patData);
      break;
    case FADE_PATTERN_ID:
      ColorFadeInOut(CRGB(r1, g1, b1), startLED, endLED);
      break;
    case CHASE_PATTERN_ID:
      switch(patData){
        case 1:
          ChaseOut(CRGB(r1,g1,b1), startLED, endLED);
          break;
        case 2:
          ChaseAll(startLED, endLED);
          break;
        default:
          ChaseIn(CRGB(r1, g1, b1), startLED, endLED);
          break;
      }
    case RAINBOW_PATTERN_ID:
      Rainbow(gHue);
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
bool centerOn = false;
int i = (int) ( ( (double)counter / (double)200) * int(((endLED - startLED + 1)) + 0.5));
  if (i == 0) {
    Off(startLED, endLED);
    centerOn = false;
  }

  Serial.print(i);

  if(!centerOn){
    leds[(int)(startLED + endLED) / 2] = color;
    centerOn = true;
  }
    leds[((startLED + endLED) / 2) - i - 1] = color;
    leds[((startLED + endLED) / 2) + i + 1] = color;
}

void Rainbow(int gHue) 
{
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void Blink(CRGB color, int startLED, int endLED, int freq) {
  if ((counter % (freq * 10)) == 0) {
    if ( leds[startLED] ) {                     //test one LED in range to see if it is not black
      Off(startLED, endLED);
    }
  else {
      Fill(color, startLED, endLED);
    }
  }
}

void ChaseAll(int startLED, int endLED){
  int i = (int) ( ( (double)counter / (double)100) * int(((endLED - startLED + 1)) + 0.5));

    if(i != 0 && hasChanged){
      hasChanged = false;
    }
    
   if(i == 0 && !hasChanged){
      if(curChase == 0){
          
        curChase = 1;
        hasChanged = true;
        curCol = CRGB::Green;
      }else if(curChase == 1 && !hasChanged){
        curChase = 2;
        hasChanged = true;
        curCol = CRGB::Blue;
      }else if(!hasChanged && curChase == 2){
        curChase = 0;
        hasChanged = true;
        curCol = CRGB::Red;
      }
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
    for(int x=0; x < RANGE_COUNT; x++){
     for(int i=0; i < 4; i++) { 
        lightStates[x][i] = Wire.read();
     } 
    }
   }
}

//That's all folks!
