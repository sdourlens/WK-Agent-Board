#include "wf.h"
#include "ampli.h"

#define oneHzSample 1000000/maxSamplesNum  // sample for the 1Hz signal expressed in microseconds

volatile int wave0 = 0, wave1 = 0;
int i = 0;

void setup() {
  Ampli_init(1,1);
  Ampli_on(G0DB); //9
 // Ampli_off();
}

void loop() {
  analogWrite(DAC0, waveformsTable[wave0][i]);  // write the selected waveform on DAC0
  // analogWrite(DAC1, waveformsTable[wave1][i]);  // write the selected waveform on DAC1

  i++;  if(i == maxSamplesNum) i=0; // Reset the counter to repeat the wave

  delayMicroseconds(1);  // Hold the sample value for the sample time
}

