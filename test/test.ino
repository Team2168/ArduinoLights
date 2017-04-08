#include <FastLED.h>  

#define LIGHTPIN 5                                                                                                                                                                                                    
#define LIGHTCOUNT 30


CRGB leds[LIGHTCOUNT];

void setup() {
  // put your setup code here, to run once:
    FastLED.addLeds<WS2812, LIGHTPIN>(leds, LIGHTCOUNT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < LIGHTCOUNT; i++){
    
      leds[i] = CRGB::White;
  }
  FastLED.show();
  delay(500);
  for (int i = 0; i < LIGHTCOUNT; i++){
      leds[i] = CRGB(0,0,255);
  }
  FastLED.show();
  delay(500);
}
