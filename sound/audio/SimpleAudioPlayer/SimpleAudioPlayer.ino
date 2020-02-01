/*
  Simple Audio Player

  Demonstrates the use of the Audio library for the WK Agent

  Sébastien Dourlens
  9/11/2015
  v1.0

  Requirements:
    Speakers 8 ohms, not 4 ohms
    Don't use DAC0 with CAN2, CAN1 yes !
*/

#include "Audio.h"
#include "ampli.h"
#include "music.h"

void setup()
{
  Ampli_init(1,1);
  Ampli_off();

  // debug output at 9600 baud
  Serial.begin(115200);

  // setup SD-card
  Serial.print("Initializing Audio...");
  
  // 44100Khz stereo => 88200 sample rate
  // 8 Khz => 16384
  // 100 mSec of prebuffering.
  Audio.begin(88200, 100); 
}

void loop()
{
  int sizefile = 1192; // Number of samples to read in block

  Serial.print("Playing");  
  Ampli_on(G9DB);
  
  // Prepare samples
  int volume = 1024;
  Audio.prepare(music, sizefile, volume);
  
  // Feed samples to audio
  Audio.write(F(music), sizefile);

  Serial.println("The end!");
  Ampli_off();
  
  while (true) ;
}

