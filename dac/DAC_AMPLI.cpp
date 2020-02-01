#include <Audio.h>
#include <DAC.h>
#include <SPI.h>
#include <SD.h>
#include "Arduino.h"
#include "global.h"
#include "DAC_AMPLI.h"
#include "PINSMAPPING.h"



/*****************************************************************/
/************************* FONCTIONS DAC POUR AMPLI **************/
/*****************************************************************/

//-----------------  Initialize SPI and DAC -----------------
int init_AMPLI()
{
  if (!SD.begin(4)) {
    return(API_ERROR);
  }

 
  pinMode(PIN_AMPLI_A0,OUTPUT);
  pinMode(PIN_AMPLI_A1,OUTPUT);
  pinMode(PIN_AMPLI_SHDN,OUTPUT);
  pinMode(PIN_AMPLI_ENABLE,OUTPUT);
  
  digitalWrite(PIN_AMPLI_SHDN, HIGH);  //AMPLI Shut Downed
  digitalWrite(PIN_AMPLI_ENABLE, LOW); //GAIN DISABLED
  digitalWrite(PIN_AMPLI_A0, LOW);     //GAIN DISABLED
  digitalWrite(PIN_AMPLI_A1, LOW);     //GAIN DISABLED
  
  
  // hi-speed SPI transfers
  SPI.setClockDivider(4);
  
  // 44100Khz stereo => 88200 sample rate
  // 100 mSec of prebuffering.
  Audio.begin(88200, 100);
  return(API_OK);
}

//-----------------------PLAY sound file ---------------------------
int play_SoundFile(const char * soundfile, int interval_delay_ms, int number_of_readings, char selected_Gain)  //number  of readings = -1 for non stop
{
  // open wave file from sdcard
  
  if (selected_Gain<0 || selected_Gain>3)
  {
    return(BAD_ARG);
  }
  
  File myFile = SD.open(soundfile);
  if (!myFile) {
    return(API_ERROR);
  }
  
  digitalWrite(PIN_AMPLI_SHDN, LOW);  //AMPLI DISABLED
  digitalWrite(PIN_AMPLI_ENABLE, HIGH); //GAIN DISABLED

  switch(selected_Gain)
  {
    case (0):
      digitalWrite(PIN_AMPLI_A0, 0); //15dB
      digitalWrite(PIN_AMPLI_A1, 0);
    break;
    
    case (1):
      digitalWrite(PIN_AMPLI_A0, 1); //18dB
      digitalWrite(PIN_AMPLI_A1, 0);
    break;
    
    case (2):
      digitalWrite(PIN_AMPLI_A0, 0); //12dB
      digitalWrite(PIN_AMPLI_A1, 1);
    break;
    
    case (3):
      digitalWrite(PIN_AMPLI_A0, 1); //9dB
      digitalWrite(PIN_AMPLI_A1, 1);
    break;
  }
  
  int i=0;
  const int S = 1024; // Number of samples to read in block
  short buffer[S];

  // until the file is not finishe
  while(!STOP_SOUND && i!=number_of_readings)
  {
    while (myFile.available() && !STOP_SOUND) 
    {
      // read from the file into buffer
      myFile.read(buffer, sizeof(buffer));
  
      // Prepare samples
      int volume = 1024;
      Audio.prepare(buffer, S, volume);
      // Feed samples to audio
      Audio.write(buffer, S);
    }
    delay(interval_delay_ms);
    i++;
  }
  STOP_SOUND=0;
  myFile.close();
  digitalWrite(PIN_AMPLI_SHDN, HIGH);  //AMPLI Shut Downed
  digitalWrite(PIN_AMPLI_ENABLE, LOW); //GAIN DISABLED
  return (API_OK) ;
}
