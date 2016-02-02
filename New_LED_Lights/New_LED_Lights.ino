#include "FastLED.h"

#define NUM_LEDS 30

#define DATA_PIN 2
#define COM_PIN0 4
#define COM_PIN1 5
#define COM_PIN2 6
#define COM_PIN3 7

int counter = 0;
boolean fadeIn = true;

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  pinMode(COM_PIN0, INPUT);
  pinMode(COM_PIN1, INPUT);
  pinMode(COM_PIN2, INPUT);
  pinMode(COM_PIN3, INPUT);
  Serial.begin(9600);
}

void loop() {
  if(ValFromRobo() == 1){
    ColorFadeInOut(CRGB::White,0,15);
  }else if(ValFromRobo() == 3){
    Blink(CRGB::Red, 0, NUM_LEDS-1);
  }else{
    ColorFadeInOut(CRGB::Turquoise,21,29);
    StaticColor(CRGB::Orange,16,20);
    Blink(CRGB::Purple,0,15);
  }
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

int ValFromRobo() {
  int Sum = 0;

  if (digitalRead(COM_PIN0) == HIGH){
    Sum += 1;
  }
  if (digitalRead(COM_PIN1) == HIGH){
    Sum += 2;
  }
  if (digitalRead(COM_PIN2) == HIGH){
    Sum += 4;
  }
  if (digitalRead(COM_PIN3) == HIGH){
    Sum += 8;
  }
  return(Sum);
}
