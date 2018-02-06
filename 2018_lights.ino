#include <FastLED.h>
#include <Wire.h>

#define NUM_LEDS 27
#define DATA_PIN 3
#define I2C_ID 10

// lightStates[0] is red1, [1] is g1, [2] is b1, [3] is r2, [4] is g2, [5] is b2, [6] is pattern, and [7] is speed
byte lightStates[8] = {0, 0, 255, 255, 0, 0, 14, 1};


int animation[19][9] = {{0, 1, 2, 16, 17, 18, 19, 20, 9}, {1, 2, 3, 15, 16, 19, 20, 21, 9}, {2, 3, 4, 14, 15, 20, 21, 22, 9}, {3, 4, 5, 13, 14, 21, 22, 23, 9}, {4, 5, 6, 12, 13, 22, 23, 24, 9}, {5, 6, 7, 11, 12, 23, 24, 25, 9}, {6, 7, 8, 10, 11, 24, 25, 26, 9}, {5, 6, 7, 11, 12, 23, 24, 25, 10}, {4, 5, 6, 12, 13, 22, 23, 24, 11}, {3, 4, 5, 13, 14, 21, 22, 23, 12}, {2, 3, 4, 14, 15, 20, 21, 22, 13}, {1, 2, 3, 15, 16, 19, 20, 21, 14}, {0, 1, 2, 16, 17, 18, 19, 20, 15}, {0, 1, 2, 16, 17, 18, 19, 20, 14}, {0, 1, 2, 16, 17, 18, 19, 20, 13}, {0, 1, 2, 16, 17, 18, 19, 20, 12}, {0, 1, 2, 16, 17, 18, 19, 20, 11}, {0, 1, 2, 16, 17, 18, 19, 20, 10}, {0, 1, 2, 16, 17, 18, 19, 20, 9}};

//int frame1[9] = {0, 1, 2, 16, 17, 18, 19, 20, 9};
//int frame2[9] = {1, 2, 3, 15, 16, 19, 20, 21, 9};
//int frame3[9] = {2, 3, 4, 14, 15, 20, 21, 22, 9};
//int frame4[9] = {3, 4, 5, 13, 14, 21, 22, 23, 9};
//int frame5[9] = {4, 5, 6, 12, 13, 22, 23, 24, 9};
//int frame6[9] = {5, 6, 7, 11, 12, 23, 24, 25, 9};
//int frame7[9] = {6, 7, 8, 10, 11, 24, 25, 26, 9};
//int frame8[9] = {5, 6, 7, 11, 12, 23, 24, 25, 10};
//int frame9[9] = {4, 5, 6, 12, 13, 22, 23, 24, 11};
//int frame10[9] = {3, 4, 5, 13, 14, 21, 22, 23, 12};
//int frame11[9] = {2, 3, 4, 14, 15, 20, 21, 22, 13};
//int frame12[9] = {1, 2, 3, 15, 16, 19, 20, 21, 14};
//int frame13[9] = {0, 1, 2, 16, 17, 18, 19, 20, 15};
//int frame14[9] = {0, 1, 2, 16, 17, 18, 19, 20, 14};
//int frame15[9] = {0, 1, 2, 16, 17, 18, 19, 20, 13};
//int frame16[9] = {0, 1, 2, 16, 17, 18, 19, 20, 12};
//int frame17[9] = {0, 1, 2, 16, 17, 18, 19, 20, 11};
//int frame18[9] = {0, 1, 2, 16, 17, 18, 19, 20, 10};
//int frame19[9] = {0, 1, 2, 16, 17, 18, 19, 20, 9};
//int animation[19] = {&frame1, &frame2, &frame3, &frame4, &frame5, &frame6, &frame7, &frame8, &frame9, &frame10, &frame11, &frame12, &frame13, &frame14, &frame15, &frame16, &frame17, &frame18, &frame19};

#define OFF_PATTERN_ID 0
#define FILL_PATTERN_ID 1
#define BLINK_PATTERN_ID 2
#define CHASE_PATTERN_ID 3
#define LOADING_PATTERN_ID 4
#define ARROWS_RIGHT_PATTERN_ID 5
#define ARROWS_LEFT_PATTERN_ID 6
#define RAINBOW_PATTERN_ID 7
#define SPARKLE_PATTERN_ID 8
#define CONFETTI_PATTERN_ID 9
#define CROSS_PATTERN_ID 10
#define POLICE_PATTERN_ID 11
#define LEAPFROG_PATTERN_ID 12
#define WAVE_PATTERN_ID 13
#define CANDY_CANE_PATTERN_ID 14
#define INTAKE_CUBE_PATTERN_ID 15

  CRGB leds[NUM_LEDS];
  uint8_t hue = 0;
  int i = 0;
  int a = 0;
  int b = 0;
  int c = 0;
  int d = 0;



  void setup() {

    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

    Serial.begin(9600);
    Wire.begin(I2C_ID);
    Wire.onReceive(receiveEvent1);

    FastLED.setBrightness(20);

    delay(30);

  }


  /**
   *
  **/


  void makePatterns(int r1, int g1, int b1, int r2, int g2, int b2, int pattern, int speed) {

    switch (pattern) {
      case FILL_PATTERN_ID:
        fill(CRGB(r1, g1, b1));
        break;
      case BLINK_PATTERN_ID:
        blink_all(CRGB(r1, g1, b1), speed);
        break;
      case CHASE_PATTERN_ID:
        chase(CRGB(r1, g1, b1), speed);
        break;
      case LOADING_PATTERN_ID:
        loading(CRGB(r1, g1, b1), speed);
        break;
      case ARROWS_RIGHT_PATTERN_ID:
        arrows_right();
        break;
      case ARROWS_LEFT_PATTERN_ID:
        arrows_left();
        break;
      case RAINBOW_PATTERN_ID:
        rainbow();
        break;
      case SPARKLE_PATTERN_ID:
        sparkle();
        break;
      case CONFETTI_PATTERN_ID:
        confetti();
        break;
      case CROSS_PATTERN_ID:
        cross();
        break;
      case POLICE_PATTERN_ID:
        police();
        break;
      case LEAPFROG_PATTERN_ID:
        leapfrog(CRGB(r1, g1, b1));
        break;
      case WAVE_PATTERN_ID:
        wave(CRGB(r1, g1, b1));
        break;
      case CANDY_CANE_PATTERN_ID:
        candy_cane(CRGB(r1, g1, b1), CRGB(r2, g2, b2));
        break;
       case INTAKE_CUBE_PATTERN_ID:
        intake_cube();
        break;

      default: //OFF_PATTERN_ID:
        off();
        break;
    }

  }


  //////////////////////////////////////// MAIN PATTERNS /////////////////////////////////////////////


  // Turns off all LEDs
  void off() {

    // Sets all LEDs to off
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;

      delay(30);
    }

    // Turns the LEDs off
    FastLED.show();

  }


  // Makes all LEDs the inputted color
  void fill(CRGB color) {

    for (int i = 0; i < NUM_LEDS; i++) {
      // Sets all LEDs to the inputted color
      leds[i] = color;

      delay(30);
    }

    // Shows LEDs
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

    // Sets all LEDs to the inputted color
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = color;

      delay(30);
    }

    // Turns on LEDs and waits for the inputted amound of time
    FastLED.show();
    delay(speed);

    // Sets all LEDs to off
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;

      delay(30);
    }

    // Turns off LEDs and waits for the inputted amount of time
    FastLED.show();
    delay(speed);

  }



  // Makes the lights turn on and then off one at a time, first going right and then going left
  // Color sets what color the LEDs will be
  // Speed sets the amount of miliseconds it takes the LEDs to switch from the color to off and how long it takes the next LED to turn on
  void chase(CRGB color, float speed) {

    // Makes each light turn on and then off one at a time
    // starting from LED 0
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = color;

      delay(30);
      FastLED.show();
      delay(speed);

      leds[i] = CRGB::Black;

      delay(30);
      FastLED.show();
      delay(speed);
    }

    // Makes each light turn on and then off one at a time
    // starting from the last LED
    for (int i = NUM_LEDS - 1; i > -1; i--) {
      leds[i] = color;
      delay(30);
      FastLED.show();
      delay(speed);

      leds[i] = CRGB::Black;

      delay(30);
      FastLED.show();
      delay(speed);
    }

  }



  // Turns on each light one at a time
  // Color sets what color the LEDs will be
  // Speed sets the amount of miliseconds it takes the LEDs to turn on
  void loading(CRGB color, float speed) {

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = color;
      delay(30);

      FastLED.show();
      delay(speed);
    }

    // Turns off each light one at a time
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
      delay(30);

      FastLED.show();
      delay(speed);
    }

  }



  // Makes red, green, and blue arrows scrolling to the right
  void arrows_right() {

    // Only works with 3 rows of 9 LEDs

    a = 18 + i % 9;
    b = 17 - i % 9;
    c = 0 + i % 9;
    d = 17 - (i + 1) % 9;


    leds[a] = CRGB::Red;
    leds[b] = CRGB::Red;
    leds[c] = CRGB::Red;
    leds[d] = CRGB::Red;

    leds[18 + (i + 3) % 9] = CRGB::Green;
    leds[17 - (i + 3) % 9] = CRGB::Green;
    leds[0 + (i + 3) % 9] = CRGB::Green;
    leds[17 - (i + 4) % 9] = CRGB::Green;

    leds[18 + (i + 6) % 9] = CRGB::Blue;
    leds[17 - (i + 6) % 9] = CRGB::Blue;
    leds[0 + (i + 6) % 9] = CRGB::Blue;
    leds[17 - (i + 7) % 9] = CRGB::Blue;

    delay(30);
    FastLED.show();
    delay(100);

    leds[a] = CRGB::Black;
    leds[b] = CRGB::Black;
    leds[c] = CRGB::Black;
    leds[d] = CRGB::Black;

    leds[18 + (i + 3) % 9] = CRGB::Black;
    leds[17 - (i + 3) % 9] = CRGB::Black;
    leds[0 + (i + 3) % 9] = CRGB::Black;
    leds[17 - (i + 4) % 9] = CRGB::Black;

    leds[18 + (i + 6) % 9] = CRGB::Black;
    leds[17 - (i + 6) % 9] = CRGB::Black;
    leds[0 + (i + 6) % 9] = CRGB::Black;
    leds[17 - (i + 7) % 9] = CRGB::Black;

    delay(30);
    FastLED.show();
    delay(100);

    i = (i + 1) % 9;
  }



  // Makes red, green, and blue arrows scrolling to the left
  void arrows_left() {

    // Only works with 3 rows of 9 LEDs

    a = 26 - i % 9;
    b = 9 + i % 9;
    c = 8 - i % 9;
    d = 9 + (i + 1) % 9;

    leds[a] = CRGB::Red;
    leds[b] = CRGB::Red;
    leds[c] = CRGB::Red;
    leds[d] = CRGB::Red;

    leds[26 - (i + 3) % 9] = CRGB::Green;
    leds[9 + (i + 3) % 9] = CRGB::Green;
    leds[8 - (i + 3) % 9] = CRGB::Green;
    leds[9 + (i + 4) % 9] = CRGB::Green;

    leds[26 - (i + 6) % 9] = CRGB::Blue;
    leds[9 + (i + 6) % 9] = CRGB::Blue;
    leds[8 - (i + 6) % 9] = CRGB::Blue;
    leds[9 + (i + 7) % 9] = CRGB::Blue;

    delay(30);
    FastLED.show();
    delay(100);

    leds[a] = CRGB::Black;
    leds[b] = CRGB::Black;
    leds[c] = CRGB::Black;
    leds[d] = CRGB::Black;

    leds[26 - (i + 3) % 9] = CRGB::Black;
    leds[9 + (i + 3) % 9] = CRGB::Black;
    leds[8 - (i + 3) % 9] = CRGB::Black;
    leds[9 + (i + 4) % 9] = CRGB::Black;

    leds[26 - (i + 6) % 9] = CRGB::Black;
    leds[9 + (i + 6) % 9] = CRGB::Black;
    leds[8 - (i + 6) % 9] = CRGB::Black;
    leds[9 + (i + 7) % 9] = CRGB::Black;

    delay(30);
    FastLED.show();
    delay(100);

    i = (i + 1) % 9;

  }



  void receiveEvent1(int numBytes) {

    while (Wire.available() >= 8) {
      for (int i = 0; i < 8; i++) {
        lightStates[i] = Wire.read();
      }
    }

  }



  ////////////////////////////////////////// EXTRA PATTERNS /////////////////////////////////////////


  // Makes the lights change colors to different colors of the rainbow
  void rainbow() {

    fill_rainbow( leds, NUM_LEDS, hue, 4);

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



  // Makes a red, blinking cross with a blinking white background
  void cross() {

    // Only works as intended with 3 rows of 9 LEDs
    for (int i = 0; i < NUM_LEDS; i++) {

      // Sets all LEDs to white
      leds[i] = CRGB::White;

      delay(30);

    }

    // Sets the following LEDS to be off
    leds[4] = CRGB::Black;
    leds[12] = CRGB::Black;
    leds[13] = CRGB::Black;
    leds[14] = CRGB::Black;
    leds[22] = CRGB::Black;

    delay(30);

    // Turns on the leds
    FastLED.show();
    delay(50);

    // Sets all LEDs to be off
    for (int i = 0; i < NUM_LEDS; i++) {

      leds[i] = CRGB::Black;

      delay(30);

    }

    // Sets the following LEDS to be red
    leds[4] = CRGB::Red;
    leds[12] = CRGB::Red;
    leds[13] = CRGB::Red;
    leds[14] = CRGB::Red;
    leds[22] = CRGB::Red;

    delay(30);
    FastLED.show();

    delay(50);

  }



  // Makes all LEDs blink from blue to red
  void police() {

    // Sets all LEDs to blue
    fill(CRGB::Blue);

    // Sets all LEDs to red
    fill(CRGB::Red);

  }



  void leapfrog(CRGB color) {

    // Makes the lights look like they're playing leapfrog
    // Color makes the lights the inputted color
    for (int i = 0; i < NUM_LEDS; i++) {

      leds[i] = color;

      delay(300);

      FastLED.show();

      delay(300);

      leds[i - 1] = CRGB::Black;

      leds[NUM_LEDS - 1] = CRGB::Black;

      FastLED.show();

    }

  }



  // Makes a wave motion with the LEDs
  void wave(CRGB color) {

    // Only works as intended with 3 rows of 9 LEDs
    // Color makes the lights the inputted color
    leds[8] = CRGB::Black;
    leds[0] = color;
    delay(30);
    FastLED.show();
    delay(100);
    leds[0] = CRGB::Black;
    leds[16] = color;
    delay(30);
    FastLED.show();
    delay(100);
    leds[16] = CRGB::Black;
    leds[2] = color;
    delay(30);
    FastLED.show();
    delay(100);
    leds[2] = CRGB::Black;
    leds[14] = color;
    delay(30);
    FastLED.show();
    delay(100);
    leds[14] = CRGB::Black;
    leds[4] = color;
    delay(30);
    FastLED.show();
    delay(100);
    leds[4] = CRGB::Black;
    leds[12] = color;
    delay(30);
    FastLED.show();
    delay(100);
    leds[12] = CRGB::Black;
    leds[6] = color;
    delay(30);
    FastLED.show();
    delay(100);
    leds[6] = CRGB::Black;
    leds[10] = color;
    delay(30);
    FastLED.show();
    delay(100);
    leds[10] = CRGB::Black;
    leds[8] = color;
    delay(30);
    FastLED.show();
    delay(100);

  }



  // Makes a moving candycane pattern out of two inputted colors
  void candy_cane(CRGB color1, CRGB color2) {

    fill_ns(color1);

    for (i = 0; i < NUM_LEDS; i += 2) {
      leds[i] = color2;
    }

    FastLED.show();

    delay(500);

    fill_ns(color1);

    for (i = 1; i < NUM_LEDS; i += 2) {
      leds[i] = color2;
    }

    FastLED.show();

    delay(500);

  }



  void intake_cube() {
  
    for(i=0; i<19; i++) {
      leds[animation[i][0]] = CRGB::Red;
      leds[animation[i][1]] = CRGB::Red;
      leds[animation[i][2]] = CRGB::Red;
      leds[animation[i][3]] = CRGB::Red;
      leds[animation[i][4]] = CRGB::Red;
      leds[animation[i][5]] = CRGB::Red;
      leds[animation[i][6]] = CRGB::Red;
      leds[animation[i][7]] = CRGB::Red;
      leds[animation[i][8]] = CRGB::Yellow;
  
      delay(30);
      FastLED.show();
      delay(100);
      fill(CRGB::Black);
      delay(100);
    
    }
  
  }


  /////////////////////////////////////////// LOOP //////////////////////////////////////////////////////


  // Extra features / low priorty: intake cube animation
  // Create branch "2018 Lights"
  void loop() {

    //makePatterns(lightStates[0], lightStates[1], lightStates[2], lightStates[3], lightStates[4], lightStates[5], lightStates[6], lightStates[7]);
    intake_cube();

  }
