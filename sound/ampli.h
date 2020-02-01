//////////////// AMPLI & GAIN MANAGEMENT //////////////////////////

enum GAIN { G0DB=0,G15DB=4,G18DB=5,G12DB=3,G9DB=2,G6DB=1 };

#define AUDIO_ENABLE 12
#define AUDIO_A0     11
#define AUDIO_A1     30
#define AUDIO_SHUTDN 29
#define AUDIO_SWITCH 23 
#define AUDIO_DAC0   66
#define AUDIO_DAC1   67


// resolution: 8 to 12 bits
void ana_config(char resolution) {
  analogWriteResolution(resolution);  // set the analog output resolution to 12 bit (4096 levels)
  analogReadResolution(resolution);   // set the analog input resolution to 12 bit
}

// OE pin set on AUDIO Mode (CAN1 RX locked, TX not locked)
void DAC0AudioMode(){
  pinMode(AUDIO_SWITCH,OUTPUT); 
  digitalWrite(AUDIO_SWITCH,HIGH);
}

// let RX coming from transciever of can1 and going to processor (PB15) 
void DAC0CanMode(){
  pinMode(AUDIO_SWITCH,OUTPUT); 
  digitalWrite(AUDIO_SWITCH,LOW);
 // pinMode(AUDIO_DAC0, INPUT);  // for CAN1
}

void Ampli_init(char left,char right) {
  DAC0AudioMode();
  ana_config(12);
  pinMode(AUDIO_SHUTDN, OUTPUT);               // /SHUTDOWN=PD6 (D29)
  pinMode(AUDIO_A0, OUTPUT);               // Ampli. A0=pd7 (D11)
  pinMode(AUDIO_ENABLE, OUTPUT);               // ENABLE==pd8 (D12)
  pinMode(AUDIO_A1, OUTPUT);               // A1=pd9 (D30)
  if (right) pinMode(AUDIO_DAC0, OUTPUT);    // DAC0 = PB15 (D66)
  if (left) pinMode(AUDIO_DAC1, OUTPUT);     // DAC1 PB16 (D67)
}

void Ampli_on(char gain) {
  // SHUTDOWN off
  digitalWrite(AUDIO_SHUTDN,HIGH);
  // GAIN
  switch(gain) {
    case G0DB:
        digitalWrite(AUDIO_ENABLE, LOW);
        break;
    case G6DB:
        // A0 and A1 not connected
        digitalWrite(AUDIO_ENABLE, HIGH);
        break;              
    case G9DB:
        digitalWrite(AUDIO_A0, HIGH);
        digitalWrite(AUDIO_A1, HIGH);
        digitalWrite(AUDIO_ENABLE, HIGH);
        break;    
    case G12DB:
        digitalWrite(AUDIO_A0, LOW);
        digitalWrite(AUDIO_A1, HIGH);
        digitalWrite(AUDIO_ENABLE, HIGH);
        break;    
    case G15DB:
        digitalWrite(AUDIO_A0, LOW);
        digitalWrite(AUDIO_A1, LOW);
        digitalWrite(AUDIO_ENABLE, HIGH);
        break;    
    case G18DB:
        digitalWrite(AUDIO_A0, HIGH);
        digitalWrite(AUDIO_A1, LOW);
        digitalWrite(AUDIO_ENABLE, HIGH);
        break;  
  }
}

void Ampli_off() {
  pinMode(AUDIO_SHUTDN, OUTPUT);   // /SHUTDOWN=PD6 (D29)
  pinMode(AUDIO_ENABLE, OUTPUT);   // ENABLE==pd8 (D12)
  digitalWrite(AUDIO_SHUTDN,LOW);  // shutdown on
  digitalWrite(AUDIO_ENABLE,LOW);  // 0DB
}

