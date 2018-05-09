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
unsigned long arrowsRightDelayStart = 0.0;
unsigned long arrowsLeftDelayStart = 0.0;

/**
 * Dependencies for the intake pattern
 */
int intakePatternCounter = 0;
unsigned long intakePatternDelayStart = 0.0;
int animation[19][9] = {{22, 21, 20, 23, 24, 68, 67, 66, 31}, {21, 20, 19, 24, 25, 67, 66, 65, 31}, {20, 19, 18, 25, 26, 66, 65, 64, 31}, {19, 18, 17, 26, 27, 65, 64, 63, 31}, {18, 17, 16, 27, 28, 64, 63, 62, 31}, {17, 16, 15, 28, 29, 63, 62, 61, 31}, {16, 15, 14, 29, 30, 62, 61, 60, 31}, {17, 16, 15, 28, 29, 63, 62, 61, 30}, {18, 17, 16, 27, 28, 64, 63, 62, 29}, {19, 18, 17, 26, 27, 65, 64, 63, 28}, {20, 19, 18, 25, 26, 66, 65, 64, 27}, {21, 20, 19, 24, 25, 67, 66, 65, 26}, {22, 21, 20, 23, 24, 68, 67, 66, 25}, {22, 21, 20, 23, 24, 68, 67, 66, 26}, {22, 21, 20, 23, 24, 68, 67, 66, 27}, {22, 21, 20, 23, 24, 68, 67, 66, 28}, {22, 21, 20, 23, 24, 68, 67, 66, 29}, {22, 21, 20, 23, 24, 68, 67, 66, 30}, {22, 21, 20, 23, 24, 68, 67, 66, 31}};

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
  
  if(timeElapsedGreaterThanZero && timeElapsedLessThanEqualSpeed) {
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

  int number = 150;
  bool timeElapsedGreaterThanEqualNumber = millis()-arrowsRightDelayStart >= number;
  
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

  int number = 150;
  bool timeElapsedGreaterThanEqualNumber = millis()-arrowsLeftDelayStart >= number;
  
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

  int number = 500;
  bool timeElapsedGreaterThanZero = millis()-crossDelayStart > 0;
  bool timeElapsedLessThanEqualNumber = millis()-crossDelayStart <= number;
  bool timeElapsedGreaterThanNumber = millis()-crossDelayStart > number;
  bool timeElapsedLessThanEqualTwiceNumber = millis()-crossDelayStart <= 2*number;
  
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

  int number = 500;
  bool timeElapsedGreaterThanZero = millis()-policeDelayStart > 0;
  bool timeElapsedLessThanEqualNumber = millis()-policeDelayStart <= number;
  bool timeElapsedGreaterThanNumber = millis()-policeDelayStart > number;
  bool timeElapsedLessThanEqualTwiceNumber = millis()-policeDelayStart <= 2*number;
  
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

  int number = 200;
  bool timeElapsedGreaterThanEqualZero = millis()-leapfrogDelayStart >= 0;
  bool timeElapsedLessThanEqualNumber = millis()-leapfrogDelayStart <= number;
  bool timeElapsedGreaterThanNumber = millis()-leapfrogDelayStart > number;
  bool timeElapsedLessThanEqualTwiceNumber = millis()-leapfrogDelayStart <= 2*number;
  
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
  
  int number = 100;
  bool timeElapsedGreaterThanZero = millis()-waveDelayStart > 0;
  bool timeElapsedLessThanEqualNumber = millis()-waveDelayStart <= number;
  bool timeElapsedGreaterThanNumber = millis()-waveDelayStart > number;
  bool timeElapsedLessThanEqualTwiceNumber = millis()-waveDelayStart <= 2*number;
  bool timeElapsedGreaterThanTwiceNumber = millis()-waveDelayStart > 2*number;
  bool timeElapsedLessThanEqualThreeTimesNumber = millis()-waveDelayStart <= 3*number;
  bool timeElapsedGreaterThanThreeTimesNumber = millis()-waveDelayStart > 3*number;
  bool timeElapsedLessThanEqualFourTimesNumber = millis()-waveDelayStart <= 4*number;
  bool timeElapsedGreaterThanFourTimesNumber = millis()-waveDelayStart > 4*number;
  bool timeElapsedLessThanEqualFiveTimesNumber = millis()-waveDelayStart <= 5*number;
  bool timeElapsedGreaterThanFiveTimesNumber = millis()-waveDelayStart > 5*number;
  bool timeElapsedLessThanEqualSixTimesNumber = millis()-waveDelayStart <= 6*number;
  bool timeElapsedGreaterThanSixTimesNumber = millis()-waveDelayStart > 6*number;
  bool timeElapsedLessThanEqualSevenTimesNumber = millis()-waveDelayStart <= 7*number;
  bool timeElapsedGreaterThanSevenTimesNumber = millis()-waveDelayStart > 7*number;
  bool timeElapsedLessThanEqualEightTimesNumber = millis()-waveDelayStart <= 8*number;
  bool timeElapsedGreaterThanEightTimesNumber = millis()-waveDelayStart > 8*number;
  bool timeElapsedLessThanEqualNineTimesNumber = millis()-waveDelayStart <= 9*number;
  bool timeElapsedGreaterThanNineTimesNumber = millis()-waveDelayStart > 9*number;
  bool timeElapsedLessThanEqualTenTimesNumber = millis()-waveDelayStart <= 10*number;
  bool timeElapsedGreaterThanTenTimesNumber = millis()-waveDelayStart > 10*number;
  bool timeElapsedLessThanEqualElevenTimesNumber = millis()-waveDelayStart <= 11*number;
  bool timeElapsedGreaterThanElevenTimesNumber = millis()-waveDelayStart > 11*number;
  bool timeElapsedLessThanEqualTwelveTimesNumber = millis()-waveDelayStart <= 12*number;
  bool timeElapsedGreaterThanTwelveTimesNumber = millis()-waveDelayStart > 12*number;
  bool timeElapsedLessThanEqualThirteenTimesNumber = millis()-waveDelayStart <= 13*number;
  bool timeElapsedGreaterThanThirteenTimesNumber = millis()-waveDelayStart > 13*number;
  bool timeElapsedLessThanEqualFourteenTimesNumber = millis()-waveDelayStart <= 14*number;
  bool timeElapsedGreaterThanFourteenTimesNumber = millis()-waveDelayStart > 14*number;
  bool timeElapsedLessThanEqualFifteenTimesNumber = millis()-waveDelayStart <= 15*number;
  bool timeElapsedGreaterThanFifteenTimesNumber = millis()-waveDelayStart > 15*number;
  bool timeElapsedLessThanEqualSixteenTimesNumber = millis()-waveDelayStart <= 16*number;
  bool timeElapsedGreaterThanSixteenTimesNumber = millis()-waveDelayStart > 16*number;
  bool timeElapsedLessThanEqualSeventeenTimesNumber = millis()-waveDelayStart <= 17*number;
  bool timeElapsedGreaterThanSeventeenTimesNumber = millis()-waveDelayStart > 17*number;
  bool timeElapsedLessThanEqualEighteenTimesNumber = millis()-waveDelayStart <= 18*number;
  bool timeElapsedGreaterThanEighteenTimesNumber = millis()-waveDelayStart > 18*number;
  bool timeElapsedLessThanEqualNineteenTimesNumber = millis()-waveDelayStart <= 19*number;
  bool timeElapsedGreaterThanNineteenTimesNumber = millis()-waveDelayStart > 19*number;
  bool timeElapsedLessThanEqualTwentyTimesNumber = millis()-waveDelayStart <= 20*number;
  bool timeElapsedGreaterThanTwentyTimesNumber = millis()-waveDelayStart > 20*number;
  bool timeElapsedLessThanEqualTwentyoneTimesNumber = millis()-waveDelayStart <= 21*number;
  bool timeElapsedGreaterThanTwentyoneTimesNumber = millis()-waveDelayStart > 21*number;
  bool timeElapsedLessThanEqualTwentytwoTimesNumber = millis()-waveDelayStart <= 22*number;
  bool timeElapsedGreaterThanTwentytwoTimesNumber = millis()-waveDelayStart > 22*number;
  bool timeElapsedLessThanEqualTwentythreeTimesNumber = millis()-waveDelayStart <= 23*number;
  
  if(timeElapsedGreaterThanZero && timeElapsedLessThanEqualNumber) {
    leds[22] = CRGB::Black;
    leds[0] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanNumber && timeElapsedLessThanEqualTwiceNumber) {
    leds[0] = CRGB::Black;
    leds[44] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanTwiceNumber && timeElapsedLessThanEqualThreeTimesNumber) {
    leds[44] = CRGB::Black;
    leds[2] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanThreeTimesNumber && timeElapsedLessThanEqualFourTimesNumber) {
    leds[2] = CRGB::Black;
    leds[42] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanFourTimesNumber && timeElapsedLessThanEqualFiveTimesNumber) {
    leds[42] = CRGB::Black;
    leds[4] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanFiveTimesNumber && timeElapsedLessThanEqualSixTimesNumber) {
    leds[4] = CRGB::Black;
    leds[40] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanSixTimesNumber && timeElapsedLessThanEqualSevenTimesNumber) {
    leds[40] = CRGB::Black;
    leds[6] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanSevenTimesNumber && timeElapsedLessThanEqualEightTimesNumber) {
    leds[6] = CRGB::Black;
    leds[38] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanEightTimesNumber && timeElapsedLessThanEqualNineTimesNumber) {
    leds[38] = CRGB::Black;
    leds[8] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanNineTimesNumber && timeElapsedLessThanEqualTenTimesNumber) {
    leds[8] = CRGB::Black;
    leds[36] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanTenTimesNumber && timeElapsedLessThanEqualElevenTimesNumber) {
    leds[36] = CRGB::Black;
    leds[10] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanElevenTimesNumber && timeElapsedLessThanEqualTwelveTimesNumber) {
    leds[10] = CRGB::Black;
    leds[34] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanTwelveTimesNumber && timeElapsedLessThanEqualThirteenTimesNumber) {
    leds[34] = CRGB::Black;
    leds[12] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanThirteenTimesNumber && timeElapsedLessThanEqualFourteenTimesNumber) {
    leds[12] = CRGB::Black;
    leds[32] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanFourteenTimesNumber && timeElapsedLessThanEqualFifteenTimesNumber) {
    leds[32] = CRGB::Black;
    leds[14] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanFifteenTimesNumber && timeElapsedLessThanEqualSixteenTimesNumber) {
    leds[14] = CRGB::Black;
    leds[30] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanSixteenTimesNumber && timeElapsedLessThanEqualSeventeenTimesNumber) {
    leds[30] = CRGB::Black;
    leds[16] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanSeventeenTimesNumber && timeElapsedLessThanEqualEighteenTimesNumber) {
    leds[16] = CRGB::Black;
    leds[28] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanEighteenTimesNumber && timeElapsedLessThanEqualNineteenTimesNumber) {
    leds[28] = CRGB::Black;
    leds[18] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanNineteenTimesNumber && timeElapsedLessThanEqualTwentyTimesNumber) {
    leds[18] = CRGB::Black;
    leds[26] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanTwentyTimesNumber && timeElapsedLessThanEqualTwentyoneTimesNumber) {
    leds[26] = CRGB::Black;
    leds[20] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanTwentyoneTimesNumber && timeElapsedLessThanEqualTwentytwoTimesNumber) {
    leds[20] = CRGB::Black;
    leds[24] = color;
    delay(30);
    FastLED.show();
  }
  else if(timeElapsedGreaterThanTwentytwoTimesNumber && timeElapsedLessThanEqualTwentythreeTimesNumber) {
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

  int number = 500;
  bool timeElapsedGreaterThanZero = millis()-candyCaneDelayStart > 0;
  bool timeElapsedLessThanEqualNumber = millis()-candyCaneDelayStart <= number;
  bool timeElapsedGreaterThanNumber = millis()-candyCaneDelayStart > number;
  bool timeElapsedLessThanEqualTwiceNumber = millis()-candyCaneDelayStart <= 2*number;
  
  if(timeElapsedGreaterThanZero && timeElapsedLessThanEqualNumber) {
    fill_ns(color1);
    
    for (i = 0; i < NUM_LEDS; i += 2) {
      leds[i] = color2;
    }

    FastLED.show();
  }
  else if(timeElapsedGreaterThanNumber && timeElapsedLessThanEqualTwiceNumber) {
    fill_ns(color1);

    for (i = 1; i < NUM_LEDS; i += 2) {
      leds[i] = color2;
    }

    FastLED.show();
  }
  else {
    candyCaneDelayStart = millis();
  }

}


/**
 * An animation of a robot picking up and shooting out a cube
 */
void intake_cube(CRGB robotColor) {

  int number = 100;
  bool timeElapsedGreaterThanEqualNumber = millis()-intakePatternDelayStart >= number;
  
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
  
  if(timeElapsedGreaterThanEqualNumber) {
    fill(CRGB::Black);
    intakePatternCounter++;

    if(intakePatternCounter > 18)
      intakePatternCounter = 0;

    intakePatternDelayStart = millis();
  }
  
}

////////////////////////////////////////////////////////////////// LOOP /////////////////////////////////////////////////////////////////

void loop() {
  
  if(toggleRoboRio == 1)
    fill(CRGB::Red);
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
  
  if(messageReceived) {
    /**
     * Makes the variable newPattern true when the pattern is being changed
     */
    if(toggleRoboRio != inputRoboRio) {
      toggleRoboRio = inputRoboRio;
      newPattern = true;
    }

    inputRoboRio = -1;
    messageReceived = false;
  }
  
  /**
   * If the pattern is being changed, all LEDs are briefly turned off and some variables are reset
   */
  if(newPattern) {
    off();
    if(toggleRoboRio == 4) {
      loadingCounter = -1;
      loadingColor = true;
    }
    newPattern = false;
  }
  
}


/**
 * Sets the variable inputRoboRio to the value the Arduino received from the RoboRio
 */
void receiveEvent1(int howMany) {
  while (1 < Wire.available()) {
    char c = Wire.read();
  }
  int x = Wire.read();
  inputRoboRio = x;
  messageReceived = true;
  Serial.println(x);
}
