 
#include <FastLED.h>
#include <Wire.h>

#define NUM_LEDS 256
#define DATA_PIN 3
#define NUM_ROWS 8
#define NUM_COLUMNS 32
#define I2C_ID 8

CRGB leds[NUM_LEDS];

/** 
 *  For I2C communication
 */
int inputRoboRio = -1;
int toggleRoboRio = 99;
boolean messageReceived = false;
boolean newPattern = false;
byte hue1 = 0;
byte sat1 = 0;
byte val1 = 0;
int hue2 = 0;
byte sat2 = 0;
byte val2 = 0;

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
 * Dependencies for the rocket pattern
 */
 int rocketAscendsPatternCounter = 12; //change to zero to fly up through the bottom (NOT taking off from ground)
 int rocketDescendsPatternCounter = NUM_COLUMNS+11;
 unsigned long rocketDelayStart = 0.0;
 
                
/** 
 *  Dependency for the 2168 pattern
 */
 int pattern2168Counter = 0;
 unsigned long pattern2168DelayStart = 0.0;

/**
 * Dependency for the rainbow pattern
 */
uint8_t hue = 0;

/**
 * Dependency for the blink pattern
 */
 unsigned long blinkAllDelayStart = 0.0;

/**
 * Dependency for the confetti rainbow pattern
 */
 unsigned long confettiRainbowDelayStart = 0.0;

/**
 * Dependency for the animated wave counter
 */
unsigned long animatedWaveDelayStart = 0.0;
int animatedColumnCounter = 0;
int animatedRowCounter = 0;
bool upDownBool = true;

///**
// * Dependencies for the arrows patterns
// */
//int i = 0;
//int a = 0;
//int b = 0;
//int c = 0;
//int d = 0;
//unsigned long arrowsRightDelayStart = 0.0;
//unsigned long arrowsLeftDelayStart = 0.0;


///**
// * Dependencies for the intake pattern
// */
//int intakePatternCounter = 0;
//unsigned long intakePatternDelayStart = 0.0;
//int animation[19][9] = {{22, 21, 20, 23, 24, 68, 67, 66, 31}, {21, 20, 19, 24, 25, 67, 66, 65, 31}, {20, 19, 18, 25, 26, 66, 65, 64, 31}, {19, 18, 17, 26, 27, 65, 64, 63, 31}, {18, 17, 16, 27, 28, 64, 63, 62, 31}, {17, 16, 15, 28, 29, 63, 62, 61, 31}, {16, 15, 14, 29, 30, 62, 61, 60, 31}, {17, 16, 15, 28, 29, 63, 62, 61, 30}, {18, 17, 16, 27, 28, 64, 63, 62, 29}, {19, 18, 17, 26, 27, 65, 64, 63, 28}, {20, 19, 18, 25, 26, 66, 65, 64, 27}, {21, 20, 19, 24, 25, 67, 66, 65, 26}, {22, 21, 20, 23, 24, 68, 67, 66, 25}, {22, 21, 20, 23, 24, 68, 67, 66, 26}, {22, 21, 20, 23, 24, 68, 67, 66, 27}, {22, 21, 20, 23, 24, 68, 67, 66, 28}, {22, 21, 20, 23, 24, 68, 67, 66, 29}, {22, 21, 20, 23, 24, 68, 67, 66, 30}, {22, 21, 20, 23, 24, 68, 67, 66, 31}};
// 
//
// /**
// * Dependencies for the chase pattern
// */
//unsigned long chaseDelayStart = 0.0;
//int chaseCounter = -1;
//boolean chaseForward = true;
//
///**
// * Dependencies for the loading pattern
// */
//unsigned long loadingDelayStart = 0.0;
//int loadingCounter = -1;
//boolean loadingColor = true;
//
///**
// * Dependency for the cross pattern
// */
//unsigned long crossDelayStart = 0.0;
//
///**
// * Dependency for the police pattern
// */
//unsigned long policeDelayStart = 0.0;
//
///**
// * Dependencies for the leapfrog pattern
// */
//unsigned long leapfrogDelayStart = 0.0;
//int leapfrogCounter = 0;
//
///**
// * Dependency for the candy cane pattern
// */
//unsigned long candyCaneDelayStart = 0.0;


//////////////////////////////////////// SETUP /////////////////////////////////////////////

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(30);

  Wire.begin(I2C_ID);
  Wire.onReceive(receiveEvent1);
  Serial.begin(9600);
  
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
 * fills with a HSV color based on 3 input parameters
 */
 void fill(int hue, int sat, int val)
 {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, sat, val);
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

/**
 * Sets all LEDs to the inputted HSV color params without turning the LEDs on
 */
 void fill_ns(int hue, int sat, int val)
 {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(hue, sat, val);
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
  if(column < NUM_COLUMNS && column >= 0)
  {
    for (int i = NUM_ROWS*column; i<(NUM_ROWS*column+NUM_ROWS); i++)
    {
      leds[i] = color;
    }
  }
}

/*
 * The dimmer panel is filled with brighter columns, starting at column 0, using HSV color value
 * @param speed is a delay between before the addition of a new column
 */
void brightnessWaveHSV(int hue, int sat, int speed)
{

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
   bool timeElapsedGreaterThanEqualZero = millis()-runningColumnsRightHSVDelayStart >= 0;
   bool timeElapsedLessThanEqualSpeed = millis()-runningColumnsRightHSVDelayStart <= speed;
   int mediumBrightness = 75;
   int highBrightness = 150;
   CHSV color(hue, sat, 100);
  
   if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsRightHSVCounter == -2)
   {
    fill_ns(color);
   }
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
   {
   // brightnessHSV(color, 0);
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
bool timeElapsedGreaterThanEqualZero = millis()-runningColumnsLeftHSVDelayStart >= 0;
   bool timeElapsedLessThanEqualSpeed = millis()-runningColumnsLeftHSVDelayStart <= speed;
   int mediumBrightness = 75;
   int highBrightness = 150;
   CHSV color(hue, sat, 100);
  //starting condition
   if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed && runningColumnsLeftHSVCounter == NUM_COLUMNS+1)
   {
    fill_ns(color);
   }
   //main increment
   else if (timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
   {
   // brightnessHSV(color, 0);
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

void runningColumnsHSV(int hue1, int sat1, int speed1, int hue2, int sat2, int speed2)
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

//////////////Animations /////////////////////////////////////////
/**
 * @return the index of the LED pixel as an int based on inputted coordinates
 *          If the coordinates are outside the bounds of the panel, returns -1
 */
int coordinatesToLEDIndex(int column, int row)
{
  if (column < NUM_COLUMNS && row < NUM_ROWS)
  {
    if (column%2 == 0)
    {
      return NUM_ROWS*column + row;
    }
    else
    {
      return NUM_ROWS*column + (NUM_ROWS-1) - row;
    }
  }
  else 
  {
    //can catch this later and not turn on
    return -1;
  }
}

/**
 * Lights a specific LED pixel based on the coordinates in the specified color
 */
void fillLEDCoordinates(int column, int row, CHSV color)
{
  
  if (column < NUM_COLUMNS && column >= 0 && row < NUM_ROWS && row >= 0)
  {
    if (column%2 == 0)
    {
      leds[NUM_ROWS*column + row] = color;
    }
    else
    {
      leds[NUM_ROWS*column + (NUM_ROWS-1) - row] = color;
    }
  }
}

void fillRocketColumn(int rocketColumn, int destinationColumn, CHSV color)
{
  byte rocket[13][8] = {{10, 10, 10, 3, 4, 10, 10, 10}, //column 0
                        {10, 10, 2, 3, 4, 5, 10, 10}, //1
                        {10, 10, 2, 3, 4, 5, 10, 10}, //2
                        {10, 10, 2, 3, 4, 5, 10, 10}, //3
                        {10, 10, 2, 3, 4, 5, 10, 10}, //4
                        {10, 10, 2, 3, 4, 5, 10, 10}, //5
                        {10, 10, 2, 3, 4, 5, 10, 10}, //6
                        {10, 10, 2, 3, 4, 5, 10, 10}, //7
                        {10, 1, 2, 3, 4, 5, 6, 10}, //8
                        {0, 1, 2, 3, 4, 5, 6, 7}, //9
                        {10, 10, 2, 3, 4, 5, 10, 10}, //10
                        {10, 10, 2, 3, 4, 5, 10, 10}, //11
                        {10, 1, 10, 3, 4, 10, 6, 10}}; //12 
  //The rocket-if want to test different configs or colors  
  //    fillRocketColumn(0, 12, blue);
  //    fillRocketColumn(1, 11, blue);
  //    fillRocketColumn(2, 10, blue);
  //    fillRocketColumn(3, 9, blue);
  //    fillRocketColumn(4, 8, blue);
  //    fillRocketColumn(5, 7, blue);
  //    fillRocketColumn(6, 6, blue);
  //    fillRocketColumn(7, 5, blue);
  //    fillRocketColumn(8, 4, blue);
  //    fillRocketColumn(9, 3, blue);
  //    fillRocketColumn(10, 2, fire1);
  //    fillRocketColumn(11, 1, fire2);
  //    fillRocketColumn(12, 0, fire3);
  //    FastLED.show();                  
  fillLEDCoordinates(destinationColumn, rocket[rocketColumn][0], color);
  fillLEDCoordinates(destinationColumn, rocket[rocketColumn][1], color);
  fillLEDCoordinates(destinationColumn, rocket[rocketColumn][2], color);
  fillLEDCoordinates(destinationColumn, rocket[rocketColumn][3], color);
  fillLEDCoordinates(destinationColumn, rocket[rocketColumn][4], color);
  fillLEDCoordinates(destinationColumn, rocket[rocketColumn][5], color);
  fillLEDCoordinates(destinationColumn, rocket[rocketColumn][6], color);
  fillLEDCoordinates(destinationColumn, rocket[rocketColumn][7], color);
}

void rocketAscendsPattern(int hue, int sat, int val, int speed)
{
  CHSV fire1(9, 150, 200);
  CHSV fire2(7, 175, 200);
  CHSV fire3(7, 225, 200); 
  CHSV robotColor(hue, sat, val);
  bool timeElapsedGreaterThanEqualZero = millis()-rocketDelayStart >= 0;
  bool timeElapsedLessThanEqualSpeed = millis()-rocketDelayStart <= speed;
  //main increment
   if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
  {
    fill_ns(CRGB::Black);
    fillRocketColumn(0, rocketAscendsPatternCounter, robotColor);
    fillRocketColumn(1, rocketAscendsPatternCounter-1, robotColor);
    fillRocketColumn(2, rocketAscendsPatternCounter-2, robotColor);
    fillRocketColumn(3, rocketAscendsPatternCounter-3, robotColor);
    fillRocketColumn(4, rocketAscendsPatternCounter-4, robotColor);
    fillRocketColumn(5, rocketAscendsPatternCounter-5, robotColor);
    fillRocketColumn(6, rocketAscendsPatternCounter-6, robotColor);
    fillRocketColumn(7, rocketAscendsPatternCounter-7, robotColor);
    fillRocketColumn(8, rocketAscendsPatternCounter-8, robotColor);
    fillRocketColumn(9, rocketAscendsPatternCounter-9, robotColor);
    fillRocketColumn(10, rocketAscendsPatternCounter-10, fire1);
    fillRocketColumn(11, rocketAscendsPatternCounter-11, fire2);
    fillRocketColumn(12, rocketAscendsPatternCounter-12, fire3);
  }
  else
  {
    rocketDelayStart = millis();
    rocketAscendsPatternCounter++;

    if (rocketAscendsPatternCounter >= NUM_COLUMNS+12)
    {
      rocketAscendsPatternCounter = 0;
    }
  }
  FastLED.show();
  
  
}

void rocketDescendsPattern(int hue, int sat, int val, int speed)
{
  CHSV fire1(9, 150, 200);
  CHSV fire2(7, 175, 200);
  CHSV fire3(7, 225, 200); 
  CHSV robotColor(hue, sat, val);
  bool timeElapsedGreaterThanEqualZero = millis()-rocketDelayStart >= 0;
  bool timeElapsedLessThanEqualSpeed = millis()-rocketDelayStart <= speed;

  //main increment
   if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
  {
    fill_ns(CRGB::Black);
    fillRocketColumn(0, rocketDescendsPatternCounter, robotColor);
    fillRocketColumn(1, rocketDescendsPatternCounter-1, robotColor);
    fillRocketColumn(2, rocketDescendsPatternCounter-2, robotColor);
    fillRocketColumn(3, rocketDescendsPatternCounter-3, robotColor);
    fillRocketColumn(4, rocketDescendsPatternCounter-4, robotColor);
    fillRocketColumn(5, rocketDescendsPatternCounter-5, robotColor);
    fillRocketColumn(6, rocketDescendsPatternCounter-6, robotColor);
    fillRocketColumn(7, rocketDescendsPatternCounter-7, robotColor);
    fillRocketColumn(8, rocketDescendsPatternCounter-8, robotColor);
    fillRocketColumn(9, rocketDescendsPatternCounter-9, robotColor);
    fillRocketColumn(10, rocketDescendsPatternCounter-10, fire1);
    fillRocketColumn(11, rocketDescendsPatternCounter-11, fire2);
    fillRocketColumn(12, rocketDescendsPatternCounter-12, fire3);
  }
  else
  {
    rocketDelayStart = millis();
    rocketDescendsPatternCounter--;

    if (rocketDescendsPatternCounter < 0)//change to 12 to land on the ground
    {
      rocketDescendsPatternCounter = NUM_COLUMNS+11;
    }
  }
  FastLED.show();
  
  
}

/**
 * fills a column wtih the appropriate LEDs from a specified column2168 in a specific color
 */
void fill2168Column(int column2168, int destinationColumn, CHSV color)
{
  byte pattern2168LED[17][8] = {{10, 1, 2, 10, 10, 5, 6, 10}, //column 0::8
                    {0, 10, 10, 3, 4, 10, 10, 7}, //1
                    {0, 10, 10, 3, 4, 10, 10, 7}, //2
                    {0, 10, 10, 3, 4, 10, 10, 7}, //3
                    {10, 1, 2, 10, 10, 5, 6, 10}, //4

                    {10, 1, 2, 10, 4, 5, 6, 10}, //column 5::6
                    {0, 1, 10, 10, 4, 5, 6, 7}, //6
                    {0, 1, 10, 10, 4, 10, 10, 7}, //7
                    {0, 1, 10, 10, 4, 5, 6, 7}, //8
                    {10, 1, 2, 3, 4, 5, 6, 10}, //9

                    {0, 1, 2, 3, 4, 5, 6, 7}, //column 10::1
                    {0, 1, 2, 3, 4, 5, 6, 7}, //11
                    {0, 1, 10, 10, 10, 10, 10, 10}, //12
                  
                    {10, 1, 2, 3, 10, 10, 6, 7}, //column 13::2
                    {0, 1, 10, 3, 4, 10, 6, 7}, //14
                    {0, 1, 10, 10, 4, 5, 6, 7}, //15
                    {0, 1, 10, 10, 10, 5, 6, 10}}; //16

//         //test code to see the pattern stationary
//         fill2168Column(0, 28, blue);
//        fill2168Column(1, 27, blue);
//        fill2168Column(2, 26, blue);
//        fill2168Column(3, 25, blue);
//        fill2168Column(4, 24, blue);
//        fillColumn_ns(CRGB::Black, 23);
//        fillColumn_ns(CRGB::Black, 22);
//        fillColumn_ns(CRGB::Black, 21);
//        fill2168Column(5, 20, blue);
//        fill2168Column(6, 19, blue);
//        fill2168Column(7, 18, blue);
//        fill2168Column(8, 17, blue);
//        fill2168Column(9, 16, blue);
//        fillColumn_ns(CRGB::Black, 15);
//        fillColumn_ns(CRGB::Black, 14);
//        fillColumn_ns(CRGB::Black, 13);
//        fill2168Column(10, 12, blue);
//        fill2168Column(11, 11, blue);
//        fill2168Column(12, 10, blue);
//        fillColumn_ns(CRGB::Black, 9);
//        fillColumn_ns(CRGB::Black, 8);
//        fillColumn_ns(CRGB::Black, 7);
//        fill2168Column(13, 6, blue);
//        fill2168Column(14, 5, blue);
//        fill2168Column(15, 4, blue);
//        fill2168Column(16, 3, blue);
//        FastLED.show();
  fillLEDCoordinates(destinationColumn, pattern2168LED[column2168][0], color);
  fillLEDCoordinates(destinationColumn, pattern2168LED[column2168][1], color);
  fillLEDCoordinates(destinationColumn, pattern2168LED[column2168][2], color);
  fillLEDCoordinates(destinationColumn, pattern2168LED[column2168][3], color);
  fillLEDCoordinates(destinationColumn, pattern2168LED[column2168][4], color);
  fillLEDCoordinates(destinationColumn, pattern2168LED[column2168][5], color);
  fillLEDCoordinates(destinationColumn, pattern2168LED[column2168][6], color);
  fillLEDCoordinates(destinationColumn, pattern2168LED[column2168][7], color);
}

void pattern2168(int hue, int sat, int val, int speed)
{
  CHSV color(hue, sat, val);
  bool timeElapsedGreaterThanEqualZero = millis()-pattern2168DelayStart >= 0;
  bool timeElapsedLessThanEqualSpeed = millis()-pattern2168DelayStart <= speed;

  //main increment
  if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
  {
    fill_ns(CRGB::Black);
    fill2168Column(0, pattern2168Counter, color);
    fill2168Column(1, pattern2168Counter-1, color);
    fill2168Column(2, pattern2168Counter-2, color);
    fill2168Column(3, pattern2168Counter-3, color);
    fill2168Column(4, pattern2168Counter-4, color);
//    fillColumn_ns(CRGB::Black, pattern2168Counter-5);
//    fillColumn_ns(CRGB::Black, pattern2168Counter-6);
//    fillColumn_ns(CRGB::Black, pattern2168Counter-7);
    fill2168Column(5, pattern2168Counter-8, color);
    fill2168Column(6, pattern2168Counter-9, color);
    fill2168Column(7, pattern2168Counter-10, color);
    fill2168Column(8, pattern2168Counter-11, color);
    fill2168Column(9, pattern2168Counter-12, color);
//    fillColumn_ns(CRGB::Black, pattern2168Counter-13);
//    fillColumn_ns(CRGB::Black, pattern2168Counter-14);
//    fillColumn_ns(CRGB::Black, pattern2168Counter-15);
    fill2168Column(10, pattern2168Counter-16, color);
    fill2168Column(11, pattern2168Counter-17, color);
    fill2168Column(12, pattern2168Counter-18, color);
//    fillColumn_ns(CRGB::Black, pattern2168Counter-19);
//    fillColumn_ns(CRGB::Black, pattern2168Counter-20);
//    fillColumn_ns(CRGB::Black, pattern2168Counter-21);
    fill2168Column(13, pattern2168Counter-22, color);
    fill2168Column(14, pattern2168Counter-23, color);
    fill2168Column(15, pattern2168Counter-24, color);
    fill2168Column(16, pattern2168Counter-25, color);
  }
  else
  {
    pattern2168DelayStart = millis();
    pattern2168Counter++;

    if (pattern2168Counter >= NUM_COLUMNS+25)
    {
      pattern2168Counter = 0;
    }
  }
  FastLED.show();
}

void animatedWave(int hue, int sat, int val, int speed)
{
  CHSV color(hue, sat, val);
  bool timeElapsedGreaterThanEqualZero = millis()-animatedWaveDelayStart >= 0;
  bool timeElapsedLessThanEqualSpeed = millis()-animatedWaveDelayStart <= speed;
  if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
  {
    fill_ns(CRGB::Black);
    fillLEDCoordinates(animatedColumnCounter,animatedRowCounter, color);
    //fillLEDCoordinates(animatedColumnCounter+1, animatedRowCounter, color);
    if(animatedRowCounter == NUM_ROWS-2 && !upDownBool)
    {
      fillLEDCoordinates(animatedColumnCounter-1, animatedRowCounter+1, color);
      fillLEDCoordinates(animatedColumnCounter-2, animatedRowCounter, color);
    }
    else if(animatedRowCounter == 1 && upDownBool)
    {
      fillLEDCoordinates(animatedColumnCounter-1, animatedRowCounter-1, color);
      fillLEDCoordinates(animatedColumnCounter-2, animatedRowCounter, color);
    }
    else if (upDownBool)
    {
      fillLEDCoordinates(animatedColumnCounter-1, animatedRowCounter-1, color);
      fillLEDCoordinates(animatedColumnCounter-2, animatedRowCounter-2, color);
    } 
    else if (!upDownBool)
    {
      fillLEDCoordinates(animatedColumnCounter-1, animatedRowCounter+1, color);
      fillLEDCoordinates(animatedColumnCounter-2, animatedRowCounter+2, color);
    }
  }
  else 
  {
    animatedWaveDelayStart = millis();
    if (animatedRowCounter == NUM_ROWS-1){
      upDownBool = false;
    }
    else if (animatedRowCounter == 0){
      upDownBool = true;
    }
      
    if(upDownBool == true)
      animatedRowCounter++;
    else
      animatedRowCounter--;
      
    animatedColumnCounter++;
    if(animatedColumnCounter >= NUM_COLUMNS)
    {
      animatedColumnCounter = 0;
    }
    
  }
  FastLED.show();
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


///**
// * Makes an LED move, first going right to left and then going left to right
// */
//void chase(CRGB color, float speed) {
//  
//  bool timeElapsedGreaterThanEqualZero = millis()-chaseDelayStart >= 0;
//  bool timeElapsedGreaterThanZero = millis()-chaseDelayStart > 0;
//  bool timeElapsedLessThanEqualSpeed = millis()-chaseDelayStart <= speed;
//  bool timeElapsedGreaterThanSpeed = millis()-chaseDelayStart > speed;
//  bool timeElapsedLessThanEqualTwiceSpeed = millis()-chaseDelayStart <= 2*speed;
//  
//  if(chaseForward) {
//    if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed) {
//      leds[chaseCounter] = color;
//    }
//    else if(timeElapsedGreaterThanSpeed && timeElapsedLessThanEqualTwiceSpeed) {
//      leds[chaseCounter + 1] = color;
//      leds[chaseCounter] = CRGB::Black;
//    }
//    else {
//      chaseDelayStart = millis();
//      chaseCounter++;
//      
//      if(chaseCounter >= NUM_LEDS-1) {
//        chaseForward = false;
//      }
//    }
//    
//    FastLED.show();
//  }
//  else {
//    if(timeElapsedGreaterThanZero && timeElapsedLessThanEqualSpeed) {
//      leds[chaseCounter] = color;
//    }
//    else if(timeElapsedGreaterThanSpeed && timeElapsedLessThanEqualTwiceSpeed) {
//      leds[chaseCounter - 1] = color;
//      leds[chaseCounter] = CRGB::Black;
//    }
//    else {
//      chaseDelayStart = millis();
//      chaseCounter--;
//      
//      if(chaseCounter < 1) {
//        chaseForward = true;
//      }
//    }
//    
//    FastLED.show();
//  }
//  
//}
//
//
///**
// * Turns on each light one at a time
// */
//void loading(CRGB color, float speed) {
//  
//  bool timeElapsedGreaterThanEqualZero = millis()-loadingDelayStart >= 0;
//  bool timeElapsedLessThanEqualSpeed = millis()-loadingDelayStart <= speed;
//  
//  if(loadingColor) {
//    if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed) {
//      leds[loadingCounter] = color;    
//    }
//    else {
//      loadingDelayStart = millis();
//      loadingCounter++;
//      
//      if(loadingCounter > NUM_LEDS-1) {
//        loadingColor = false;
//        loadingCounter = 0;
//      }
//    }
//    
//    FastLED.show();
//  }
//  else {
//    if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed) {
//      leds[loadingCounter] = CRGB::Black;    
//    }
//    else {
//      loadingDelayStart = millis();
//      loadingCounter++;
//      
//      if(loadingCounter > NUM_LEDS-1) {
//        loadingColor = true;
//        loadingCounter = 0;
//      }
//    }
//    
//    FastLED.show();
//  }
//}




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

void confettiRainbow(int density)
{
  int numberOfLights = density;
  int speed = 30;
//  int i = random(0, NUM_LEDS - 1);
//  int colorHue = random(0, 255);
  
  bool timeElapsedGreaterThanEqualZero = millis()-confettiRainbowDelayStart >= 0;
  bool timeElapsedLessThanEqualSpeed = millis()-confettiRainbowDelayStart <= speed;
  bool timeElapsedGreaterThanSpeed = millis()-confettiRainbowDelayStart > speed;
  bool timeElapsedLessThanEqualTwiceSpeed = millis()-confettiRainbowDelayStart <= 2*speed;
  if(timeElapsedGreaterThanEqualZero && timeElapsedLessThanEqualSpeed)
  {
    for (int i = 0; i < numberOfLights; i++)
    {
      int j = random(0, NUM_LEDS - 1);
      int colorHue = random(0, 255);
      leds[j] = CHSV(colorHue, 255, 255);
    }
  }

   else if(timeElapsedGreaterThanSpeed && timeElapsedLessThanEqualTwiceSpeed)
  {
    off();
  }
  else
  {
    confettiRainbowDelayStart = millis();
  }

  FastLED.show();
}

 /**
 * Sets the variable inputRoboRio to the value the Arduino received from the RoboRio
 */
void receiveEvent1(int howMany) {
  while (7 < Wire.available()) {
    char c = Wire.read();
  }
  if(Wire.available() == 7)
  {
    hue2 = Wire.read();
    sat2 = Wire.read();
    val2 = Wire.read();
    hue1 = Wire.read();
    sat1 = Wire.read();
    val1 = Wire.read();
    inputRoboRio = Wire.read();
  }
  else if(Wire.available() < 7 && Wire.available() >= 4)
  {
    while (4 < Wire.available())
    {
      char c = Wire.read();
    }
    hue1 = Wire.read();
    sat1 = Wire.read();
    val1 = Wire.read();
    inputRoboRio = Wire.read();
  }
  else if(Wire.available() < 4 && Wire.available() >= 1)
  {
    while (1 < Wire.available())
    {
      char c = Wire.read();
    }
    inputRoboRio = Wire.read();
  }
  
  //inputRoboRio = x;
  
  messageReceived = true;

  Serial.println(inputRoboRio);
  Serial.print(hue1);
  Serial.print(sat1);
  Serial.println(val1);
  Serial.print(hue2);
  Serial.print(sat2);
  Serial.println(val2);
}


/////////////////////////////////////////// LOOP //////////////////////////////////////////////////////

void loop() {
    
  if(toggleRoboRio == 1)
    fill(hue1, sat1, val1);
  else if(toggleRoboRio == 99)
    fill(CRGB::Purple);
  else if(toggleRoboRio == 0)
    off();
  else if(toggleRoboRio == 2)
//    runningColumnsHSV(160, 255, 50, 245, 255, 50);
      runningColumnsHSV(hue1, sat1, 50, hue2, sat2, 50);
  else if(toggleRoboRio == 3)
  {
    //Coliding columns
    runningColumnsRightHSV(hue1, sat1, 50);
    runningColumnsLeftHSV(hue2, sat2 , 50);
  }
  else if(toggleRoboRio == 4)
    runningColumnsRightHSV(hue1, sat1, 50);
  else if(toggleRoboRio == 5)
    runningColumnsLeftHSV(hue1, sat1, 50);
  else if(toggleRoboRio == 6)
    rocketAscendsPattern(hue1, sat1, val1, 50);
  else if(toggleRoboRio == 7)
    rocketDescendsPattern(hue1, sat1, val1, 50);
  else if(toggleRoboRio == 8)
    pattern2168(hue1, sat1, val1, 50);
  else if(toggleRoboRio == 9)
    confettiRainbow(5);
  else if(toggleRoboRio == 10)
    animatedWave(hue1, sat1, val1, 40);


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
    if(toggleRoboRio == 2 || toggleRoboRio == 3 || toggleRoboRio == 4 || toggleRoboRio == 5) {
      runningColumnsRightHSVDelayStart = 0.0;
      runningColumnsRightHSVCounter = -2;
      runningColumnsLeftHSVDelayStart = 0.0;
      runningColumnsLeftHSVCounter = NUM_COLUMNS+1;
      runningColumnsForward = true;
    }
    else if(toggleRoboRio == 6 || toggleRoboRio == 7)
    {
      rocketAscendsPatternCounter = 12; //change to zero to fly up through the bottom
      rocketDescendsPatternCounter = NUM_COLUMNS+11;
      rocketDelayStart = 0.0;
    }
    else if(toggleRoboRio == 8)
    {
      pattern2168Counter = 0;
      pattern2168DelayStart = 0.0;
    }
    else if(toggleRoboRio == 10)
    {
      animatedColumnCounter = 0;
      animatedRowCounter = 0;
      animatedWaveDelayStart = 0.0;
    }
    newPattern = false;
  }
  
}
