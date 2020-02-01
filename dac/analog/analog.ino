#include "ampli.h"

// T=6.28*us/alpha=1/F
// us=alpha/(6.28*F)

float phase = 0.0;
float twopi = 3.1415926 * 2;
float alpha=0.02; //us

// us=250 => T=20ms, F=50 Hz , +-10V

void setup() {
  analogWriteResolution(12);
  Ampli_off();
  Ampli_init(1,1);
  Ampli_on(G9DB);
}

void loop() {
  float val = sin(phase) * 2000.0 + 2050.0;
  analogWrite(66, (int)val);
  phase = phase + alpha;
  if (phase >= twopi) phase = 0;
  delayMicroseconds(500);
}
