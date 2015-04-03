#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 200

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PiN.  For led chipsets that are SPi based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PiN and CLOCK_PiN
#define DATA_PIN 2
#define INPUT_PIN_1 13 //Gripper engaged
#define INPUT_PIN_2 12 //Intake engaged
#define INPUT_PIN_3 11 //Intake tote in/out
#define INPUT_PIN_4 10 //Enabled

// Define the array of leds
CRGB leds[NUM_LEDS];
int intkLedsLeft[] = {0, 59};//{0, 59}; //60 LEDs
int intkLedsRight[] = {60, 119};//{60, 119}; //60 LEDs
int gripLeds[] = {120,146};//27 leds long
int leds3[] = {21,29}; //place holder

//int intkLeds[] = {0,119}; //120leds long; first 60 are onm the right side, 2nd 60 on left
//int gripLeds[] = {120,146};//27 leds long
//int leds3[] = {147,157}; //place holder

void ColorBlink(int r, int g, int b, int range[]);
void AlternatingColorBlink(int r1, int g1, int b1, int r2, int g2, int b2, int range[]);
void RainbowSweep(int range[]);
void StaticColor(int r, int g, int b, int range[]);

double i = 0;

void setup() {
      FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
      Serial.begin(9600);
}

void loop() {
  //robot is disabled:
  if(digitalRead(INPUT_PIN_4) == LOW) {
      RainbowSweep(gripLeds);
      RainbowSweep(intkLedsLeft);
      RainbowSweep(intkLedsRight);
  }
  else {
    // gripper is in: 
    if(digitalRead(INPUT_PIN_1) == HIGH) {
        StaticColor(0, 0, 128, gripLeds);
    }
    // gripper is out: 
    else if(digitalRead(INPUT_PIN_1) == LOW) {
        StaticColor(128, 0, 0, gripLeds);
    }
    // intake is engaged: 
    if (digitalRead(INPUT_PIN_2) == HIGH) {
        StaticColor(128, 0, 0, intkLedsLeft);

    }
    // intake is not engaged: 
    else if (digitalRead(INPUT_PIN_2) == LOW) {
        StaticColor(0, 0, 128, intkLedsLeft);
    }
    //tote intake is in:  
    if (digitalRead(INPUT_PIN_3) == HIGH) {
        ColorBlink(128, 0, 0, intkLedsRight);
    }
    //tote intake is out: 
    else if (digitalRead(INPUT_PIN_3) == LOW) {
        ColorBlink(0, 0, 128, intkLedsRight);
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
      leds[j] = CHSV(i*127,255,120);
    }
}

void StaticColor(int r, int g, int b, int range[]) {
  for (int j=range[0];j<=range[1];j++) {
      leds[j] = CRGB(r,g,b);
    }
}
