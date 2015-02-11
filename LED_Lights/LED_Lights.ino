#include <SPI.h>
#include <FastLED.h>

#define STRIP_LENGTH 18

// Wiring (signals from cRIO)
#define inputPin1 2
#define inputPin2 3
#define inputPin3 4
#define inputPin4 5

//Strip communication pins
#define DATA_PIN  9

//Set up LED strip
CRGB leds[STRIP_LENGTH];



void OneToteAcquired();
void TwoToteAcquired();
void ThreeToteAcquired();
void FourToteAcquired();

int retValue = 0;
int lastretValue = 0;
    
void setup() {
   FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, STRIP_LENGTH);
  
  //light all pixels on the strip to verify they're working
   for(int q = 0; q < STRIP_LENGTH; q++) {
    leds[q].setRGB(127,127,127);
  }
  FastLED.show();
  delay(1500);
   
  //Turn the LEDs back off
  TurnOffLights();
  FastLED.show();

  // Note, pins 10 - 13 are in use by the SPI library. 
  pinMode(inputPin1, INPUT); 
  pinMode(inputPin2, INPUT); 
  pinMode(inputPin3, INPUT);  
  pinMode(inputPin4, INPUT);  

  Serial.begin(9600); // Serial Monitor
}

void loop() {
  
  //============================================
  //  Hex        TELEOP MODE               Bits#
  // Values                                3,2,1,0
  //============================================
  //  1       OneToteAcquired           // 0 0 0 1
  //  5       TwoToteAcquired           // 0 1 0 1
  //  2       ThreeToteAcquired         // 0 0 1 0
  //  6       FourToteAcquired          // 0 1 1 0
  //  3       UnknownTarget             // 0 0 1 1
  //  7       UnknownTarget2Ball        // 0 1 1 1
  //  0       Off                       // 0 0 0 0
  
  retValue = 0;

  //Read signals for cRIO
  if(digitalRead(inputPin1) == HIGH) { // if pin is high, return value is 1 (0001)
     retValue = retValue + 1;
  }
  
  if(digitalRead(inputPin2) == HIGH) { // if pin is high, return value is 2 (0010)
     retValue = retValue + 2;
  }

  if(digitalRead(inputPin3) == HIGH) { // if pin is high, return value is 4 (0100)
     retValue = retValue + 4;
  }
  
  if(digitalRead(inputPin4) == HIGH) { // if pin is high, return value is 8 (1000)
     retValue = retValue + 8;
  }
  
  if (retValue == 1){
    OneToteAcquired();
  }
  if (retValue == 5){
    TwoToteAcquired(); 
  }
  if (retValue == 2){
     ThreeToteAcquired(); 
  }
  if (retValue == 6) {
     FourToteAcquired(); 
  }

}
//*****************************************************//
// One Tote Aquired
//*****************************************************//
void OneToteAcquired () {
  for(int x = 1; x <= 3; x++) {
    leds[x].setRGB(115, 24, 44); //Maroon
  }
}
//*****************************************************//
// Two Tote Acquired
//*****************************************************//
void TwoToteAcquired () {
  for(int x = 0; x < 6; x++) {
     leds[x].setRGB(255, 0, 0); // Red 
  }
}
//*****************************************************//
// Three Tote Acquired
//*****************************************************//
void ThreeToteAcquired () {
  for(int x = 0; x <= 9; x++) {
     leds[x].setRGB(255, 0, 255); // Magenta/Fuschia 
  }
}
//*****************************************************//
// Four Tote Acquired
//*****************************************************//
void FourToteAcquired () {
  for(int x = 9;  x <= 12; x++) {
      leds[x].setRGB(255, 209, 220); // Pastel Pink 
  }
}

//*****************************************************//
// TurnOffLights
//*****************************************************//
void TurnOffLights () {
  for(int q = 0; q < STRIP_LENGTH; q++){
    leds[q].setRGB(0, 0, 0);
  }
}
