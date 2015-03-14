#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 150

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 3
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];
int leds1[] = {0,49};
int leds2[] = {50,99};
int leds3[] = {100,149};

void ColorSweep(int r, int g, int b, int range[]);
void AlternatingColorSweep(int r1, int g1, int b1, int r2, int g2, int b2, int range[]);

double i1 = 0;
double i2 = 0;
double i3 = 0;

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
  ColorSweep(127, 64, 0, leds1);
  AlternatingColorSweep(127, 127, 0, 64, 0, 127, leds2);
  FastLED.show();
  delay(500);
}

void ColorSweep(int r, int g, int b, int range[]) {
  if (i1 < 1) {
    for (int j=range[0];j<=range[1];j++) {
      leds[j] = CRGB(i1*r,i1*g,i1*b);
    }  
    i1+=0.1;
  }
  else {
    double i = 2.0-i1;
    for (int j=range[0];j<=range[1];j++) {
      leds[j] = CRGB(i*r,i*g,i*b);
    }  
    i1+=0.1;
  }
  if (i1 >= 2.0) {
    i1 = 0;
  }  
}

void AlternatingColorSweep(int r1, int g1, int b1, int r2, int g2, int b2, int range[]) {
  if (i2 < 1) {
    for (int j=range[0];j<=range[1];j++) {
      if (j % 2 == 0) {
        leds[j] = CRGB(i2*r1,i2*g1,i2*b1);
      }
      else {
        leds[j] = CRGB(i2*r2,i2*g2,i2*b2);
      }
    }  
    i2+=0.1;
  }
  else {
    double i = 2.0-i2;
    for (int j=range[0];j<=range[1];j++) {
      if (j % 2 == 0) {
        leds[j] = CRGB(i*r1,i*g1,i*b1);
      }
      else {
        leds[j] = CRGB(i*r2,i*g2,i*b2);
      }
    }  
    i2+=0.1;
    Serial.println(i);
  }
  if (i2 >= 2) {
    i2 = 0;
  }
}

void RainbowSweep() {
  
}
