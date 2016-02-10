#include "FastLED.h"
#include <Wire.h>

#define NUM_LEDS 40
#define DATA_PIN 2
#define I2C_ID 10

int counter = 0;
boolean fadeIn = true;
int gHue = 0;
byte lightStates[6] = {0, 0, 0, 0, 0, 0};

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);

  //begins the i2c connection.
  Wire.begin(I2C_ID);
  //Sets the Recieve Event Function
  Wire.onReceive(receiveEvent);
}

void loop() {
  ColorFadeInOut(CRGB(lightStates[1], lightStates[2], lightStates[3]), lightStates[4], lightStates[5]);
  FastLED.show();
  delay(10);
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
  EVERY_N_MILLISECONDS( 10 ) { IncrementCounter(); }
}

void IncrementCounter() {
  counter++;
  if (counter == 100) {
    counter = 0;
    fadeIn = !fadeIn;
    Off();
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
  for (int i=startLED;i<endLED+1;i++) {
    leds[i] = color;
  }
}

void StaticColor(CRGB color, int startLED, int endLED){
  for (int i=startLED; i<endLED+1; i++){
    leds[i] = color;
  }
}

void ChaseIn(CRGB color, int startLED, int endLED) {
  int i = (int) ( ( (double)counter / (double)100) * int(((endLED - startLED + 1) / 2) + 0.5));
  Serial.print(String(i) + "\n");
  leds[startLED + i] = color;
  leds[endLED - i] = color;
}

void Rainbow(int gHue) 
{
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void Blink(CRGB color, int startLED, int endLED) {
  if(counter <= 50){
    for (int i=startLED; i<endLED+1; i++){
      leds[i] = color;
    }  
  }
  if(counter > 50){
    for (int i=startLED; i<endLED+1; i++){
      leds[i] = 0;
    }
  }
}

void Off(){
  for (int i = 0; i < NUM_LEDS; i++){
      leds[i] = 0;
    }
}

void receiveEvent(int numBytes){
  while(Wire.available() != 0){
    lightStates[6 - Wire.available()] = Wire.read();
  }
}

