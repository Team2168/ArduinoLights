#include <FastLED.h>
#include <Wire.h>

#define NUM_LEDS 69
#define DATA_PIN 3
#define I2C_ID 8

// lightStates[0] is red1, [1] is g1, [2] is b1, [3] is r2, [4] is g2, [5] is b2, [6] is pattern, and [7] is speed
byte lightStates[8] = {0, 0, 255, 255, 0, 0, 5, 300};

int animation[19][9] = {{22, 21, 20, 23, 24, 68, 67, 66, 31}, {21, 20, 19, 24, 25, 67, 66, 65, 31}, {20, 19, 18, 25, 26, 66, 65, 64, 31}, {19, 18, 17, 26, 27, 65, 64, 63, 31}, {18, 17, 16, 27, 28, 64, 63, 62, 31}, {17, 16, 15, 28, 29, 63, 62, 61, 31}, {16, 15, 14, 29, 30, 62, 61, 60, 31}, {17, 16, 15, 28, 29, 63, 62, 61, 30}, {18, 17, 16, 27, 28, 64, 63, 62, 29}, {19, 18, 17, 26, 27, 65, 64, 63, 28}, {20, 19, 18, 25, 26, 66, 65, 64, 27}, {21, 20, 19, 24, 25, 67, 66, 65, 26}, {22, 21, 20, 23, 24, 68, 67, 66, 25}, {22, 21, 20, 23, 24, 68, 67, 66, 26}, {22, 21, 20, 23, 24, 68, 67, 66, 27}, {22, 21, 20, 23, 24, 68, 67, 66, 28}, {22, 21, 20, 23, 24, 68, 67, 66, 29}, {22, 21, 20, 23, 24, 68, 67, 66, 30}, {22, 21, 20, 23, 24, 68, 67, 66, 31}};

//#define OFF_PATTERN_ID 0
//#define FILL_PATTERN_ID 1
//#define BLINK_PATTERN_ID 2
//#define CHASE_PATTERN_ID 3
//#define LOADING_PATTERN_ID 4
//#define ARROWS_RIGHT_PATTERN_ID 5
//#define ARROWS_LEFT_PATTERN_ID 6
//#define RAINBOW_PATTERN_ID 7
//#define SPARKLE_PATTERN_ID 8
//#define CONFETTI_PATTERN_ID 9
//#define CROSS_PATTERN_ID 10
//#define POLICE_PATTERN_ID 11
//#define LEAPFROG_PATTERN_ID 12 
//#define WAVE_PATTERN_ID 13
//#define CANDY_CANE_PATTERN_ID 14
//#define INTAKE_CUBE_PATTERN_ID 15

  CRGB leds[NUM_LEDS];
  uint8_t hue = 0;
  int i = 0;
  int a = 0;
  int b = 0;
  int c = 0;
  int d = 0;
  int intakePatternCounter = 0;
  unsigned long intakePatternDelayStart = 0.0;
  unsigned long arrowsRightDelayStart = 0.0;
  unsigned long arrowsLeftDelayStart = 0.0;
  unsigned long blinkAllDelayStart = 0.0;
  unsigned long chaseDelayStart = 0.0;
  int chaseCounter = -1;
  boolean chaseForward = true;
  unsigned long loadingDelayStart = 0.0;
  int loadingCounter = -1;
  boolean loadingColor = true;
  unsigned long crossDelayStart = 0.0;
  unsigned long policeDelayStart = 0.0;
  unsigned long leapfrogDelayStart = 0.0;
  int leapfrogCounter = 0;
  unsigned long waveDelayStart = 0.0;
  unsigned long candyCaneDelayStart = 0.0;

  // For I2C communication
  int inputRoboRio = -1;
  int toggleRoboRio = 99;
  boolean messageReceived = false;
  boolean newPattern = false;


//////////////////////////////////////// SETUP /////////////////////////////////////////////

  
  void setup() {

    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2812, 4, GRB>(leds, NUM_LEDS);

//    Serial.begin(9600);
//    inputRoboRio.reserve(200);
    
    
    Wire.begin(I2C_ID);
    Wire.onReceive(receiveEvent1);
    Serial.begin(9600);

    //FastLED.setBrightness(20);

    delay(30);

  }


  /**
   *
  **/


//  void makePatterns(int r1, int g1, int b1, int r2, int g2, int b2, int pattern, int speed) {
//
//    switch (pattern) {
//      case FILL_PATTERN_ID:
//        fill(CRGB(r1, g1, b1));
//        break;
//      case BLINK_PATTERN_ID:
//        blink_all(CRGB(r1, g1, b1), speed);
//        break;
//      case CHASE_PATTERN_ID:
//        chase(CRGB(r1, g1, b1), speed);
//        break;
//      case LOADING_PATTERN_ID:
//        loading(CRGB(r1, g1, b1), speed);
//        break;
//      case ARROWS_RIGHT_PATTERN_ID:
//        arrows_right();
//        break;
//      case ARROWS_LEFT_PATTERN_ID:
//        arrows_left();
//        break;
//      case RAINBOW_PATTERN_ID:
//        rainbow();
//        break;
//      case SPARKLE_PATTERN_ID:
//        sparkle();
//        break;
//      case CONFETTI_PATTERN_ID:
//        confetti();
//        break;
//      case CROSS_PATTERN_ID:
//        cross();
//        break;
//      case POLICE_PATTERN_ID:
//        police();
//        break;
//      case LEAPFROG_PATTERN_ID:
//        leapfrog(CRGB(r1, g1, b1));
//        break;
//      case WAVE_PATTERN_ID:
//        wave(CRGB(r1, g1, b1));
//        break;
//      case CANDY_CANE_PATTERN_ID:
//        candy_cane(CRGB(r1, g1, b1), CRGB(r2, g2, b2));
//        break;
//       case INTAKE_CUBE_PATTERN_ID:
//        intake_cube(CRGB(r1, g1, b1));
//        break;
//
//      default: //OFF_PATTERN_ID:
//        off();
//        break;
//    }
//
//  }


  //////////////////////////////////////// PATTERNS /////////////////////////////////////////////


  // Turns off all LEDs
  void off() {

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }

    FastLED.show();

  }


  // Makes all LEDs the inputted color
  void fill(CRGB color) {

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = color;
    }

    FastLED.show();

  }



  // Sets all LEDs to the inputted color without turning the LEDs on
  void fill_ns(CRGB color) {

    for (i = 0; i < NUM_LEDS; i++) {
      leds[i] = color;
    }

  }



  // Makes all LEDs blink from the inputted color to black
  // Speed sets the amount of miliseconds it takes the LEDs to switch from the color to off, and from off to the color
  void blink_all(CRGB color, float speed) {

    // This if statement is used in place of a delay so that the Arduino does not stop receiving messages;
    // that would make it impossible to quickly switch between patterns
    if(millis()-blinkAllDelayStart > 0 && millis()-blinkAllDelayStart <= speed) {
      // If the amount of milliseconds that has passed is above 0 and less than or equal to speed, all LEDs will be set to color
      fill_ns(color);
    }
    else if(millis()-blinkAllDelayStart > speed && millis()-blinkAllDelayStart <= 2*speed) {
      // If the amount of milliseconds that has passed is above speed and less than or equal to 2 * speed, all LEDs will be set to off
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
      }
    }
    else {
      // Otherwise, blinkAllDelayStart will be set to the current amount of milliseconds passed
      blinkAllDelayStart = millis();
    }

    // In every case, the LEDs will be turned on
    FastLED.show();
    
  }



  // Makes the lights turn on and then off one at a time, first going right and then going left
  // Speed sets the amount of miliseconds it takes the LEDs to switch from the color to off and how long it takes the next LED to turn on
  void chase(CRGB color, float speed) {

    if(chaseForward) {
      if(millis()-chaseDelayStart >= 0 && millis()-chaseDelayStart <= speed) {
        // If the amount of milliseconds that has passed is greater than or equal to 0 and less than or equal to speed, an LED will be set to color
        leds[chaseCounter] = color;    
      }
      else if(millis()-chaseDelayStart > speed && millis()-chaseDelayStart <= 2*speed) {
        // If the amount of milliseconds that has passed is greater than speed and less than or equal to 2 * speed, an LED will be set to color
        // and an LED will be set to off
        leds[chaseCounter + 1] = color;
        leds[chaseCounter] = CRGB::Black;
      }
      else {
        // Otherwise, the following code will execute
        chaseDelayStart = millis();
        chaseCounter++;
        
        if(chaseCounter >= NUM_LEDS-1) {
          chaseForward = false;
        }
      }
      // In every case, the LEDs will turn on
      FastLED.show();
    }
    else {
      if(millis()-chaseDelayStart > 0 && millis()-chaseDelayStart <= speed) {
        // If the amount of milliseconds that has passed is greater than 0 and less than or equal to speed, an LED will be set to color
        leds[chaseCounter] = color;
      }
      else if(millis()-chaseDelayStart > speed && millis()-chaseDelayStart <= 2*speed) {
        // If the amount of milliseconds that has passed is greater than speed and less than or equal to 2 * speed, an LED will be set to color
        // and an LED will be set to off
        leds[chaseCounter - 1] = color;
        leds[chaseCounter] = CRGB::Black;
      }
      else {
        // Otherwise, the following code will execute
        chaseDelayStart = millis();
        chaseCounter--;
        
        if(chaseCounter < 1) {
          chaseForward = true;
        }
      }
      // In every case, the LEDs will turn on
      FastLED.show();
    }
  }



  // Turns on each light one at a time
  // Speed sets the amount of miliseconds it takes the LEDs to turn on
  void loading(CRGB color, float speed) {

    if(loadingColor) {
      if(millis()-loadingDelayStart >= 0 && millis()-loadingDelayStart <= speed) {
        // If the amount of milliseconds that has passed is greater than or equal to 0 and less than or equal to speed, an LED will be set to color
        leds[loadingCounter] = color;    
      }
      else {
        // Otherwise, the following code will execute
        loadingDelayStart = millis();
        loadingCounter++;
        
        if(loadingCounter > NUM_LEDS-1) {
          loadingColor = false;
          loadingCounter = 0;
        }
      }
      // In every case, the LEDs will turn on
      FastLED.show();
  }
  else {
    if(millis()-loadingDelayStart >= 0 && millis()-loadingDelayStart <= speed) {
      // If the amount of milliseconds that has passed is greater than or equal to 0 and less than or equal to speed, an LED will be set to off
      leds[loadingCounter] = CRGB::Black;    
    }
    else {
      // Otherwise, the following code will execute
      loadingDelayStart = millis();
      loadingCounter++;
      
      if(loadingCounter > NUM_LEDS-1) {
        loadingColor = true;
        loadingCounter = 0;
      }
    }
    // In every case, the LEDs will turn on
    FastLED.show();
  }
 }



  // Makes red, green, and blue arrows scrolling to the right
  // Only works with 3 rows of 23 LEDs
  void arrows_right() {

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

    if(millis()-arrowsRightDelayStart >= 150) {
      // If the amount of milliseconds that has passed is greater than or equal to 150, the following code will execute
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



  // Makes red, green, and blue arrows scrolling to the left
  // Only works with 3 rows of 23 LEDs
  void arrows_left() {

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
    if(millis()-arrowsLeftDelayStart >= 150) {
    // If the amount of milliseconds that has passed is greater than or equal to 150, the following code will execute
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



  // Makes the lights change colors to different colors of the rainbow
  void rainbow() {

    fill_rainbow(leds, NUM_LEDS, hue, 4);

    delay(30);

    FastLED.show();

    hue++;

  }



  // Turns on and then turns off random LEDs
  void sparkle() {

    // Sets i to a random number between zero and NUM_LEDS minus one
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



  // Turns on and then turns off random LEDs with a randomly assigned color
  void confetti() {

    // Sets i to a random number between zero and NUM_LEDS minus one
    // Sets color to a random number between zero and two

    int i = random(0, NUM_LEDS - 1);
    int color = random(0, 3);

    // If color equals 0, the LED will be red
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

    // If color equals 1, the LED will be green
    if (color == 1) {
      leds[i] = CRGB::Green;
      delay(30);
      FastLED.show();
      delay(30);
      leds[i] = CRGB::Black;
      delay(30);
      FastLED.show();
      delay(30);
    }

    // If color equals 2, the LED will be blue
    if (color == 2) {
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



  // Makes red, blinking crosses with a blinking white background
  // Hardcoded; only works as intended with 3 rows of 23 LEDs
  void cross() {

    if(millis()-crossDelayStart>0 && millis()-crossDelayStart<=500) {
      // If the amount of milliseconds that has passed is greater than 0 and less than or equal to 500, the following code will execute
      fill_ns(CRGB::White);

      // The top part of the LEDs
      leds[18] = CRGB::Black;
      leds[26] = CRGB::Black;
      leds[27] = CRGB::Black;
      leds[28] = CRGB::Black;
      leds[64] = CRGB::Black;

      // The angled part of the LEDs
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
    else if(millis()-crossDelayStart>500 && millis()-crossDelayStart<=1000) {
      // If the amount of seconds that has passed is greater than 500 and less than or equal to 1000, the following code will execute
      fill_ns(CRGB::Black);

      // The top part of the LEDs
      leds[18] = CRGB::Red;
      leds[26] = CRGB::Red;
      leds[27] = CRGB::Red;
      leds[28] = CRGB::Red;
      leds[64] = CRGB::Red;

      // The angled part of the LEDs
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
      // Otherwise, crossDelayStart will be set to the current amount of milliseconds passed
      crossDelayStart = millis();
    }

    // In every case, the LEDs will turn on
    FastLED.show();

  }



  // Makes all LEDs blink from blue to red
  void police() {

    if(millis()-policeDelayStart > 0 && millis()-policeDelayStart <= 500) {
      // If the amount of milliseconds that has passed is greater than 0 and less than or equal to 500, all LEDs will become blue
      fill(CRGB::Blue);
    }
    else if(millis()-policeDelayStart > 500 && millis()-policeDelayStart <= 1000) {
      // If the amount of milliseconds that has passed is greater than 500 and less than or equal to 1000, all LEDs will become red
      fill(CRGB::Red);
    }
    else {
      // Otherwise, policeDelayStart will be set to the current amount of milliseconds passed
      policeDelayStart = millis();
    }
    
  }



  // Makes the lights look like they're playing leapfrog
  void leapfrog(CRGB color) {
    
    if(leapfrogCounter < NUM_LEDS) {

      if(millis()-leapfrogDelayStart >= 0 && millis()-leapfrogDelayStart <= 200) {
        // If the amount of milliseconds that has passed is greater than or equal to 0 and less than or equal to 200, an LED will become color
        leds[leapfrogCounter] = color;
        FastLED.show();
      }
      else if(millis()-leapfrogDelayStart > 200 && millis()-leapfrogDelayStart <= 400) {
        // If the amount of milliseconds that has passed is greater than 200 and less than or equal to 400, an LED will turn off
        leds[leapfrogCounter-1] = CRGB::Black;
        FastLED.show();
      }
      else {
        // Otherwise, the following code will be executed
        leapfrogDelayStart = millis();
        leapfrogCounter++;
      }
    }
    else {
      leapfrogCounter = 0;
      leds[NUM_LEDS-1] = CRGB::Black;
    }

  }



  // Makes a wave motion with the LEDs
  // Hardcoded; only works as intended with 3 rows of 23 LEDs
  void wave(CRGB color) {
    
    if(millis()-waveDelayStart > 0 && millis()-waveDelayStart <= 100) {
      // If the amount of milliseconds that has passed is greater than 0 and less than or equal to 100, the following code will execute
      leds[22] = CRGB::Black;
      leds[0] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 100 && millis()-waveDelayStart <= 200) {
      // If the amount of milliseconds that has passed is greater than 100 and less than or equal to 200, the following code will execute
      leds[0] = CRGB::Black;
      leds[44] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 200 && millis()-waveDelayStart <= 300) {
      // If the amount of milliseconds that has passed is greater than 200 and less than or equal to 300, the following code will execute
      leds[44] = CRGB::Black;
      leds[2] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 300 && millis()-waveDelayStart <= 400) {
      // If the amount of milliseconds that has passed is greater than 300 and less than or equal to 400, the following code will execute
      leds[2] = CRGB::Black;
      leds[42] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 400 && millis()-waveDelayStart <= 500) {
      // If the amount of milliseconds that has passed is greater than 400 and less than or equal to 500, the following code will execute
      leds[42] = CRGB::Black;
      leds[4] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 500 && millis()-waveDelayStart <= 600) {
      // If the amount of milliseconds that has passed is greater than 500 and less than or equal to 600, the following code will execute
      leds[4] = CRGB::Black;
      leds[40] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 600 && millis()-waveDelayStart <= 700) {
      // If the amount of milliseconds that has passed is greater than 600 and less than or equal to 700, the following code will execute
      leds[40] = CRGB::Black;
      leds[6] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 700 && millis()-waveDelayStart <= 800) {
      // If the amount of milliseconds that has passed is greater than 700 and less than or equal to 800, the following code will execute
      leds[6] = CRGB::Black;
      leds[38] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 800 && millis()-waveDelayStart <= 900) {
      // If the amount of milliseconds that has passed is greater than 800 and less than or equal to 900, the following code will execute
      leds[38] = CRGB::Black;
      leds[8] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 900 && millis()-waveDelayStart <= 1000) {
      // If the amount of milliseconds that has passed is greater than 900 and less than or equal to 1000, the following code will execute
      leds[8] = CRGB::Black;
      leds[36] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 1000 && millis()-waveDelayStart <= 1100) {
      // If the amount of milliseconds that has passed is greater than 1000 and less than or equal to 1100, the following code will execute
      leds[36] = CRGB::Black;
      leds[10] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 1100 && millis()-waveDelayStart <= 1200) {
      // If the amount of milliseconds that has passed is greater than 1100 and less than or equal to 1200, the following code will execute
      leds[10] = CRGB::Black;
      leds[34] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 1200 && millis()-waveDelayStart <= 1300) {
      // If the amount of milliseconds that has passed is greater than 1200 and less than or equal to 1300, the following code will execute
      leds[34] = CRGB::Black;
      leds[12] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 1300 && millis()-waveDelayStart <= 1400) {
      // If the amount of milliseconds that has passed is greater than 1300 and less than or equal to 1400, the following code will execute
      leds[12] = CRGB::Black;
      leds[32] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 1400 && millis()-waveDelayStart <= 1500) {
      // If the amount of milliseconds that has passed is greater than 1400 and less than or equal to 1500, the following code will execute
      leds[32] = CRGB::Black;
      leds[14] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 1500 && millis()-waveDelayStart <= 1600) {
      // If the amount of milliseconds that has passed is greater than 1500 and less than or equal to 1600, the following code will execute
      leds[14] = CRGB::Black;
      leds[30] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 1600 && millis()-waveDelayStart <= 1700) {
      // If the amount of milliseconds that has passed is greater than 1600 and less than or equal to 1700, the following code will execute
      leds[30] = CRGB::Black;
      leds[16] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 1700 && millis()-waveDelayStart <= 1800) {
      // If the amount of milliseconds that has passed is greater than 1700 and less than or equal to 1800, the following code will execute
      leds[16] = CRGB::Black;
      leds[28] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 1800 && millis()-waveDelayStart <= 1900) {
      // If the amount of milliseconds that has passed is greater than 1800 and less than or equal to 1900, the following code will execute
      leds[28] = CRGB::Black;
      leds[18] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 1900 && millis()-waveDelayStart <= 2000) {
      // If the amount of milliseconds that has passed is greater than 1900 and less than or equal to 2000, the following code will execute
      leds[18] = CRGB::Black;
      leds[26] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 2000 && millis()-waveDelayStart <= 2100) {
      // If the amount of milliseconds that has passed is greater than 2000 and less than or equal to 2100, the following code will execute
      leds[26] = CRGB::Black;
      leds[20] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 2100 && millis()-waveDelayStart <= 2200) {
      // If the amount of milliseconds that has passed is greater than 2100 and less than or equal to 2200, the following code will execute
      leds[20] = CRGB::Black;
      leds[24] = color;
      delay(30);
      FastLED.show();
    }
    else if(millis()-waveDelayStart > 2200 && millis()-waveDelayStart <= 2300) {
      // If the amount of milliseconds that has passed is greater than 2200 and less than or equal to 2300, the following code will execute
      leds[24] = CRGB::Black;
      leds[22] = color;
      delay(30);
      FastLED.show();
    }
    else {
      // Otherwise, waveDelayStart will be set to the current amount of milliseconds passed
      waveDelayStart = millis();
    }

  }



  // Makes a moving candycane pattern out of two inputted colors
  void candy_cane(CRGB color1, CRGB color2) {

    if(millis()-candyCaneDelayStart > 0 && millis()-candyCaneDelayStart <= 500) {
      // If the amount of milliseconds that has passed is greater than 0 and less than or equal to 500, the following code will execute
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


  /////////////////////////////////////////// LOOP //////////////////////////////////////////////////////


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

