#include <FastLED.h>
#include <Wire.h>

#define NUM_LEDS 69
#define DATA_PIN 3
#define I2C_ID 8

CRGB leds[NUM_LEDS];

/**
 * Dependency for the rainbow pattern
 */
uint8_t hue = 0;

/**
 * Dependencies for the arrows patterns
 */
int i = 0;
int a = 0;
int b = 0;
int c = 0;
int d = 0;

/**
 * Dependencies for the intake pattern
 */
int intakePatternCounter = 0;
unsigned long intakePatternDelayStart = 0.0;
int animation[19][9] = {{22, 21, 20, 23, 24, 68, 67, 66, 31}, {21, 20, 19, 24, 25, 67, 66, 65, 31}, {20, 19, 18, 25, 26, 66, 65, 64, 31}, {19, 18, 17, 26, 27, 65, 64, 63, 31}, {18, 17, 16, 27, 28, 64, 63, 62, 31}, {17, 16, 15, 28, 29, 63, 62, 61, 31}, {16, 15, 14, 29, 30, 62, 61, 60, 31}, {17, 16, 15, 28, 29, 63, 62, 61, 30}, {18, 17, 16, 27, 28, 64, 63, 62, 29}, {19, 18, 17, 26, 27, 65, 64, 63, 28}, {20, 19, 18, 25, 26, 66, 65, 64, 27}, {21, 20, 19, 24, 25, 67, 66, 65, 26}, {22, 21, 20, 23, 24, 68, 67, 66, 25}, {22, 21, 20, 23, 24, 68, 67, 66, 26}, {22, 21, 20, 23, 24, 68, 67, 66, 27}, {22, 21, 20, 23, 24, 68, 67, 66, 28}, {22, 21, 20, 23, 24, 68, 67, 66, 29}, {22, 21, 20, 23, 24, 68, 67, 66, 30}, {22, 21, 20, 23, 24, 68, 67, 66, 31}};

/**
 * Dependencies for the arrows patterns
 */
unsigned long arrowsRightDelayStart = 0.0;
unsigned long arrowsLeftDelayStart = 0.0;

/**
 * Dependency for the blink pattern
 */
unsigned long blinkAllDelayStart = 0.0;

/**
 * Dependencies for the chase pattern
 */
unsigned long chaseDelayStart = 0.0;
int chaseCounter = -1;
boolean chaseForward = true;

/**
 * Dependencies for the loading pattern
 */
unsigned long loadingDelayStart = 0.0;
int loadingCounter = -1;
boolean loadingColor = true;

/**
 * Dependency for the cross pattern
 */
unsigned long crossDelayStart = 0.0;

/**
 * Dependency for the police pattern
 */
unsigned long policeDelayStart = 0.0;

/**
 * Dependencies for the leapfrog pattern
 */
unsigned long leapfrogDelayStart = 0.0;
int leapfrogCounter = 0;

/**
 * Dependency for the wave pattern
 */
unsigned long waveDelayStart = 0.0;

/**
 * Dependency for the candy cane pattern
 */
unsigned long candyCaneDelayStart = 0.0;

/** 
 *  For I2C communication
 */
int inputRoboRio = -1;
int toggleRoboRio = 99;
boolean messageReceived = false;
boolean newPattern = false;

///////////////////////////////////////////////////////////////// SETUP //////////////////////////////////////////////////////////////////////
  
  void setup() {

    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2812, 4, GRB>(leds, NUM_LEDS);
    
    Wire.begin(I2C_ID);
    Wire.onReceive(receiveEvent1);
    Serial.begin(9600);

  }

///////////////////////////////////////////////////////////////// PATTERNS /////////////////////////////////////////////////////////////////

/**
 * Turns off all LEDs
 */
void off() {
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  
  FastLED.show();
  
}


/** 
 *  Makes all LEDs the inputted color
 */
void fill(CRGB color) {
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
  
  FastLED.show();
  
}


/**
 * Sets all LEDs to the inputted color without turning the LEDs on
 */
void fill_ns(CRGB color) {
  
  for (i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
  
}


/** 
 * Makes all LEDs blink from the inputted color to black
 * Speed sets the amount of miliseconds it takes the LEDs to switch from the color to off, and from off to the color
 */
void blink_all(CRGB color, float speed) {
  
  bool timeElapsedGreaterThanZero = millis()-blinkAllDelayStart > 0;
  bool timeElapsedLessThanEqualSpeed = millis()-blinkAllDelayStart <= speed;
  bool timeElapsedGreaterThanSpeed = millis()-blinkAllDelayStart > speed;
  bool timeElapsedLessThanEqualTwiceSpeed = millis()-blinkAllDelayStart <= 2*speed;
  
  if(timeElapsedGreaterZero && timeElapsedLessThanEqualSpeed) {
    fill_ns(color);
  }
  else if(timeElapsedGreaterThanSpeed && timeElapsedLessThanEqualTwiceSpeed) {
    off();
  }
  else {
    blinkAllDelayStart = millis();
  }

  FastLED.show();
  
}


/**
 * Makes an LED move, first going right to left and then going left to right
 */
void chase(CRGB color, float speed) {
  
  bool timeElapsedGreaterThanEqualZero = millis()-chaseDelayStart >= 0;
  bool timeElapsedGreaterThanZero = millis()-chaseDelayStart > 0;
  bool timeElapsedLessThanEqualSpeed = millis()-chaseDelayStart <= speed;
  bool timeElapsedGreaterThanSpeed = millis()-chaseDelayStart > speed;
  bool timeElapsedLessThanEqualTwiceSpeed = millis()-chaseDelayStart <= 2*speed;
  
  if(chaseForward) {
    if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed) {
      leds[chaseCounter] = color;
    }
    else if(timeElapsedGreaterThanSpeed && timeElapsedLessThanEqualTwiceSpeed) {
      leds[chaseCounter + 1] = color;
      leds[chaseCounter] = CRGB::Black;
    }
    else {
      chaseDelayStart = millis();
      chaseCounter++;
      
      if(chaseCounter >= NUM_LEDS-1) {
        chaseForward = false;
      }
    }
    
    FastLED.show();
  }
  else {
    if(timeElapsedGreaterThanZero && timeElapsedLessThanEqualSpeed) {
      leds[chaseCounter] = color;
    }
    else if(timeElapsedGreaterThanSpeed && timeElapsedLessThanEqualTwiceSpeed) {
      leds[chaseCounter - 1] = color;
      leds[chaseCounter] = CRGB::Black;
    }
    else {
      chaseDelayStart = millis();
      chaseCounter--;
      
      if(chaseCounter < 1) {
        chaseForward = true;
      }
    }
    
    FastLED.show();
  }
  
}


/**
 * Turns on each light one at a time
 */
void loading(CRGB color, float speed) {
  
  bool timeElapsedGreaterThanEqualZero = millis()-loadingDelayStart >= 0;
  bool timeElapsedLessThanEqualSpeed = millis()-loadingDelayStart <= speed;
  
  if(loadingColor) {
    if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed) {
      leds[loadingCounter] = color;    
    }
    else {
      loadingDelayStart = millis();
      loadingCounter++;
      
      if(loadingCounter > NUM_LEDS-1) {
        loadingColor = false;
        loadingCounter = 0;
      }
    }
    
    FastLED.show();
  }
  else {
    if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed) {
      leds[loadingCounter] = CRGB::Black;    
    }
    else {
      loadingDelayStart = millis();
      loadingCounter++;
      
      if(loadingCounter > NUM_LEDS-1) {
        loadingColor = true;
        loadingCounter = 0;
      }
    }
    
    FastLED.show();
  }
}


/**
 * Makes red, green, and blue arrows scrolling to the right
 * Only works with 3 rows of 69 LEDs
 */
void arrows_right() {
  
  bool timeElapsedGreaterThanEqualNumber = millis()-arrowsRightDelayStart >= 150;
  
  a = 46 + i % 23;
  b = 45 - i % 23;
  c = 0 + i % 23;
  d = 45 - (i + 1) % 23;

  leds[a] = CRGB::Red;
  leds[b] = CRGB::Red;
  leds[c] = CRGB::Red;
  leds[d] = CRGB::Red;

  leds[46 + (i + 3) % 23] = CRGB::Green;
  leds[45 - (i + 3) % 23] = CRGB::Green;
  leds[0 + (i + 3) % 23] = CRGB::Green;
  leds[45 - (i + 4) % 23] = CRGB::Green;

  leds[46 + (i + 6) % 23] = CRGB::Blue;
  leds[45 - (i + 6) % 23] = CRGB::Blue;
  leds[0 + (i + 6) % 23] = CRGB::Blue;
  leds[45 - (i + 7) % 23] = CRGB::Blue;

  delay(30);
  FastLED.show();

  if(timeElapsedGreaterThanEqualNumber) {
    leds[a] = CRGB::Black;
    leds[b] = CRGB::Black;
    leds[c] = CRGB::Black;
    leds[d] = CRGB::Black;

    leds[46 + (i + 3) % 23] = CRGB::Black;
    leds[45 - (i + 3) % 23] = CRGB::Black;
    leds[0 + (i + 3) % 23] = CRGB::Black;
    leds[45 - (i + 4) % 23] = CRGB::Black;

    leds[46 + (i + 6) % 23] = CRGB::Black;
    leds[45 - (i + 6) % 23] = CRGB::Black;
    leds[0 + (i + 6) % 23] = CRGB::Black;
    leds[45 - (i + 7) % 23] = CRGB::Black;

    i = (i + 1) % 23;

    arrowsRightDelayStart = millis();
  }
  
}


/**
 * Makes red, green, and blue arrows scrolling to the left
 * Only works with 3 rows of 69 LEDs
 */
void arrows_left() {
  
  bool timeElapsedGreaterThanEqualNumber = millis()-arrowsLeftDelayStart >= 150;
  
  a = 68 - i % 23;
  b = 23 + i % 23;
  c = 22 - i % 23;
  d = 23 + (i + 1) % 23;

  leds[a] = CRGB::Red;
  leds[b] = CRGB::Red;
  leds[c] = CRGB::Red;
  leds[d] = CRGB::Red;

  leds[68 - (i + 3) % 23] = CRGB::Green;
  leds[23 + (i + 3) % 23] = CRGB::Green;
  leds[22 - (i + 3) % 23] = CRGB::Green;
  leds[23 + (i + 4) % 23] = CRGB::Green;

  leds[68 - (i + 6) % 23] = CRGB::Blue;
  leds[23 + (i + 6) % 23] = CRGB::Blue;
  leds[22 - (i + 6) % 23] = CRGB::Blue;
  leds[23 + (i + 7) % 23] = CRGB::Blue;

  delay(30);
  FastLED.show();
  if(timeElapsedGreaterThanEqualNumber) {
    leds[a] = CRGB::Black;
    leds[b] = CRGB::Black;
    leds[c] = CRGB::Black;
    leds[d] = CRGB::Black;
  
    leds[68 - (i + 3) % 23] = CRGB::Black;
    leds[23 + (i + 3) % 23] = CRGB::Black;
    leds[22 - (i + 3) % 23] = CRGB::Black;
    leds[23 + (i + 4) % 23] = CRGB::Black;
  
    leds[68 - (i + 6) % 23] = CRGB::Black;
    leds[23 + (i + 6) % 23] = CRGB::Black;
    leds[22 - (i + 6) % 23] = CRGB::Black;
    leds[23 + (i + 7) % 23] = CRGB::Black;
  
    i = (i + 1) % 23;
  
    arrowsLeftDelayStart = millis();
  }
  
}


/**
 * Makes the lights change colors to different colors of the rainbow
 */
void rainbow() {

  fill_rainbow(leds, NUM_LEDS, hue, 4);
  delay(30);
  FastLED.show();
  hue++;

}


/**
 * Turns on and then turns off one random LED at a time very quickly, imitating a sparkle
 */
void sparkle() {

  /**
   * Sets i to a random number between zero and the amount of LEDs, to randomize which LED turns on
   */
  int i = random(0, NUM_LEDS - 1);

  leds[i] = CRGB::Red;
  delay(30);
  FastLED.show();
  delay(30);
  leds[i] = CRGB::Black;
  delay(30);
  FastLED.show();
  delay(30);

}


/**
 * Turns on and then turns off one random LED with a randomly assigned color very quickly, imitating confetti
 */
void confetti() {

  /**
   * Sets i to a random number between zero and the amount of LEDs, to randomize which LED turns on
   * 
   * Randomizes whether the color is red, green, or blue, similar to how sparkle randomizes the LED that turns on
   */
  int i = random(0, NUM_LEDS - 1);
  int color = random(0, 3);

  if (color == 0) {
    leds[i] = CRGB::Red;
    delay(30);
    FastLED.show();
    delay(30);
    leds[i] = CRGB::Black;
    delay(30);
    FastLED.show();
    delay(30);
  }
  else if (color == 1) {
    leds[i] = CRGB::Green;
    delay(30);
    FastLED.show();
    delay(30);
    leds[i] = CRGB::Black;
    delay(30);
    FastLED.show();
    delay(30);
  }
    else if (color == 2) {
    leds[i] = CRGB::Blue;
    delay(30);
    FastLED.show();
    delay(30);
    leds[i] = CRGB::Black;
    delay(30);
    FastLED.show();
    delay(30);
  }

}


/**
 * Makes red, blinking crosses with a blinking white background
 * Hardcoded; only works as intended with 3 rows of 69 LEDs
 */
void cross() {
  
  bool timeElapsedGreaterThanZero = millis()-crossDelayStart > 0;
  bool timeElapsedLessThanEqualNumber = millis()-crossDelayStart <= 500;
  bool timeElapsedGreaterThanNumber = millis()-crossDelayStart > 500;
  bool timeElapsedLessThanEqualTwiceNumber = millis()-crossDelayStart <= 1000;
  
  if(timeElapsedGreaterThanZero && timeElapsedLessThanEqualNumber) {
    fill_ns(CRGB::White);
    
    /**
     * The top part of the LEDs
     */
    leds[18] = CRGB::Black;
    leds[26] = CRGB::Black;
    leds[27] = CRGB::Black;
    leds[28] = CRGB::Black;
    leds[64] = CRGB::Black;
    
    /**
     * The angled part of the LEDs
     */
    leds[3] = CRGB::Black;
    leds[7] = CRGB::Black;
    leds[11] = CRGB::Black;
    leds[33] = CRGB::Black;
    leds[34] = CRGB::Black;
    leds[35] = CRGB::Black;
    leds[37] = CRGB::Black;
    leds[38] = CRGB::Black;
    leds[39] = CRGB::Black;
    leds[41] = CRGB::Black;
    leds[42] = CRGB::Black;
    leds[43] = CRGB::Black;
    leds[49] = CRGB::Black;
    leds[53] = CRGB::Black;
    leds[57] = CRGB::Black;

    delay(30);
  }
  else if(timeElapsedGreaterThanNumber && timeElapsedLessThanEqualTwiceNumber) {
    fill_ns(CRGB::Black);

    /**
     * The top part of the LEDs
     */
    leds[18] = CRGB::Red;
    leds[26] = CRGB::Red;
    leds[27] = CRGB::Red;
    leds[28] = CRGB::Red;
    leds[64] = CRGB::Red;

    /**
     * The angled part of the LEDs
     */
    leds[3] = CRGB::Red;
    leds[7] = CRGB::Red;
    leds[11] = CRGB::Red;
    leds[33] = CRGB::Red;
    leds[34] = CRGB::Red;
    leds[35] = CRGB::Red;
    leds[37] = CRGB::Red;
    leds[38] = CRGB::Red;
    leds[39] = CRGB::Red;
    leds[41] = CRGB::Red;
    leds[42] = CRGB::Red;
    leds[43] = CRGB::Red;
    leds[49] = CRGB::Red;
    leds[53] = CRGB::Red;
    leds[57] = CRGB::Red;
    
    delay(30);
  }
  else {
    crossDelayStart = millis();
  }
  
  FastLED.show();

}


/**
 * Makes all LEDs blink from blue to red
 */
void police() {
  
  bool timeElapsedGreaterThanZero = millis()-policeDelayStart > 0;
  bool timeElapsedLessThanEqualNumber = millis()-policeDelayStart <= 500;
  bool timeElapsedGreaterThanNumber = millis()-policeDelayStart > 500;
  bool timeElapsedLessThanEqualTwiceNumber = millis()-policeDelayStart <= 1000;
  
  if(timeElapsedGreaterThanZero && timeElapsedLessThanEqualNumber) {
    fill(CRGB::Blue);
  }
  else if(timeElapsedGreaterThanNumber && timeElapsedLessThanEqualTwiceNumber) {
    fill(CRGB::Red);
  }
  else {
    policeDelayStart = millis();
  }
  
}


/**
 * Makes the lights look like they're playing leapfrog
 */
void leapfrog(CRGB color) {
  
  bool timeElapsedGreaterThanEqualZero = millis()-leapfrogDelayStart >= 0;
  bool timeElapsedLessThanEqualNumber = millis()-leapfrogDelayStart <= 200;
  bool timeElapsedGreaterThanNumber = millis()-leapfrogDelayStart > 200;
  bool timeElapsedLessThanEqualTwiceNumber = millis()-leapfrogDelayStart <= 400;
  
  if(leapfrogCounter < NUM_LEDS) {
    if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualNumber) {
      leds[leapfrogCounter] = color;
      FastLED.show();
    }
    else if(timeElapsedGreaterThanNumber && timeElapsedLessThanEqualTwiceNumber) {
      leds[leapfrogCounter-1] = CRGB::Black;
      FastLED.show();
    }
    else {
      leapfrogDelayStart = millis();
      leapfrogCounter++;
    }
  }
  else {
    leapfrogCounter = 0;
    leds[NUM_LEDS-1] = CRGB::Black;
  }

}


/**
 * Makes a wave motion with the LEDs
 * Hardcoded; only works as intended with 3 rows of 69 LEDs
 */
void wave(CRGB color) {

  /**
   * During the given time interval, the LED will change position
   */
  if(millis()-waveDelayStart > 0 && millis()-waveDelayStart <= 100) {
    leds[22] = CRGB::Black;
    leds[0] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 100 && millis()-waveDelayStart <= 200) {
    leds[0] = CRGB::Black;
    leds[44] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 200 && millis()-waveDelayStart <= 300) {
    leds[44] = CRGB::Black;
    leds[2] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 300 && millis()-waveDelayStart <= 400) {
    leds[2] = CRGB::Black;
    leds[42] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 400 && millis()-waveDelayStart <= 500) {
    leds[42] = CRGB::Black;
    leds[4] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 500 && millis()-waveDelayStart <= 600) {
    leds[4] = CRGB::Black;
    leds[40] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 600 && millis()-waveDelayStart <= 700) {
    leds[40] = CRGB::Black;
    leds[6] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 700 && millis()-waveDelayStart <= 800) {
    leds[6] = CRGB::Black;
    leds[38] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 800 && millis()-waveDelayStart <= 900) {
    leds[38] = CRGB::Black;
    leds[8] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 900 && millis()-waveDelayStart <= 1000) {
    leds[8] = CRGB::Black;
    leds[36] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 1000 && millis()-waveDelayStart <= 1100) {
    leds[36] = CRGB::Black;
    leds[10] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 1100 && millis()-waveDelayStart <= 1200) {
    leds[10] = CRGB::Black;
    leds[34] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 1200 && millis()-waveDelayStart <= 1300) {
    leds[34] = CRGB::Black;
    leds[12] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 1300 && millis()-waveDelayStart <= 1400) {
    leds[12] = CRGB::Black;
    leds[32] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 1400 && millis()-waveDelayStart <= 1500) {
    leds[32] = CRGB::Black;
    leds[14] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 1500 && millis()-waveDelayStart <= 1600) {
    leds[14] = CRGB::Black;
    leds[30] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 1600 && millis()-waveDelayStart <= 1700) {
    leds[30] = CRGB::Black;
    leds[16] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 1700 && millis()-waveDelayStart <= 1800) {
    leds[16] = CRGB::Black;
    leds[28] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 1800 && millis()-waveDelayStart <= 1900) {
    leds[28] = CRGB::Black;
    leds[18] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 1900 && millis()-waveDelayStart <= 2000) {
    leds[18] = CRGB::Black;
    leds[26] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 2000 && millis()-waveDelayStart <= 2100) {
    leds[26] = CRGB::Black;
    leds[20] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 2100 && millis()-waveDelayStart <= 2200) {
    leds[20] = CRGB::Black;
    leds[24] = color;
    delay(30);
    FastLED.show();
  }
  else if(millis()-waveDelayStart > 2200 && millis()-waveDelayStart <= 2300) {
    leds[24] = CRGB::Black;
    leds[22] = color;
    delay(30);
    FastLED.show();
  }
  else {
    waveDelayStart = millis();
  }

}


/**
 * Makes a moving candycane pattern out of two inputted colors
 */
void candy_cane(CRGB color1, CRGB color2) {
  
  bool timeElapsedGreaterThanZero = millis()-candyCaneDelayStart > 0;
  bool timeElapsedLessThanEqualNumber = millis()-candyCaneDelayStart <= 500;
  
  if(timeElapsedGreaterThanZero && timeElapsedLessThanEqualNumber) {
    fill_ns(color1);

    // Causes diagnal lines to turn color2
    for (i = 0; i < NUM_LEDS; i += 2) {
      leds[i] = color2;
    }

    FastLED.show();
  }
  else if(millis()-candyCaneDelayStart > 500 && millis()-candyCaneDelayStart <= 1000) {
    // If the amount of milliseconds that has passed is greater than 500 and less than or equal to 1000, the following code will execute
    fill_ns(color1);

    // Causes diagnal lines to move over one LED
    for (i = 1; i < NUM_LEDS; i += 2) {
      leds[i] = color2;
    }

    FastLED.show();
  }
  else {
    // Otherwise, candyCaneDelayStart is set to the current amount of milliseconds passed
    candyCaneDelayStart = millis();
  }

}



// An animation of a robot picking up and shooting out a cube
void intake_cube(CRGB robotColor) {

  leds[animation[intakePatternCounter][0]] = robotColor;
  leds[animation[intakePatternCounter][1]] = robotColor;
  leds[animation[intakePatternCounter][2]] = robotColor;
  leds[animation[intakePatternCounter][3]] = robotColor;
  leds[animation[intakePatternCounter][4]] = robotColor;
  leds[animation[intakePatternCounter][5]] = robotColor;
  leds[animation[intakePatternCounter][6]] = robotColor;
  leds[animation[intakePatternCounter][7]] = robotColor;
  leds[animation[intakePatternCounter][8]] = CRGB::Yellow;

  delay(30);
  FastLED.show();

  if(millis()-intakePatternDelayStart >= 100) {
    // If the amount of milliseconds that has passed is greater than or equal to 100, the following code will execute
    fill(CRGB::Black);

    intakePatternCounter++;

    if(intakePatternCounter > 18)
      intakePatternCounter = 0;

    intakePatternDelayStart = millis();
  }
  
}


  ////////////////////////////////////////////////////////////////// LOOP /////////////////////////////////////////////////////////////////


  void loop() {
     
    // Causes a pattern to be shown when you type in its assigned number
    if(toggleRoboRio == 1)
      fill(CRGB::Red);
//    else if(toggleRoboRio == "b\n")
//      fill(CRGB::Blue);
//    else if(toggleRoboRio == "g\n")
//      fill(CRGB::Green);
    else if(toggleRoboRio == 99)
        fill(CRGB::Purple);
    else if(toggleRoboRio == 0)
      off();
    else if(toggleRoboRio == 2)
      blink_all(CRGB::Red, 500);
    else if(toggleRoboRio == 3)
      chase(CRGB::Blue, 100);
    else if(toggleRoboRio == 4)
      loading(CRGB::Green, 100);
    else if(toggleRoboRio == 5)
      arrows_right();
    else if(toggleRoboRio == 6)
      arrows_left();
    else if(toggleRoboRio == 7)
      rainbow();
    else if(toggleRoboRio == 8)
      sparkle();
    else if(toggleRoboRio == 9)
      confetti();
    else if(toggleRoboRio == 10)
      cross();
    else if(toggleRoboRio == 11)
      police();
    else if(toggleRoboRio == 12)
      leapfrog(CRGB::Green);
    else if(toggleRoboRio == 13)
      wave(CRGB::Blue);
    else if(toggleRoboRio == 14)
      candy_cane(CRGB::White, CRGB::Red);
    else if(toggleRoboRio == 15)
      intake_cube(CRGB::Red);

    // Causes text to be printed when you select a pattern to be shown
    if(messageReceived) {
      // If a string is enterred, the following code will execute
      if(toggleRoboRio != inputRoboRio) {
        // If toggleRoboRio does not match inputRoboRio, a new pattern is being selected, so newPattern is set to true
        toggleRoboRio = inputRoboRio;
        newPattern = true;
      }
//      if(toggleRoboRio == 1)
//        Serial.println("Arduino: Okay, now I'm red!");
//      else if(toggleRoboRio == "b\n")
//        Serial.println("Arduino: Okay, now I'm blue!");
//      else if(toggleRoboRio == "g\n")
//        Serial.println("Arduino: Okay, now I'm green!");
//      else if(toggleRoboRio == 0)
//        Serial.println("Arduino: Okay, now I'm off!");
//      else if(toggleRoboRio == 2)
//        Serial.println("Arduino: Okay, I'm blinking now!");
//      else if(toggleRoboRio == 3)
//        Serial.println("Arduino: Okay, I'll show the pattern 'chase' now!");
//      else if(toggleRoboRio == 4)
//        Serial.println("Arduino: Okay, I'll show the pattern 'loading' now!");
//      else if(toggleRoboRio == 5)
//        Serial.println("Arduino: Okay, now I'll show arrows!");
//      else if(toggleRoboRio == 6)
//        Serial.println("Arduino: Okay, now I'll show arrows!");
//      else if(toggleRoboRio == 7)
//        Serial.println("Arduino: Okay, now I'm rainbow-colored!");
//      else if(toggleRoboRio == 8)
//        Serial.println("Arduino: Okay, now I'm sparkly!");
//      else if(toggleRoboRio == 9)
//        Serial.println("Arduino: Okay, I'll show the pattern 'confetti' now!");
//      else if(toggleRoboRio == 10)
//        Serial.println("Arduino: Okay, I'll show the pattern 'cross' now!");
//      else if(toggleRoboRio == 11)
//        Serial.println("Arduino: Okay, I'll show the pattern 'police' now!");
//      else if(toggleRoboRio == 12)
//        Serial.println("Arduino: Okay, I'll show the pattern 'leapfrog' now!");
//      else if(toggleRoboRio == 13)
//        Serial.println("Arduino: Okay, I'll show the pattern 'wave' now!");
//      else if(toggleRoboRio == 14)
//        Serial.println("Arduino: Okay, I'll show the pattern 'candy_cane' now!");
//      else if(toggleRoboRio == 15)
//        Serial.println("Arduino: Okay, I'll show the pattern 'intake_cube' now!");
        

      inputRoboRio = -1;
      messageReceived = false;
    }
    
    
    if(newPattern) {
      // If the pattern is being changed, all LEDs are briefly turned off and some variables are reset
      off();
      if(toggleRoboRio == 4) {
        loadingCounter = -1;
        loadingColor = true;
      }
      newPattern = false;
    }
    
  }



//  void serialEvent() {
//    while(Serial.available()) {
//      char inChar = (char)Serial.read();
//      inputRoboRio += inChar;
//
//      if(inChar == '\n') {
//        messageReceived = true;
//      }
//    }
//  }


void receiveEvent1(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    //Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  inputRoboRio = x;
  messageReceived = true;
  Serial.println(x);         // print the integer
}

