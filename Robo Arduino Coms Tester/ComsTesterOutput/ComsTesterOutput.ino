//Define COMS pins
#define OUT_PIN0 3
#define OUT_PIN1 4
#define OUT_PIN2 5
#define OUT_PIN3 6

//Setting the default value, and declaring the input holding variable.
int InByte = 0;

//Says whether bits 1-4 are on or off.
bool Bit1 = false;
bool Bit2 = false;
bool Bit3 = false;
bool Bit4 = false;

void setup() {
  //Start serial connection (to PC for setting output.)
   Serial.begin(9600);

  //Set output pins to output.
  pinMode(OUT_PIN0, OUTPUT);
  pinMode(OUT_PIN1, OUTPUT);
  pinMode(OUT_PIN2, OUTPUT);
  pinMode(OUT_PIN3, OUTPUT);
}

void loop() {
  //sets InByte to incoming byte.

  //if statement makes sure input is coming in.
  if(Serial.available() > 0){
    InByte = Serial.read();
  

  
   //takes in the serial number and 
   if(InByte == 49){
      if(Bit1){
        Bit1 = false;  
      }else if(!Bit1){
        Bit1 = true;
      }
      digitalWrite(OUT_PIN0, Bit1);
    }else if(InByte == 50){
       if(Bit2){
          Bit2 = false;  
       }else if(!Bit1){
          Bit2 = true;
       }
       digitalWrite(OUT_PIN1, Bit2);
    }else if(InByte == 51){
       if(Bit3){
         Bit3 = false;  
      }else if(!Bit3){
         Bit3 = true;
      }
         digitalWrite(OUT_PIN2, Bit3);
    }else if(InByte == 52){
      if(Bit4){
          Bit4 = false;  
      }else if(!Bit4){
          Bit4 = true;
      }
        digitalWrite(OUT_PIN3, Bit4);
    }
    
    Serial.print("Bit1: " +  ((String)Bit1) + "\n");
    Serial.print("Bit2: " +  ((String)Bit2) + "\n");
    Serial.print("Bit3: " +  ((String)Bit3) + "\n");
    Serial.print("Bit4: " +  ((String)Bit4) + "\n");
  }
}
