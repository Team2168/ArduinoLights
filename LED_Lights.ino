#include <SPI.h>

#include <LPD8806.h>



int clockPin = 2;
int dataPin1 = 11;

//Strip Length Data
int IntakeStripLength       = 18;
int IntakeSideLeftPxZero    = 0;    // 0
int IntakeSideLeftLength    = 2;    // 8
int IntakeFrontLeftPxZero   = 2;   // 9
int IntakeFrontLeftLength   = 2;   // 8
int IntakeFrontRightPxZero  = 4;  // 17 
int IntakeFrontRightLength  = 2;  // 8
int IntakeSideRightPxZero   = 6;   // 24
int IntakeSideRightLength   = 2;   // 8 

int i = 0,
    j = 0;
       
LPD8806 strip(IntakeStripLength, dataPin1, clockPin);

//void EnabledRobot();
//void ShotReady();
void TuskRetractPosition();
void TuskIntermediatePosition();
void TuskExtendedPosition();
void HotGoalLeft();
void HotGoalRight();
void UnknownTarget();
void DisabledRobot();

int retValue = 0;
int lastretValue = 0;

//*****************************************************************************/
    // Breathe Table - Brightness and Fading Time Cycle
    //*****************************************************************************/
    uint8_t LED_Breathe_Table[]  = 
    {  202, 211, 220, 228, 236, 242, 247, 251, 254, 255, 255, 254, 251, 247, 242, 236,
       228, 220, 211, 202, 192, 182, 172, 161, 151, 141, 131, 121, 112, 103,  95,  87,  
       80,  73,  66, 60,  55,  50,  45,  41,  38,  34,  31,  28,  26,  24,  22,  20,  
       20,  20,  20, 20,  20,  20,  20,  20,  20   };

    #define BREATHE_TABLE_SIZE (sizeof(LED_Breathe_Table))
    #define BREATHE_CYCLE 700 /*breathe cycle in milliseconds*/
    #define BREATHE_UPDATE (BREATHE_CYCLE / BREATHE_TABLE_SIZE)
    
    //*****************************************************************************/
    
void setup() {

  strip.begin();
  
  for(int q = 0; q < strip.numPixels(); q++) {
  strip.setPixelColor(q, strip.Color(0,127,0)); // green
  }
  strip.show();
  delay(1000);
   
   //Turn the LEDs back off
  for(int q = 0; q < strip.numPixels(); q++) {
    strip.setPixelColor(q, strip.Color(0,0,0));
  }
  strip.show();
  
  
  // switched from inputs to outputs for testing purposes
  // Change INPUT to OUTPUT to test different pinModes
  // Set digital pins 3-9 as inputs
  // Note, pins 10 - 13 are in use by the SPI library. 
  pinMode(3, INPUT); 
  pinMode(4, INPUT); 
  pinMode(5, INPUT);  
  pinMode(6, INPUT);  
  //pinMode(7, INPUT); 
  //pinMode(8, INPUT);
  //pinMode(9, INPUT);
  
  // for testing purpose
  // pinMode(13, OUTPUT); // set pin13 as OUTPUT 
  //digitalWrite(13, HIGH); // write 5V to PIN 8
  //digitalWrite(3, LOW);
  //digitalWrite(4, LOW);
  //digitalWrite(5, HIGH);
  //digitalWrite(6, LOW);
  //digitalWrite(7, LOW);
  //digitalWrite(8, LOW);
  //digitalWrite(9, LOW);  
  
   Serial.begin(9600); // Serial Monitor
}

void loop() {
  
  //=======================================
  //          AUTO MODE               Bits
  //=======================================            
  //          HotGoalLeft           // xx01
  //          HotGoalRight          // xx10
  //          UnknownTarget         // xx11
  //---------------------------------------
  //          Not in Auto**         // xx00 check teleop
  //======================================= 
  //          TELEOP MODE             Bits
  //=======================================
  //          Off                   // 0000 
  //          TuskExtendedPosition  // 1000
  //          TuskIntermediate      // 0100
  //          TuskRetractPosition   // 1100
  //======================================= 
  //                 PIN MAP
  //     TELEOP ->  X X | X X  <- AUTO
  //       PINS ->  6 5   4 3
  //=======================================
  
  retValue = 0;
  //Read signals for cRIO
   if(digitalRead(3) == HIGH) { // if 3 is high, return value is 1 (0001)
     retValue = retValue + 1;
   }
  //delay(200);
    Serial.println(retValue);  
   if(digitalRead(4) == HIGH) { // if 4 is high, return value is 2 (0010)
     retValue = retValue + 2;
  }
  //delay(200);
    Serial.println(retValue);
   if(digitalRead(5) == HIGH) { // if 5 is high, return value is 4 (0100)
     retValue = retValue + 4;
  }
  //delay(200);
    Serial.println(retValue);
   if(digitalRead(6) == HIGH) { // if 6 is high, return value is 8 (1000)
     retValue = retValue + 8;
  }
  
   i = 0; //If new data, restart the loop
 
   if((retValue & 0x0003) == 0x0003 ) {             // UnknownTarget
      UnknownTarget();
      Serial.println("UnknownTarget");         
   }
   else if(retValue & 0x0001) {            
      HotGoalLeft();                        // HotGoalLeft 
      Serial.println("HotGoalLeft");
   }
   //===================================================
   else if(retValue & 0x0002) {             // HotGoalRight
      HotGoalRight();
      Serial.println("HotGoalRight");              
   } 
   //=================================================== 
   else if((retValue & 0x000C) == 0x000C) { // TuskRetractPosition
      TuskRetractPosition();
      Serial.println("TuskRetractPosition");    
   }
   //===================================================
   else if(retValue & 0x0004) {             // TuskIntermediatePosition
      TuskIntermediatePosition();
      Serial.println("TuskIntermediatePosition");    
   }
   //===================================================
   else if(retValue & 0x0008) {             // TuskExtendedPosition
      TuskExtendedPosition();
      Serial.println("TuskExtendedPosition");    
   }
   else 
   {
      DisabledRobot();
      Serial.println("DisabledRobot"); 
   }
   strip.show();
  }
  
//======================================================//
// Shot Ready
//======================================================//
/*void ShotReady() {
// Left Side of Intake Arm in solid blue 
 for(int q = 0; q < IntakeSideLeftLength; q++){
   strip.setPixelColor(q + IntakeSideLeftPxZero, strip.Color(0,0,127));  
 }
// Right Side of Intake Arm in solid blue
 for(int q = 0; q < IntakeSideRightLength; q++){
   strip.setPixelColor(q + IntakeSideRightPxZero, strip.Color(0,0,127)); 
 }
 
// LED pulsation in Red for LEDs 3 through 6 
 for (int cycle1 = 0; cycle1 < 1; cycle1++) {
   uniformBreathe1((LED_Breathe_Table ), BREATHE_TABLE_SIZE, BREATHE_UPDATE, 127, 2, 2);
 }
}

// Breathing Segment for IntakeFrontRight and IntakeFrontLeft pulsation  
void uniformBreathe1(uint8_t* breatheTable, uint8_t breatheTableSize, uint16_t updatePeriod, uint16_t r, uint16_t g, uint16_t b)
 {
   int i;
   uint8_t breatheIndex = 0;
   uint8_t breatheRed;
   uint8_t breatheGrn;
   uint8_t breatheBlu;
     
   for (breatheIndex = 0; breatheIndex < breatheTableSize; breatheIndex++) {
     for (i=0; i < IntakeFrontRightLength; i++) {
       breatheRed = (r * breatheTable[breatheIndex]) / 256;
       breatheGrn = (g * breatheTable[breatheIndex]) / 256;
       breatheBlu = (b * breatheTable[breatheIndex]) / 256;
       strip.setPixelColor(i + IntakeFrontRightPxZero, breatheRed, breatheGrn, breatheBlu);
     }
    
     for (i=0;  i < IntakeFrontLeftLength; i++) {
       breatheRed = (r * breatheTable[breatheIndex]) / 256;
       breatheGrn = (g * breatheTable[breatheIndex]) / 256;
       breatheBlu = (b * breatheTable[breatheIndex]) / 256;
       strip.setPixelColor(i + IntakeFrontLeftPxZero, breatheRed, breatheGrn, breatheBlu);
     }
     strip.show();   // write all the pixels out
     delay(updatePeriod);    
  }
 }
 */
//======================================================//
// Tusk Retracted
//======================================================//
void TuskRetractPosition() {
  for(int x = 0; x < strip.numPixels(); x++) {
   strip.setPixelColor(x, strip.Color(0,0,127));        // Blue
  }
}   
//======================================================//
// Tusk Intermediate Position
//======================================================//
void TuskIntermediatePosition() {  
  for(int q = 0; q < strip.numPixels(); q++){
    strip.setPixelColor(q, strip.Color(127,127,0));     // Yellow    
  } 
} 
//======================================================//
// Tusk Extended Position
//======================================================//
void TuskExtendedPosition() {
  for(int q = 0; q < strip.numPixels(); q++){
    strip.setPixelColor(q, strip.Color(127,0,0));       // Red
  }
}
//======================================================//
// Auto Mode - Hot Goal Left
//======================================================// 
void HotGoalLeft() {
  for(int q = 0; q < (strip.numPixels())/2; q++){
    strip.setPixelColor(q, strip.Color(0,127,0));       // Green
  }
  for(int q = strip.numPixels()/2; q < (strip.numPixels()); q++){
    strip.setPixelColor(q, strip.Color(0,0,0)); 
  }
}
//======================================================//
// Auto Mode - Hot Goal Right
//======================================================// 
void HotGoalRight() {
    for(int q = strip.numPixels()/2; q < (strip.numPixels()); q++){
    strip.setPixelColor(q, strip.Color(127,0,0)); 
    }
  for(int q = 0; q < strip.numPixels()/2; q++){
    strip.setPixelColor(q, strip.Color(0,0,0));       // Red
  }
}
//======================================================//
// Auto Mode - Unknown Hot Goal Target
//======================================================//
void UnknownTarget() {
  for(int q = 0; q < strip.numPixels(); q++) {
    strip.setPixelColor(q, strip.Color(127,60,0));      // Orange
  }
}
//======================================================//
// Disabled Robot
//======================================================//
void DisabledRobot() {
  for(int q = 0; q < strip.numPixels(); q++){
    strip.setPixelColor(q, strip.Color(0,0,0));         // Off
  }  
}
//======================================================// 

/* 
 void sequencedBreathe(uint8_t* breatheTable, uint8_t breatheTableSize, uint16_t updatePeriod, uint16_t r, uint16_t g, uint16_t b)
 {
   int i;
   uint8_t breatheIndex = 0;
   uint8_t breatheRed;
   uint8_t breatheGrn;
   uint8_t breatheBlu;
   uint8_t sequenceIndex;
 
   for (breatheIndex = 0; breatheIndex < breatheTableSize; breatheIndex++) {
     for (i=0; i < IntakeSideLeftLength; i++) {
       sequenceIndex = (breatheIndex + (i*4)) % breatheTableSize;
       breatheRed = (r * breatheTable[sequenceIndex]) / 256;
       breatheGrn = (g * breatheTable[sequenceIndex]) / 256;
       breatheBlu = (b * breatheTable[sequenceIndex]) / 256;
       strip.setPixelColor(i + IntakeSideLeftPxZero, breatheRed, breatheGrn, breatheBlu);
     }
     strip.show(); 

     for (i=0; i < IntakeSideLeftLength; i++) {
       sequenceIndex = (breatheIndex + (i*4)) % breatheTableSize;
       breatheRed = (r * breatheTable[sequenceIndex]) / 256;
       breatheGrn = (g * breatheTable[sequenceIndex]) / 256;
       breatheBlu = (b * breatheTable[sequenceIndex]) / 256;
       strip.setPixelColor(i + IntakeSideLeftPxZero, breatheRed, breatheGrn, breatheBlu);
     }
     strip.show();
        
     delay(updatePeriod);
   }
 }
 
//*****************************************************************************/
