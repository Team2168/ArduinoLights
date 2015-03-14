#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 150

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 3
#define CLOCK_PIN 13
#define INPUT_PIN_1 10
#define INPUT_PIN_2 11
#define INPUT_PIN_3 12

// Define the array of leds
CRGB leds[NUM_LEDS];
int leds1[] = {0,49};
int leds2[] = {50,99};
int leds3[] = {100,149};

void ColorSweep(int r, int g, int b, int range[]);
void AlternatingColorSweep(int r1, int g1, int b1, int r2, int g2, int b2, int range[]);

double i = 0;

void setup() { 
      // Uncomment/edit one of the following lines for your leds arrangement.
      // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
      FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);
      
      // FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);

      // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      Serial.begin(9600);
}

void loop() { 
  if(digitalRead(INPUT_PIN_1) == HIGH) {
      ColorSweep(127, 0, 0, leds1);
  }
  else if(digitalRead(INPUT_PIN_1) == LOW) {
      ColorSweep(0, 127, 0, leds1);
  }
  if(digitalRead(INPUT_PIN_2) == HIGH) {
      ColorSweep(127, 0, 0, leds2);
  }
  else if(digitalRead(INPUT_PIN_2) == LOW) {
      ColorSweep(0, 0, 127, leds2);
  }
  if(digitalRead(INPUT_PIN_3) == HIGH) {
      ColorSweep(0, 127, 0, leds3);
  }
  else if(digitalRead(INPUT_PIN_3) == LOW) {
      ColorSweep(0, 0, 127, leds3);
  }
  FastLED.show();
  delay(50);
  
  i+=0.1;
  if (i >= 2.0) {
    i = 0;
  }  
}

void ColorSweep(int r, int g, int b, int range[]) {
  if (i < 1) {
    for (int j=range[0];j<=range[1];j++) {
      leds[j] = CRGB(i*r,i*g,i*b);
    }  
  }
  else {
    double ix = 2.0-i;
    for (int j=range[0];j<=range[1];j++) {
      leds[j] = CRGB(ix*r,ix*g,ix*b);
    }  
  }
}

void AlternatingColorSweep(int r1, int g1, int b1, int r2, int g2, int b2, int range[]) {
  if (i < 1) {
    for (int j=range[0];j<=range[1];j++) {
      if (j % 2 == 0) {
        leds[j] = CRGB(i*r1,i*g1,i*b1);
      }
      else {
        leds[j] = CRGB(i*r2,i*g2,i*b2);
      }
    }  
  }
  else {
    double ix = 2.0-i;
    for (int j=range[0];j<=range[1];j++) {
      if (j % 2 == 0) {
        leds[j] = CRGB(ix*r1,ix*g1,ix*b1);
      }
      else {
        leds[j] = CRGB(ix*r2,ix*g2,ix*b2);
      }
    }  
  }
}
