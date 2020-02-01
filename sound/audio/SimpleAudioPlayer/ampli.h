//////////////// AMPLI & GAIN MANAGEMENT //////////////////////////
#include <Arduino.h>

enum GAIN { G0DB=0,G15DB=4,G18DB=5,G12DB=3,G9DB=2,G6DB=1 };

// resolution: 8 to 12 bits
void ana_config(char resolution) {
  analogWriteResolution(resolution);  // set the analog output resolution to 12 bit (4096 levels)
  analogReadResolution(resolution);   // set the analog input resolution to 12 bit
}

// OE pin set on AUDIO Mode (CAN1 RX lock, TX not lock)
void DAC0AudioMode(){
  pinMode(23,OUTPUT); 
  digitalWrite(23,HIGH);
}

// let RX coming from transciever of can1 and going to processor (PB15) 
void DAC0CanMode(){
  pinMode(23,OUTPUT); 
  digitalWrite(23,LOW);
 // pinMode(66, INPUT);   pinMode(88, INPUT); 
}

void Ampli_init(char left,char right) {
  DAC0AudioMode();
  ana_config(12);
  pinMode(29, OUTPUT);               // /SHUTDOWN=PD6 (D29)
  pinMode(11, OUTPUT);               // Ampli. A0=pd7 (D11)
  pinMode(12, OUTPUT);               // ENABLE==pd8 (D12)
  pinMode(30, OUTPUT);               // A1=pd9 (D30)
  if (right) pinMode(66, OUTPUT);    // DAC0 = PB15 (D66)
  if (left) pinMode(67, OUTPUT);     // DAC1 PB16 (D67)
}

void Ampli_on(char gain) {
  // SHUTDOWN off
  digitalWrite(29,HIGH);
  // GAIN
  switch(gain) {
    case G0DB:
        digitalWrite(12, LOW); // off
        break;
    case G6DB:
        // A0 and A1 not connected
        digitalWrite(12, HIGH);
        break;              
    case G9DB:
        digitalWrite(11, HIGH);
        digitalWrite(12, HIGH);
        digitalWrite(30, HIGH);
        break;    
    case G12DB:
        digitalWrite(11, LOW);
        digitalWrite(12, HIGH);
        digitalWrite(30, HIGH);
        break;    
    case G15DB:
        digitalWrite(11, LOW);
        digitalWrite(12, HIGH);
        digitalWrite(30, LOW);
        break;    
    case G18DB:
        digitalWrite(11, HIGH);
        digitalWrite(12, HIGH);
        digitalWrite(30, LOW);
        break;  
  }
}

// use after ampli_init only
void Ampli_off() {
  pinMode(29, OUTPUT);               // /SHUTDOWN=PD6 (D29)
  pinMode(12, OUTPUT);               // ENABLE==pd8 (D12)
  digitalWrite(29,LOW); // shutdown on
  digitalWrite(12,LOW);  // 0DB
}

