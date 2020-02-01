/*
  Simple Waveform generator 
  WK Agent Board
  (c) WITTY KNOWLEDGE
  Sébastien Dourlens
  V1
  Nov. 2015 
 */

#include "waveforms.h"
#include "ampli.h"

volatile int wave0 = 0, wave1 = 0;
unsigned int t; 
int i=0;
int freq=10;
unsigned int sample;
int key = 0;
char spkleft=1,spkright=1;
double p;

void setup() {
  Ampli_init(spkleft,spkright);
  Ampli_off();
  analogWriteResolution(12);  // set the analog output resolution to 12 bit (4096 levels),  can be 8,10,12
  analogReadResolution(12);   // set the analog input resolution to 12 bit
  analogWrite(DAC0,0xfff);
  analogWrite(DAC1,0xfff);

  Serial.begin(115200);
  Serial.println("WaveForm T=1 s - F=1 Hz");
  Ampli_on(G9DB);
}

void loop() {
  // 1 Hz is the minimum freq for the complete wave
  // 170 Hz is the maximum freq for the complete wave. 

  // write the selected waveform on DAC0
  if (spkright) analogWrite(DAC0, waveformsTable[wave0][i]);  else analogWrite(DAC0,0xfff);
  // write the selected waveform on DAC1
  if (spkleft) analogWrite(DAC1, waveformsTable[wave1][i]); else analogWrite(DAC1,0xfff);
  
  if(++i == maxSamplesNum) {
    i=0;  // Reset the counter to repeat the wave
    
    while (Serial.available() > 0) {
      key = Serial.read();
      if (key=='4') Ampli_on(G18DB);
      if (key=='3') Ampli_on(G15DB);
      if (key=='2') Ampli_on(G12DB);
      if (key=='1') Ampli_on(G9DB);
      if (key=='0') Ampli_off();
      if (key=='m') { spkright=0; spkleft=0; }  // mute
      if (key=='s') { spkright=1; spkleft=1; } // stereo
      if (key=='l') { spkright=0; spkleft=1; }    // mono left
      if (key=='r') { spkright=1; spkleft=0; }      // mono right
      if (key=='+') freq+=1;   // increase sample of 1Hz 
      if (key=='-') freq-=1;   // decrease sample of 1Hz      
      if (key=='+' || key=='-') {
          if (freq==0) freq=1;
          p=1.0/freq;
          String s="New sample: "+String(p,6)+" s - Frequency: "+ freq +" Hz";
          Serial.println(s);
           Serial.println("Waiting ");
          }
      }
  }

  sample=(unsigned int) (1000000000.0/(freq*120.0));
  //delayMicroseconds(sample);  // Hold the sample value for the period time
   for(t=0;t<sample;t++); // delay ns
  // Serial.println(".");
}

