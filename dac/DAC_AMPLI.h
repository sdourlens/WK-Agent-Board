
#ifndef DAC_AMPLI_h
#define DAC_AMPLI_h

/****************************************************************************/
/************************* FONCTIONS DAC pour AMPLI *************************/
/****************************************************************************/

int init_AMPLI();

int play_SoundFile(const char * soundfile, int interval_delay_ms, int number_of_readings);

#endif
