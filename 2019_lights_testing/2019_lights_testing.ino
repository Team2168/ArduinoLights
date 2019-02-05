
#include <FastLED.h>

#define NUM_LEDS 256
#define DATA_PIN 3
#define NUM_ROWS 8
#define NUM_COLUMNS 32

CRGB leds[NUM_LEDS];

//////////////////////////////////////// SETUP /////////////////////////////////////////////

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(30);
}

//////////////////////////////////////// PATTERNS /////////////////////////////////////////////
/*
 * Fills the whole panel with a single color 
 */
void fill(CRGB color)
{
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
  FastLED.show();
}

/*
 * Fills the indicated column (indexed starting with 0) with the specified color
 */
void fillColumn(CRGB color, int column)
{
  for (int i = NUM_ROWS*column; i<(NUM_ROWS*column + NUM_ROWS); i++)
  {
    leds[i] = color;
  }
  FastLED.show();
  
}

/*
 * Blinks the indicated color on the whole panel at the designated speed
 */
void blink(CRGB color, int speed)
{
  fill(color);
  delay(speed);
  fill(CRGB::Black);
  delay(speed);
}

/*
 * A single column of indicated color moves from column 0 to the end, moving at the indicated speed
 */
void wave(CRGB color, int speed)
{
  for (int i = 0; i<NUM_COLUMNS; i++)
  {
    fillColumn(color, i);
    delay(speed);
  }
  fill(CRGB::Black);
}
//////////////CRGB BRIGHTNESS--less effective, less manipulability
/*
 * Sets the brightness of a single column to the max using RGB adjustment 
 */
void increaseBrightnessColumn(int column)
{
  for (int i = NUM_ROWS*column; i<(NUM_ROWS*column+NUM_ROWS); i++)
  {
    leds[i].maximizeBrightness();
  }
}

/*
 * Decreases the brightness of the indicated column to the indicated percentage using RGB built-in adjustment
 */
void decreaseBrightnessColumn(int column, int percentage)
{
  for (int i = NUM_ROWS*column; i<(NUM_ROWS*column+NUM_ROWS); i++)
  {
    leds[i] %= percentage;
  }
}

/*
 * A dimmer panel is filled by column with brighter color, starting with column 0
 * @param CRGB color (by name, hex code, or individual numbers)
 * @param speed is the delay for the column to move
 */
void brightnessWave(CRGB color, int speed)
{
  
  fill(color);
  for (int i = 0; i<NUM_LEDS; i++)
  {
    leds[i] %= 64;
  }
  FastLED.show();
  //incrementing each column
  for (int i = 0; i<NUM_COLUMNS; i++)
  {
    //ups brightness
    increaseBrightnessColumn(i);
    FastLED.show();
    delay(speed);
  }
}

/*
 * A brighter colored column moves across the dimmer panel starting at column 0
 */
void runningColumns(CRGB color, int speed)
{
  fill(color);
  for (int i = 0; i<NUM_LEDS; i++)
  {
    leds[i] %= 30;
  }
  FastLED.show();
  //incrementing each column
  increaseBrightnessColumn(0);
  FastLED.show();
  for (int i = 1; i<(NUM_COLUMNS); i++)
  {
    //ups brightness of current column
    increaseBrightnessColumn(i);
    //decreases brightness of previous column
    decreaseBrightnessColumn((i-1), 30);
    FastLED.show();
    delay(speed);
  }
  decreaseBrightnessColumn((NUM_COLUMNS-1), 30);
  FastLED.show();
  delay(speed);
}

/*
 * changes the brightness of a single column in the indicated color
 * @param color is a CHSV object 
 * @param brightness increment is an increase (positive number) or decrease (neg number) from the last declared Value of this color
 * This value is only retained within the scope of this function. (does not change the overall color value)
 */
void brightnessColumnsHSV(CHSV color, int brightnessIncrement, int column)
{
  color.val += brightnessIncrement;
  for (int i = NUM_ROWS*column; i<(NUM_ROWS*column+NUM_ROWS); i++)
  {
    leds[i] = color;
  }
}

/*
 * The dimmer panel is filled with brighter columns, starting at column 0, using HSV color value
 * @param speed is a delay between before the addition of a new column
 */
void brightnessWaveHSV(int hue, int sat, int val, int speed)
{
  CHSV color(hue, sat, val);
  color.val = 100;
  fill(color);
  FastLED.show();
  //incrementing each column
  
  for (int i = 0; i<NUM_COLUMNS; i++)
  {
    //ups brightness
    brightnessColumnsHSV( color, 150, i);
    
    FastLED.show();
    delay(speed);
  }
}

/*
 * A brighter colored column trio (midbrightness-highbrightness-midbrightness moves across the dimmer panel starting at column 0, using HSV colors
 * @param speed is the delay for the moving columns
 */
void runningColumnsRightHSV(int hue, int sat, int val, int speed)
{
  int mediumBrightness = 75;
  int highBrightness = 150;
  CHSV color(hue, sat, val);
  color.val = 100;
  fill(color);
  FastLED.show();
  //incrementing each column
  brightnessColumnsHSV(color, mediumBrightness, 0);
  FastLED.show();
  delay(speed);
  brightnessColumnsHSV(color, mediumBrightness, 1);
  brightnessColumnsHSV(color, mediumBrightness, 0);
  FastLED.show();
  delay(speed);
  brightnessColumnsHSV(color, mediumBrightness, 2);
  brightnessColumnsHSV(color, highBrightness, 1);
  brightnessColumnsHSV(color, mediumBrightness, 0);
  for (int i = 2; i<(NUM_COLUMNS-1); i++)
  {
    //ups brightness
    brightnessColumnsHSV(color, mediumBrightness, (i+1));
    brightnessColumnsHSV(color, highBrightness, i);
    brightnessColumnsHSV(color, mediumBrightness, (i-1));
    brightnessColumnsHSV(color, 0, (i-2));
    FastLED.show();
    delay(speed);
  }
  brightnessColumnsHSV(color, highBrightness, (NUM_COLUMNS-1));
  brightnessColumnsHSV(color, mediumBrightness, (NUM_COLUMNS-2));
  brightnessColumnsHSV(color, 0, (NUM_COLUMNS-3));
  FastLED.show();
  delay(speed);
  brightnessColumnsHSV(color, mediumBrightness, (NUM_COLUMNS-1));
  brightnessColumnsHSV(color, 0, (NUM_COLUMNS-2));
  FastLED.show();
  delay(speed);
  brightnessColumnsHSV(color, 0, (NUM_COLUMNS-1));
  FastLED.show();
  delay(speed);
}

/*
 * A brighter colored column trio (midbrightness-highbrightness-midbrightness moves across the dimmer panel starting at the highest index column, using HSV colors
 * @param speed is the delay for the moving columns
 */
void runningColumnsLeftHSV(int hue, int sat, int val, int speed)
{
  int mediumBrightness = 75;
  int highBrightness = 150;
  CHSV color(hue, sat, val);
  color.val = 100;
  fill(color);
  FastLED.show();
  //incrementing each column
  brightnessColumnsHSV(color, mediumBrightness, (NUM_COLUMNS-1));
  FastLED.show();
  delay(speed);
  brightnessColumnsHSV(color, mediumBrightness, (NUM_COLUMNS-2));
  brightnessColumnsHSV(color, highBrightness, (NUM_COLUMNS-1));
  FastLED.show();
  delay(speed);
  brightnessColumnsHSV(color, mediumBrightness, (NUM_COLUMNS-3));
  brightnessColumnsHSV(color, highBrightness, (NUM_COLUMNS-2));
  brightnessColumnsHSV(color, mediumBrightness, (NUM_COLUMNS-1));
  for (int i = (NUM_COLUMNS-3); i>0; i--)
  {
    //ups brightness
    brightnessColumnsHSV(color, mediumBrightness, (i-1));
    brightnessColumnsHSV(color, highBrightness, i);
    brightnessColumnsHSV(color, mediumBrightness, (i+1));
    brightnessColumnsHSV(color, 0, (i+2));
    FastLED.show();
    delay(speed);
  }
  brightnessColumnsHSV(color, highBrightness, 0);
  brightnessColumnsHSV(color, mediumBrightness, 1);
  brightnessColumnsHSV(color, 0, 2);
  FastLED.show();
  delay(speed);
  brightnessColumnsHSV(color, mediumBrightness, 0);
  brightnessColumnsHSV(color, 0, 1);
  FastLED.show();
  delay(speed);
  brightnessColumnsHSV(color, 0, 0);
  FastLED.show();
  delay(speed);
}
  


/////////////////////////////////////////// LOOP //////////////////////////////////////////////////////

void loop() {
  //Test code that works :)
   //blink(0x4d0099, 300);
   //wave(CRGB::Blue, 300);
   //wave(0x00b300, 300);

   //using RGB brightness control
//    brightnessWave(CRGB::Blue, 200);
//    runningColumns(CRGB::Green, 300);

   //testing brightness adjustments with %= and maximizeBrightness No easy way to increase brightness gradually->thus the HSV solution
   //Works
//   fill(CRGB::Blue);
//   FastLED.show();
//   for (int i = 0; i<NUM_LEDS; i++)
//   {
//    leds[i] %= 64;
//   }
//   FastLED.show();
//   delay(1000);
//   for (int i = 0; i<NUM_LEDS; i++)
//   {
//    leds[i].maximizeBrightness();
//   }
//   FastLED.show(); 
//   delay(1000);


//   next test block: direct brightness control with HSV 
//   CHSV Blue(160, 255, 255);
//   fill(Blue);
//   FastLED.show();
//   delay(1000);
//   Blue.val += -200;
//   fill(Blue);
//   FastLED.show();
//   delay(1000);
//   Blue.val += 100;
//   fill(Blue);
//   FastLED.show();
//   delay(1000);
//   brightnessColumnsHSV(Blue, -100, 0);
//   FastLED.show();
//   delay(1000);

//   HSV running columns in red and blue
   runningColumnsRightHSV(160, 255, 200, 50);
   runningColumnsLeftHSV(245, 255, 200, 50);
  
}
