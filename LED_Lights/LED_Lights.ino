//Note, the TM1803V2.0 strip I'm using looks like it takes values
//  in the form 0xRRBBGG, doesn't look like FAST LED is working
//  correctly out of the box w/ appropriate colors on this strip.

#include <SPI.h>
#include <FastLED.h>

#define NUM_LEDS 20
#define PATTERN_REPLAY 1
#define NUM_PATTERNS 10

#define DATA_PIN 3
#define PAUSE_LOOP_PIN 2

//Set up LED strip
CRGB leds[NUM_LEDS];

int pattern = 0;

void setup() {
  pinMode(PAUSE_LOOP_PIN, INPUT_PULLUP);
  
  FastLED.addLeds<TM1803, DATA_PIN, GBR>(leds, NUM_LEDS);
  
  //light all pixels on the strip to verify they're working
  LEDS.showColor(CRGB::White);
  delay(1500);

  //Turn the LEDs back off
  TurnOffLights();
  FastLED.show();
}

void loop() {
  //Play through the list of patterns 
  for(int i = 0; i < PATTERN_REPLAY; i++) {
    switch(pattern){
      case 0:
        LEDS.showColor(CRGB::Red);
        delay(10000);
        break;
      case 1:
        LEDS.showColor(CRGB::Blue);
        delay(10000);
        break;
      case 2:
        fadeUpDown(10, 10);
        break;
      case 3:
        twinkleRand(2, CRGB::White, CRGB::Red, 1000, 40);
        break;
      case 4:
        simpleWave(0.03,150,10);
        break;
      case 5:
        rainbow(0.03,150,10);
        break;
      case 6:

        break;
      case 7:
      case 8:
      default:
        //fire();
        break;
    }
  }
  
  //Only move on to the next pattern if the jumper is removed.
  if(digitalRead(PAUSE_LOOP_PIN) == HIGH) {
    pattern++;
    pattern = (pattern % NUM_PATTERNS);
  }
}

//======================================================//
// TurnOffLights
//======================================================//
void TurnOffLights() {
  for(int q = 0; q < NUM_LEDS; q++){
    leds[q] = 0x000000;
  }  
}

void fadeUpDown(int wait, int loops) {
  for(int i = 0; i < loops; i++) {
    for(int x = 0; x < 128; x++) { 
      LEDS.showColor(CRGB(x, 0, 0));
      delay(wait);
    }

    // fade down
    for(int x = 128; x >= 0; x--) { 
      LEDS.showColor(CRGB(x, 0, 0));
      delay(wait);
    }
  }
}

// twinkle random number of pixels
// number, twinkle color, background color, delay
// twinkleRand(5,strip.Color(255,255,255),strip.Color(255, 0, 100),90);
void twinkleRand(int num, CRGB c, CRGB bg, int wait, int loops) {
  for(int x = 0; x < loops; x++){
    // set background
    for(int i=0; i<NUM_LEDS; i++){
      leds[i] = bg;
    }
    // for each num
    for (int i=0; i<num; i++) {
      leds[random(NUM_LEDS)] = c;
    }
    FastLED.show();
    delay(wait);
  }
}

// very simple wave: velocity, cycles,delay between frames
// simpleWave(0.03,5,10);
void simpleWave(float rate,int cycles, int wait) {
  float pos=0.0;
  // cycle through x times
  for(int x=0; x<(NUM_LEDS*cycles); x++)
  {
      pos = pos+rate;
      for(int i=0; i<NUM_LEDS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        float level = sin(i+pos) * 127 + 128;
        // set color level 
        leds[i].setRGB((int)level,0,0);
      }
      FastLED.show();
      delay(wait);
  }
}  

void rainbow(float rate,int cycles, int wait) {
  float pos=0.0;
  float levelR, levelG, levelB;
  // cycle through x times
  for(int x=0; x<(NUM_LEDS*cycles); x++)
  {
      pos = pos+rate;
      for(int i=0; i<NUM_LEDS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        levelR = sin(i+pos) * 127 + 128;
        levelG = sin(i+pos+120) * 127;
        levelB = sin(i+pos+240) * 127 + 128;
        // set color level 
        leds[i].setRGB((int)levelR,(int)levelG,(int)levelB);
      }
      FastLED.show();
      delay(wait);
  }
}

// CRGB HeatColor( uint8_t temperature)
// [to be included in the forthcoming FastLED v2.1]
//
// Approximates a 'black body radiation' spectrum for
// a given 'heat' level.  This is useful for animations of 'fire'.
// Heat is specified as an arbitrary scale from 0 (cool) to 255 (hot).
// This is NOT a chromatically correct 'black body radiation'
// spectrum, but it's surprisingly close, and it's extremely fast and small.
//
// On AVR/Arduino, this typically takes around 70 bytes of program memory,
// versus 768 bytes for a full 256-entry RGB lookup table.
 
CRGB HeatColor( uint8_t temperature)
{
  CRGB heatcolor;
 
  // Scale 'heat' down from 0-255 to 0-191,
  // which can then be easily divided into three
  // equal 'thirds' of 64 units each.
  uint8_t t192 = scale8_video( temperature, 192);
 
  // calculate a value that ramps up from
  // zero to 255 in each 'third' of the scale.
  uint8_t heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // now figure out which third of the spectrum we're in:
  if( t192 & 0x80) {
    // we're in the hottest third
    heatcolor.r = 255; // full red
    heatcolor.g = 255; // full green
    heatcolor.b = heatramp; // ramp up blue
   
  } else if( t192 & 0x40 ) {
    // we're in the middle third
    heatcolor.r = 255; // full red
    heatcolor.g = heatramp; // ramp up green
    heatcolor.b = 0; // no blue
   
  } else {
    // we're in the coolest third
    heatcolor.r = heatramp; // ramp up red
    heatcolor.g = 0; // no green
    heatcolor.b = 0; // no blue
  }
 
  return heatcolor;
}

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  50

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 100

void Fire2012()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 3; k > 0; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
        leds[j] = HeatColor( heat[j]);
    }
}

#define FRAMES_PER_SECOND 20
void fire() {
  for(int i = 0; i < ((1000 / FRAMES_PER_SECOND) * 10); i++) {
    // Add entropy to random number generator; we use a lot of it.
    random16_add_entropy( random());

    Fire2012(); // run simulation frame
  
    FastLED.show(); // display this frame
    delay(1000 / FRAMES_PER_SECOND);
  }
}
