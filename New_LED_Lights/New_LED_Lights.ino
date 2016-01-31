#include "FastLED.h"

#define DATA_PIN 2
#define NUM_LEDS 11

void IncrementCounter();
void ColorFadeInOut(int r, int g, int b);
void ChaseIn(CRGB color);

int counter = 0;
boolean fadeIn = true;

CRGB leds[NUM_LEDS];
void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
}

void loop() {
  ColorFadeInOut(255, 0, 0);
  FastLED.show();
  delay(10);
  EVERY_N_MILLISECONDS( 10 ) { IncrementCounter(); }
}

void IncrementCounter() {
  counter++;
  if (counter == 100) {
    counter = 0;
    fadeIn = !fadeIn;
  }
}

void ColorFadeInOut(int r, int g, int b) {
  CRGB color = CRGB(r,g,b);
  if (fadeIn) {
    double factor = ((double)counter)/((double)100);
    color.r = factor*color.r;
  }
  else {
    double factor = ((double)(100-counter))/((double)100);
    color.r = factor*color.r;
  }
  for (int i=0;i<NUM_LEDS;i++) {
    leds[i] = color;
  }
}

void ChaseIn(CRGB color) {
  int i = int((counter/100)*(NUM_LEDS/2));
  leds[i] = color;
}

void SolidColor(CRGB color){
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = color;
  }
}

