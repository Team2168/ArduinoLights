#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 30

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PiN.  For led chipsets that are SPi based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PiN and CLOCK_PiN
#define DATA_PIN 12
#define CLOCK_PIN 13
#define INPUT_PIN_1 2
#define INPUT_PIN_2 3
#define INPUT_PIN_3 4
#define INPUT_PIN_4 5

// Define the array of leds
CRGB leds[NUM_LEDS];
int gripLeds[] = {0,10};
int intkLeds[] = {14,17};
int leds3[] = {21,2};

void ColorBlink(int r, int g, int b, int range[]);
void AlternatingColorBlink(int r1, int g1, int b1, int r2, int g2, int b2, int range[]);
void RainbowSweep(int range[]);
void StaticColor(int r, int g, int b, int range[]);

double i = 0;

void setup() { 
  
      // Uncomment/edit one of the following lines for your leds arrangement.
      // FastLED.addLeds<TM1803, DATA_PiN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1804, DATA_PiN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1809, DATA_PiN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2811, DATA_PiN, RGB>(leds, NUM_LEDS);
      FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812B, DATA_PiN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<NEOPiXEL, DATA_PiN>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903, DATA_PiN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903B, DATA_PiN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205, DATA_PiN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205_400, DATA_PiN, RGB>(leds, NUM_LEDS);
      
      // FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);

      // FastLED.addLeds<WS2801, DATA_PiN, CLOCK_PiN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, DATA_PiN, CLOCK_PiN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, DATA_PiN, CLOCK_PiN, RGB>(leds, NUM_LEDS);
      Serial.begin(9600);
}

void loop() {
  //robot is disabled:
  if(digitalRead(INPUT_PIN_4) == HIGH) {
      RainbowSweep(gripLeds);
      RainbowSweep(intkLeds);
  }
  else {
    // gripper is in: 
    if(digitalRead(INPUT_PIN_1) == HIGH) {
        StaticColor(0, 0, 255, gripLeds);
    }
    // gripper is out: 
    else if(digitalRead(INPUT_PIN_1) == LOW) {
        StaticColor(255, 0, 0, gripLeds);
    }
    // intake is in and intake wheels are off: 
    if (digitalRead(INPUT_PIN_2) == HIGH && digitalRead(INPUT_PIN_3) == LOW) {
        StaticColor(0, 0, 255, intkLeds);
    }
    // intake is out and intake wheels are off: 
    else if (digitalRead(INPUT_PIN_2) == LOW && digitalRead(INPUT_PIN_3) == LOW) {
        StaticColor(255, 0, 0, intkLeds);
    }
    // intake is in and intake wheels are off: 
    else if (digitalRead(INPUT_PIN_2) == HIGH && digitalRead(INPUT_PIN_3) == HIGH) {
        ColorBlink(0, 0, 255, intkLeds);
    }
    // intake is out and intake wheels are off: 
    else if (digitalRead(INPUT_PIN_2) == LOW && digitalRead(INPUT_PIN_3) == HIGH) {
        ColorBlink(255, 0, 0, intkLeds);
    }
  }
  FastLED.show();
  delay(50);
  
  i+=0.1;
  if (i >= 2.0) {
    i = 0;
  }  
}

void ColorBlink(int r, int g, int b, int range[]) {
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

void AlternatingColorBlink(int r1, int g1, int b1, int r2, int g2, int b2, int range[]) {
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

void RainbowSweep(int range[]) {
  for (int j=range[0];j<=range[1];j++) {
      leds[j] = CHSV(i*127,255,255);
    }
}

void StaticColor(int r, int g, int b, int range[]) {
  for (int j=range[0];j<=range[1];j++) {
      leds[j] = CRGB(r,g,b);
    }
}
