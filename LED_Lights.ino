//Intake Strip Length 32 LEDS

//Intake Side Left Length 0-7

//Intake Front Left Length 8-15

//Intake Front Right Length 16-23

//Intake Side Right Length 24-32

#include "LPD8806.h"
#include "SPI.h"

//Hardware Pins
int clockPin = 3;
int dataPin1 = 11;

//Strip Length Data
int IntakeStripLength = 32;
int IntakeSideLeftPxZero = 0;
int IntakeSideLeftLength = 8;
int IntakeFrontLeftPxZero = 9;
int IntakeFrontLeftLength = 8;
int IntakeFrontRightPxZero = 17;
int IntakeFrontRightLength = 8;
int IntakeSideRightPxZero = 24;
int IntakeSideRightLength = 8;


LPD8806 strip(IntakeStripLength, dataPin1, clockPin);

void DisabledRobot();
void EnabledRobot();
void ShotReady();
void TuskPosition(int pos);
void HotGoal(int HotTarget);
int GetLEDStatus();
void InitPins();

int main()
{
  
  InitPins();
  
  while(true)
  {

    if(GetLEDStatus() == 1)
    {
      DisabledRobot();
    }  
    
    if(GetLEDStatus() == 2)
    {
      EnabledRobot(); 
    }
    
    if(GetLEDStatus() == 3)
    {
      ShotReady(); 
    }
    
    if(GetLEDStatus() == 4)
    {
      TuskPosition(GetLEDStatus()); 
    }
    
    if(GetLEDStatus() == 5)
    {
      TuskPosition(GetLEDStatus()); 
    }
    
    if(GetLEDStatus() == 6)
    {
      TuskPosition(GetLEDStatus()); 
    }
    
    if(GetLEDStatus() == 7)
    {
      HotGoal(GetLEDStatus());
    }
    
    if(GetLEDStatus() == 8)
    {
      HotGoal(GetLEDStatus());
    }
    
  }
  
}


void InitPins()
{
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);  
}

int GetLEDStatus()
{

  int retValue = 0;
  
  if(digitalRead(4) == LOW)
  {
     retValue = 1;   
  }
  
  if(digitalRead(4) == HIGH)
  {
     retValue = 2; 
  }
  
  if(digitalRead(5) == LOW)
  {
    retValue = 3;
  }  
  
  if(digitalRead(5) == HIGH)
  {
    retValue = 4;
  }  
  
  if(digitalRead(6) == LOW)
  {
    retValue = 5;
  }  
  
  if(digitalRead(6) == HIGH)
  {
    retValue = 6;
  }  
  
  if(digitalRead(7) == LOW)
  {
    retValue = 7;
  }  
  
  if(digitalRead(7) == HIGH)
  {
    retValue = 8;
  }  
  
  return retValue;
  
}

void DisabledRobot()
{

  for(int q = 0; q < strip.numPixels(); q++){
    strip.setPixelColor(q, strip.Color(256,0,0)); //Red
  }  
}

void EnabledRobot()
{
 
  for(int q = 0; q < strip.numPixels(); q++){
    strip.setPixelColor(q, strip.Color(0,256,0)); //Green
  }
  
}

void ShotReady()
{
  
  for(int q = 0; q < strip.numPixels(); q++){
    strip.setPixelColor(q, strip.Color(0,127,127)); //Cyan 
  }
  
}

void TuskPosition(int pos)
{
 
  //Tusks Retracted
  if(pos == 4)
  {
     for(int q = 0; q < strip.numPixels(); q++){
        strip.setPixelColor(q, strip.Color(127,127,0)); //Ugly Green
     }      
  }
  
  //Tusks Intermediate 
  if(pos == 5)
  {
    for(int q = 0; q < strip.numPixels(); q++){
      strip.setPixelColor(q, strip.Color(0,0,256)); //Blue
    } 
  }
  
  //Tusks Extended
  if(pos == 6)
  {
    for(int q = 0; q < strip.numPixels(); q++){
      strip.setPixelColor(q, strip.Color(255,165,0)); //Orange
    }   
  }
  
}

//Auto Mode
void HotGoal(int HotTarget)
{
  
  //Hot Goal Left
  if(HotTarget == 7)
  {
 
    for(int q = 0; q < strip.numPixels(); q++){
      strip.setPixelColor(q, strip.Color(256,0,0));
    }
    
  }
  
  //Hot Goal Right
  if(HotTarget == 8)
  {
     for(int q = 0; q < strip.numPixels(); q++){
       strip.setPixelColor(q, strip.Color(0,256,0));
     }
  }
  
}
