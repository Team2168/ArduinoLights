
#include <FastLED.h>

#define NUM_LEDS 256
#define DATA_PIN 3
#define NUM_ROWS 8
#define NUM_COLUMNS 32

CRGB leds[NUM_LEDS];

/**
 * Dependency for the wave pattern
 */
 unsigned long waveDelayStart = 0.0;
 int waveCounter = -1;
 boolean waveForward = true;

/**
 * Dependency for the columnLoader pattern
 */
 unsigned long columnLoaderDelayStart = 0.0;
 int columnLoaderCounter = 0;
 boolean columnLoaderForward = true;
 
 /**
  * Dependency for the brightnessWave pattern
  */
 unsigned long brightnessWaveHSVDelayStart = 0.0;
 int brightnessWaveHSVCounter = -1;
 boolean brightnessWaveHSVForward = true;

 /**
  * dependency for the runningColumnsRightHSV pattern
  */
  unsigned long runningColumnsRightHSVDelayStart = 0.0;
  int runningColumnsRightHSVCounter = -2;
 /**
  * Dependency for the runningColumnsLeftHSV pattern
  */
  unsigned long runningColumnsLeftHSVDelayStart = 0.0;
  int runningColumnsLeftHSVCounter = NUM_COLUMNS+1;
  
 /**
  * Dependency for the runningColumnsLeftHSVpattern
  */
  boolean runningColumnsForward = true;
  
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
 * Dependency for the candy cane pattern
 */
unsigned long candyCaneDelayStart = 0.0;


//////////////////////////////////////// SETUP /////////////////////////////////////////////

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(30);
}

//////////////////////////////////////// PATTERNS /////////////////////////////////////////////
/**
 * Turns off all LEDs
 */
 void off() 
 {
  for (int i = 0; i <NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }

  FastLED.show();
 }
 
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

/**
 * Sets all LEDs to the inputted color without turning the LEDs on
 */
 void fill_ns(CRGB color)
 {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
  }
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

void fillColumn_ns(CRGB color, int column)
{
  for (int i = NUM_ROWS*column; i<(NUM_ROWS*column + NUM_ROWS); i++)
  {
    leds[i] = color;
  }
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
 * This doesn't work with millis and I don't know why!!
 */
void wave(CRGB color, float speed)
{
//  for (int i = 0; i<NUM_COLUMNS; i++)
//  {
//    fillColumn(color, i);
//    delay(speed);
//  }
//  fill(CRGB::Black);

//  bool timeElapsedGreaterThanEqualZero = millis()-waveDelayStart >= 0;
//  bool timeElapsedGreaterThanZero = millis()-waveDelayStart > 0;
//  bool timeElapsedLessThanEqualSpeed = millis()-waveDelayStart <= speed;
//  bool timeElapsedGreaterThanSpeed = millis()-waveDelayStart > speed;
//  bool timeElapsedLessThanEqualTwiceSpeed = millis()-waveDelayStart <= 2*speed;
//
//  if(waveForward)
//  {
//    if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
//    {
//      fillColumn_ns(color, waveCounter);
//    }
//    else if (timeElapsedGreaterThanSpeed && timeElapsedLessThanEqualTwiceSpeed)
//    {
//      fillColumn_ns(color, (waveCounter+1));
//      fillColumn_ns(CRGB::Black, waveCounter); //delete this to have a loading bar
//    }
//    else 
//    {
//      waveDelayStart = millis();
//      waveCounter++;
//      
//      if (waveCounter >= NUM_COLUMNS)
//      {
//        waveForward = false;
//      }
//    }
//
//    FastLED.show();
//  }
//  else 
//  {
//    if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
//    {
//      fillColumn_ns(color, waveCounter);
//    }
//    else if (timeElapsedGreaterThanSpeed && timeElapsedLessThanEqualTwiceSpeed)
//    {
//      fillColumn_ns(color, waveCounter-1);
//      fillColumn_ns(CRGB::Black, waveCounter);
//    }
//    else 
//    {
//      waveDelayStart = millis();
//      waveCounter--;
//
//      if (waveCounter < 1)
//      {
//        waveForward = true;
//      }
//    }
//    FastLED.show();
//  }

  bool timeElapsedGreaterThanEqualZero = millis()-waveDelayStart >= 0;
  bool timeElapsedGreaterThanZero = millis()-waveDelayStart > 0;
  bool timeElapsedLessThanEqualSpeed = millis()-waveDelayStart <= speed;

  if(waveForward)
  {
    if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && waveCounter == -1)
    {
      fillColumn_ns(color, waveCounter+1);
      fillColumn_ns(CRGB::Black, NUM_COLUMNS-1);
    }
    else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
    {
      fillColumn_ns(color, (waveCounter+1));
      fillColumn_ns(CRGB::Black, waveCounter); 
    }
    else 
    {
      waveDelayStart = millis();
      waveCounter++;
      
      if (waveCounter >= NUM_COLUMNS-1)
      {
        waveForward = false;
        waveCounter = NUM_COLUMNS;
      }
    }

    FastLED.show();
  }
  else 
  {
    if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && waveCounter == NUM_COLUMNS)
    {
      fillColumn_ns(color, waveCounter-1);
      fillColumn_ns(CRGB::Black, 0);
    }
    else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
    {
      fillColumn_ns(color, waveCounter-1);
      fillColumn_ns(CRGB::Black, waveCounter);
    }
    else 
    {
      waveDelayStart = millis();
      waveCounter--;

      if (waveCounter < 1)
      {
        waveForward = true;
      }
    }
    FastLED.show();
  }
}

void columnLoader(CRGB color, int speed)
{
  bool timeElapsedGreaterThanEqualZero = millis()-columnLoaderDelayStart >= 0;
  bool timeElapsedLessThanEqualSpeed = millis()-columnLoaderDelayStart <= speed;
  
  if(columnLoaderForward)
  {
    if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
    {
      fillColumn_ns(color, columnLoaderCounter);
    }
    else 
    {
      columnLoaderDelayStart = millis();
      columnLoaderCounter++;
      
      if (columnLoaderCounter >= NUM_COLUMNS)
      {
        off();
        columnLoaderCounter = 0;
      }
    }

    FastLED.show();
  }
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

///*
// * DOESN'T WORK WORK WITH MILLIS
// * A dimmer panel is filled by column with brighter color, starting with column 0
// * @param CRGB color (by name, hex code, or individual numbers)
// * @param speed is the delay for the column to move
// */
//void brightnessWave(CRGB color, int speed)
//{
//  
////  fill(color);
////  for (int i = 0; i<NUM_LEDS; i++)
////  {
////    leds[i] %= 64;
////  }
////  FastLED.show();
////  //incrementing each column
////  for (int i = 0; i<NUM_COLUMNS; i++)
////  {
////    //ups brightness
////    increaseBrightnessColumn(i);
////    FastLED.show();
////    delay(speed);
////  }
//    bool timeElapsedGreaterThanEqualZero = millis()-brightnessWaveDelayStart >= 0;
//    bool timeElapsedLessThanEqualSpeed = millis()-brightnessWaveDelayStart <= speed;
//    if (brightnessWaveCounter = 0)
//    {
//      fill(color);
//      for (int i = 0; i<NUM_LEDS; i++)
//      {
//        leds[i] %= 64;
//      }
//      FastLED.show();
//    }
//    //incrementing each column
//    if(brightnessWaveForward)
//    {
//      //delay time
//      if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
//      {
//        increaseBrightnessColumn(brightnessWaveCounter);
//      }
//      
//      else
//      {
//        brightnessWaveDelayStart = millis();
//        brightnessWaveCounter++;
//
//        if (brightnessWaveCounter >= NUM_LEDS-1)
//        {
//          brightnessWaveForward = false;
//          brightnessWaveCounter = 0;
//        }
//      }
//    }
//    
//}

/*
 * A brighter colored column moves across the dimmer panel starting at column 0
 * Doesn't work with millis because 
 * it relies on percentage reductions of brightness
 */
//void runningColumns(CRGB color, int speed)
//{
//  fill(color);
//  for (int i = 0; i<NUM_LEDS; i++)
//  {
//    leds[i] %= 30;
//  }
//  FastLED.show();
//  //incrementing each column
//  increaseBrightnessColumn(0);
//  FastLED.show();
//  for (int i = 1; i<(NUM_COLUMNS); i++)
//  {
//    //ups brightness of current column
//    increaseBrightnessColumn(i);
//    //decreases brightness of previous column
//    decreaseBrightnessColumn((i-1), 30);
//    FastLED.show();
//    delay(speed);
//  }
//  decreaseBrightnessColumn((NUM_COLUMNS-1), 30);
//  FastLED.show();
//  delay(speed);
//    bool timeElapsedGreaterThanEqualZero = millis()-runningColumnsDelayStart >= 0;
//    bool timeElapsedLessThanEqualSpeed = millis()-runningColumnsDelayStart <= speed;
//    bool timeElapsedGreaterThanSpeed = millis()-runningColumnsDelayStart > speed;
//    bool timeElapsedLessThanEqualTwiceSpeed = millis()-runningColumnsDelayStart <= 2*speed;
//    //set-up
//    if (runningColumnsCounter == 0)
//    {
//      fill(color);
//      for (int i = 0; i<NUM_LEDS; i++)
//      {
//        leds[i] %= 64;
//      }
//      FastLED.show();
//      increaseBrightnessColumn(runningColumnsCounter);
//    }
//    //incrementing each column
//    if(runningColumnsForward)
//    {
//      //delay time
//      if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
//      {
//        ;
//      }
//      else if(timeElapsedGreaterThanSpeed && timeElapsedLessThanEqualTwiceSpeed)
//      {
//        //starts on column 1, with loop having already gone 
//        //through once and upped counter
//        increaseBrightnessColumn(runningColumnsCounter); 
//        decreaseBrightnessColumn(runningColumnsCounter-1);
//      }
//      else
//      {
//        runningColumnsDelayStart = millis();
//        runningColumnsCounter++;
//
//        if (runningColumnsCounter >= NUM_LEDS-1)
//        {
//          runningColumnsForward = false;
//          runningColumnsCounter++;
//        }
//      }
//    }
//}

/////////////HSV Brightness patterns
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
void brightnessWaveHSV(int hue, int sat, int speed)
{
//  CHSV color(hue, sat, val);
//  color.val = 100;
//  fill(color);
//  FastLED.show();
//  //incrementing each column
//  
//  for (int i = 0; i<NUM_COLUMNS; i++)
//  {
//    //ups brightness
//    brightnessColumnsHSV( color, 150, i);
//    
//    FastLED.show();
//    delay(speed);
//  }

   bool timeElapsedGreaterThanEqualZero = millis()-brightnessWaveHSVDelayStart >= 0;
   bool timeElapsedLessThanEqualSpeed = millis()-brightnessWaveHSVDelayStart <= speed;
   CHSV color(hue, sat, 100);
  //starting condition
  if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && brightnessWaveHSVCounter == -1)
  {
    fill_ns(color);
  }
  //increment
  else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
  {
    brightnessColumnsHSV(color, 150, brightnessWaveHSVCounter);
  }
  else
  {
    brightnessWaveHSVDelayStart = millis();
    brightnessWaveHSVCounter++;

    if (brightnessWaveHSVCounter >= NUM_COLUMNS)
    {
      brightnessWaveHSVCounter = -1;
    }
  }
  
  FastLED.show();
}

/*
 * A brighter colored column trio (midbrightness-highbrightness-midbrightness moves across the dimmer panel starting at column 0, using HSV colors
 * @param speed is the delay for the moving columns
 */
void runningColumnsRightHSV(int hue, int sat, int speed)
{
//  int mediumBrightness = 75;
//  int highBrightness = 150;
//  CHSV color(hue, sat, val);
//  color.val = 100;
//  fill(color);
//  //incrementing each column
//  brightnessColumnsHSV(color, mediumBrightness, 0);
//  FastLED.show();
//  delay(speed);
//  brightnessColumnsHSV(color, mediumBrightness, 1);
//  brightnessColumnsHSV(color, highBrightness, 0);
//  FastLED.show();
//  delay(speed);
//  brightnessColumnsHSV(color, mediumBrightness, 2);
//  brightnessColumnsHSV(color, highBrightness, 1);
//  brightnessColumnsHSV(color, mediumBrightness, 0);
//  for (int i = 2; i<(NUM_COLUMNS-1); i++)
//  {
//    //ups brightness
//    brightnessColumnsHSV(color, mediumBrightness, (i+1));
//    brightnessColumnsHSV(color, highBrightness, i);
//    brightnessColumnsHSV(color, mediumBrightness, (i-1));
//    brightnessColumnsHSV(color, 0, (i-2));
//    FastLED.show();
//    delay(speed);
//  }
//  brightnessColumnsHSV(color, highBrightness, (NUM_COLUMNS-1));
//  brightnessColumnsHSV(color, mediumBrightness, (NUM_COLUMNS-2));
//  brightnessColumnsHSV(color, 0, (NUM_COLUMNS-3));
//  FastLED.show();
//  delay(speed);
//  brightnessColumnsHSV(color, mediumBrightness, (NUM_COLUMNS-1));
//  brightnessColumnsHSV(color, 0, (NUM_COLUMNS-2));
//  FastLED.show();
//  delay(speed);
//  brightnessColumnsHSV(color, 0, (NUM_COLUMNS-1));
//  FastLED.show();
//  delay(speed);
   bool timeElapsedGreaterThanEqualZero = millis()-runningColumnsRightHSVDelayStart >= 0;
   bool timeElapsedLessThanEqualSpeed = millis()-runningColumnsRightHSVDelayStart <= speed;
   int mediumBrightness = 75;
   int highBrightness = 150;
   CHSV color(hue, sat, 100);
   //starting increments
   if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsRightHSVCounter == -2)
   {
    fill_ns(color);
   }
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsRightHSVCounter == -1)
   {
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsRightHSVCounter+1);
   }
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsRightHSVCounter == 0)
   {
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsRightHSVCounter+1);
    brightnessColumnsHSV(color, highBrightness, runningColumnsRightHSVCounter);
   }
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsRightHSVCounter == 1)
   {
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsRightHSVCounter+1);
    brightnessColumnsHSV(color, highBrightness, runningColumnsRightHSVCounter);
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsRightHSVCounter-1);
   }
   //end conditions
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsRightHSVCounter == NUM_COLUMNS-1)
   {
    brightnessColumnsHSV(color, highBrightness, runningColumnsRightHSVCounter);
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsRightHSVCounter-1);
    brightnessColumnsHSV(color, 0, runningColumnsRightHSVCounter-2);
   }
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsRightHSVCounter == NUM_COLUMNS)
   {
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsRightHSVCounter-1);
    brightnessColumnsHSV(color, 0, runningColumnsRightHSVCounter-2);
   }
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsRightHSVCounter == NUM_COLUMNS+1)
   {
    brightnessColumnsHSV(color, 0, runningColumnsRightHSVCounter-2);
   }
   //main increment
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
   {
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsRightHSVCounter+1);
    brightnessColumnsHSV(color, highBrightness, runningColumnsRightHSVCounter);
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsRightHSVCounter-1);
    brightnessColumnsHSV(color, 0, runningColumnsRightHSVCounter-2);
   }
   else 
   {
    runningColumnsRightHSVDelayStart = millis();
    runningColumnsRightHSVCounter++;

    if (runningColumnsRightHSVCounter >= NUM_COLUMNS+2)
    {
      runningColumnsRightHSVCounter = -2;
      runningColumnsForward = false;
    }
   }
   FastLED.show();
  
}

/*
 * A brighter colored column trio (midbrightness-highbrightness-midbrightness moves across the dimmer panel starting at the highest index column, using HSV colors
 * @param speed is the delay for the moving columns
 */
void runningColumnsLeftHSV(int hue, int sat, int speed)
{
//  int mediumBrightness = 75;
//  int highBrightness = 150;
//  CHSV color(hue, sat, val);
//  color.val = 100;
//  fill(color);
//  FastLED.show();
//  //incrementing each column
//  brightnessColumnsHSV(color, mediumBrightness, (NUM_COLUMNS-1));
//  FastLED.show();
//  delay(speed);
//  brightnessColumnsHSV(color, mediumBrightness, (NUM_COLUMNS-2));
//  brightnessColumnsHSV(color, highBrightness, (NUM_COLUMNS-1));
//  FastLED.show();
//  delay(speed);
//  brightnessColumnsHSV(color, mediumBrightness, (NUM_COLUMNS-3));
//  brightnessColumnsHSV(color, highBrightness, (NUM_COLUMNS-2));
//  brightnessColumnsHSV(color, mediumBrightness, (NUM_COLUMNS-1));
//  for (int i = (NUM_COLUMNS-3); i>0; i--)
//  {
//    //ups brightness
//    brightnessColumnsHSV(color, mediumBrightness, (i-1));
//    brightnessColumnsHSV(color, highBrightness, i);
//    brightnessColumnsHSV(color, mediumBrightness, (i+1));
//    brightnessColumnsHSV(color, 0, (i+2));
//    FastLED.show();
//    delay(speed);
//  }
//  brightnessColumnsHSV(color, highBrightness, 0);
//  brightnessColumnsHSV(color, mediumBrightness, 1);
//  brightnessColumnsHSV(color, 0, 2);
//  FastLED.show();
//  delay(speed);
//  brightnessColumnsHSV(color, mediumBrightness, 0);
//  brightnessColumnsHSV(color, 0, 1);
//  FastLED.show();
//  delay(speed);
//  brightnessColumnsHSV(color, 0, 0);
//  FastLED.show();
//  delay(speed);
 bool timeElapsedGreaterThanEqualZero = millis()-runningColumnsLeftHSVDelayStart >= 0;
   bool timeElapsedLessThanEqualSpeed = millis()-runningColumnsLeftHSVDelayStart <= speed;
   int mediumBrightness = 75;
   int highBrightness = 150;
   CHSV color(hue, sat, 100);
   //starting increments
   if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsLeftHSVCounter == NUM_COLUMNS+1)
   {
    fill_ns(color);
   }
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsLeftHSVCounter == NUM_COLUMNS)
   {
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsLeftHSVCounter-1);
   }
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsLeftHSVCounter == NUM_COLUMNS-1)
   {
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsLeftHSVCounter-1);
    brightnessColumnsHSV(color, highBrightness, runningColumnsLeftHSVCounter);
   }
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsLeftHSVCounter == NUM_COLUMNS-2)
   {
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsLeftHSVCounter-1);
    brightnessColumnsHSV(color, highBrightness, runningColumnsLeftHSVCounter);
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsLeftHSVCounter+1);
   }
   //end conditions
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsLeftHSVCounter == 0)
   {
    brightnessColumnsHSV(color, highBrightness, runningColumnsLeftHSVCounter);
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsLeftHSVCounter+1);
    brightnessColumnsHSV(color, 0, runningColumnsLeftHSVCounter+2);
   }
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsLeftHSVCounter == -1)
   {
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsLeftHSVCounter+1);
    brightnessColumnsHSV(color, 0, runningColumnsLeftHSVCounter+2);
   }
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsLeftHSVCounter == -2)
   {
    brightnessColumnsHSV(color, 0, runningColumnsLeftHSVCounter+2);
   }
   //main increment
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
   {
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsLeftHSVCounter-1);
    brightnessColumnsHSV(color, highBrightness, runningColumnsLeftHSVCounter);
    brightnessColumnsHSV(color, mediumBrightness, runningColumnsLeftHSVCounter+1);
    brightnessColumnsHSV(color, 0, runningColumnsLeftHSVCounter+2);
   }
   else 
   {
    runningColumnsLeftHSVDelayStart = millis();
    runningColumnsLeftHSVCounter--;

    if (runningColumnsLeftHSVCounter <= -3)
    {
      runningColumnsLeftHSVCounter = NUM_COLUMNS+1;
      runningColumnsForward = true;
    }
   }
   FastLED.show();
}

runningColumnsHSV(int hue1, int sat1, int speed1, int hue2, int sat2, int speed2)
{
  if (runningColumnsForward)
  {
    runningColumnsRightHSV(hue1, sat1, speed1);
  }
  else 
  {
    runningColumnsLeftHSV(hue2, sat2, speed2);
  }
    
   
  
}


/** 
 * Makes all LEDs blink from the inputted color to black
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
  FastLED.show();
  leds[i] = CRGB::Black;
  FastLED.show();

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

  


/////////////////////////////////////////// LOOP //////////////////////////////////////////////////////

void loop() {

//   HSV running columns in red and blue
//   runningColumnsRightHSV(160, 255, 200, 50);
//   runningColumnsLeftHSV(245, 255, 200, 50);

//colliding columns
//   runningColumnsRightHSV(160, 255, 50);
//   runningColumnsLeftHSV(245, 255, 50);

//alternating columns
    runningColumnsHSV(160, 255, 50, 245, 255, 50);


   
  
}
