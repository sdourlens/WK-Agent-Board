// audio.h

#include "wf.h"
#include "ampli.h"

#define AUDIO_ENABLE 12
#define AUDIO_A0     11
#define AUDIO_A1     30
#define AUDIO_SHUTDN 29
#define AUDIO_SWITCH 23 
#define AUDIO_DAC0   66
#define AUDIO_DAC1   67

#define oneHzSample 1000000/maxSamplesNum  // sample for the 1Hz signal expressed in microseconds

volatile int wave0 = 0, wave1 = 0;

void init_audio() {
  Serial.println("Test Audio (loudspeakers should be connected, DAC0 is in audio mode)\nPress any key to stop");
  Ampli_init(1,1);
  Ampli_on(G9DB); 
}

void test_audio() {
int i=0;
	while(Serial.available()==0) {
	  analogWrite(DAC0, waveformsTable[wave0][i]);  // write the selected waveform on DAC0
	  analogWrite(DAC1, waveformsTable[wave1][i]);  // write the selected waveform on DAC1

	  i++;  if(i == maxSamplesNum) i=0; // Reset the counter to repeat the wave

	  delayMicroseconds(9);  // Hold the sample value for the sample time
	}
	Serial.flush();
}

void reset_audio() {
	Ampli_off();
}
