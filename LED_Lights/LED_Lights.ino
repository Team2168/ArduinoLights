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
#define INPUT_PIN_4 10 //Intake On/Off

// Define the array of leds
CRGB leds[NUM_LEDS];
int intkLedsLeft[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                      29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15,
                      30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
                      59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45}; //60 LEDs (4x15)
                      
int intkLedsRight[] =  {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74,
                        89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75,
                        90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104,
                        119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105}; //60 LEDs (4x15)
                        
int gripLeds[] = {120, 121, 122, 123, 124, 125, 126, 127, 128,
                  137, 136, 135, 134, 133, 132, 131, 130, 129,
                  138, 139, 140, 141, 142, 143, 144, 145, 146}; //27 LEDs (3x9)

int intkLedsLeftLength = 60;
int intkLedsRightLength = 60;
int gripLedsLength = 27;

//int intkLedsLeft[] = {0, 59} //60 LEDs (4x15)
//int intkLedsRight[] = {60, 119} //60 LEDs (4x15)
//int gripLeds = {120, 146} //27 LEDs (3x9)
//int leds3[] = {21,29}; //place holder

void ColorBlink(int r, int g, int b, int range[], int rangeLength);
void AlternatingColorBlink(int r1, int g1, int b1, int r2, int g2, int b2, int range[], int rangeLength);
void RainbowSweep(int range[], int rangeLength);
void StaticColor(int r, int g, int b, int range[], int rangeLength);
void IntakeChase(int r, int g, int b, int range[]);

double i = 0;

void setup() {
      FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
      Serial.begin(9600);
}

void loop() {
  // gripper is in: 
  if(digitalRead(INPUT_PIN_1) == HIGH) {
      StaticColor(0, 0, 128, gripLeds, gripLedsLength);
  }
  // gripper is out: 
  else if(digitalRead(INPUT_PIN_1) == LOW) {
      StaticColor(128, 0, 0, gripLeds, gripLedsLength);
  }
  // intake wheels are off:
  if (digitalRead(INPUT_PIN_4) == LOW) {
    //intake is engaged: 
    if (digitalRead(INPUT_PIN_2) == HIGH) {
        StaticColor(128, 0, 0, intkLedsLeft, intkLedsLeftLength);
        StaticColor(128, 0, 0, intkLedsRight, intkLedsRightLength);
    }
    // intake is not engaged: 
    else if (digitalRead(INPUT_PIN_2) == LOW) {
        StaticColor(0, 0, 128, intkLedsLeft, intkLedsLeftLength);
        StaticColor(0, 0, 128, intkLedsRight, intkLedsRightLength);
    }
  }
  //tote intake is in:
  else if (digitalRead(INPUT_PIN_3) == LOW) {
    //intake is engaged: 
    if (digitalRead(INPUT_PIN_2) == HIGH) {
        IntakeChaseIn(128, 0, 0, intkLedsLeft, intkLedsLeftLength);
        IntakeChaseIn(128, 0, 0, intkLedsRight, intkLedsRightLength);
    }
    // intake is not engaged: 
    else if (digitalRead(INPUT_PIN_2) == LOW) {
        IntakeChaseIn(0, 0, 128, intkLedsLeft, intkLedsLeftLength);
        IntakeChaseIn(0, 0, 128, intkLedsRight, intkLedsRightLength);
    }
  }
  //tote intake is out: 
  else if (digitalRead(INPUT_PIN_3) == HIGH) {
    //intake is engaged: 
    if (digitalRead(INPUT_PIN_2) == HIGH) {
        IntakeChaseOut(128, 0, 0, intkLedsLeft, intkLedsLeftLength);
        IntakeChaseOut(128, 0, 0, intkLedsRight, intkLedsRightLength);
    }
    // intake is not engaged: 
    else if (digitalRead(INPUT_PIN_2) == LOW) {
        IntakeChaseOut(0, 0, 128, intkLedsLeft, intkLedsLeftLength);
        IntakeChaseOut(0, 0, 128, intkLedsRight, intkLedsRightLength);
    }
  }
  FastLED.show();
  delay(50);
  
  i+=0.1;
  if (i >= 2.0) {
    i = 0;
  }  
}

void ColorBlink(int r, int g, int b, int range[], int rangeLength) {
  if (i < 1) {
    for (int j=0;j<rangeLength;j++) {
        leds[range[j]] = CRGB(i*r,i*g,i*b);
      }  
  }
  else {
    double ix = 2.0-i;
    for (int j=0;j<rangeLength;j++) {
        leds[range[j]] = CRGB(ix*r,ix*g,ix*b);
      }  
  }
}

void AlternatingColorBlink(int r1, int g1, int b1, int r2, int g2, int b2, int range[], int rangeLength) {
  if (i < 1) {
    for (int j=0;j<=rangeLength;j++) {
        if (j % 2 == 0) {
          leds[range[j]] = CRGB(i*r1,i*g1,i*b1);
        }
        else {
          leds[range[j]] = CRGB(i*r2,i*g2,i*b2);
        }
      }  
  }
  else {
    double ix = 2.0-i;
    for (int j=0;j<rangeLength;j++) {
        if (j % 2 == 0) {
          leds[range[j]] = CRGB(ix*r1,ix*g1,ix*b1);
        }
        else {
          leds[range[j]] = CRGB(ix*r2,ix*g2,ix*b2);
        }
      }  
  }
}

void RainbowSweep(int range[], int rangeLength) {
  for (int j=0;j<rangeLength;j++) {
      leds[range[j]] = CHSV(i*127,255,120);
    }
}

void StaticColor(int r, int g, int b, int range[], int rangeLength) {
  for (int j=0;j<rangeLength;j++) {
      leds[range[j]] = CRGB(r,g,b);
    }
}

void IntakeChaseIn(int r, int g, int b, int range[]) {
  for (int j=0;j<15;j++) {
     leds[range[j]] = CRGB(r,g,b);
     leds[range[j]+15] = CRGB(r,g,b);
     leds[range[j]+30] = CRGB(r,g,b);
     leds[range[j]+45] = CRGB(r,g,b);
  }
}

void IntakeChaseOut(int r, int g, int b, int range[]) {
  for (int j=14;j>=0;j++) {
     leds[range[j]] = CRGB(r,g,b);
     leds[range[j]+15] = CRGB(r,g,b);
     leds[range[j]+30] = CRGB(r,g,b);
     leds[range[j]+45] = CRGB(r,g,b);
  }
}
