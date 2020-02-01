
 #include <SdFatHsmci.h>

 void HSMCI_init(void)
{
  /* Configure HSMCI pins */
  PIO_Configure(g_APinDescription[MCCDA_Pin].pPort, PIO_PERIPH_A, g_APinDescription[MCCDA_Pin].ulPin, PIO_DEFAULT);  // Set pin for Peripheral A functionality (HSMCI)
  PIO_Configure(g_APinDescription[MCCK_Pin].pPort, PIO_PERIPH_A, g_APinDescription[MCCK_Pin].ulPin, PIO_DEFAULT);    // Set pin for Peripheral A functionality (HSMCI)
  PIO_Configure(g_APinDescription[MCDA0_Pin].pPort, PIO_PERIPH_A, g_APinDescription[MCDA0_Pin].ulPin, PIO_DEFAULT);    // Set pin for Peripheral A functionality (HSMCI)
  PIO_Configure(g_APinDescription[MCDA1_Pin].pPort, PIO_PERIPH_A, g_APinDescription[MCDA1_Pin].ulPin, PIO_DEFAULT);    // Set pin for Peripheral A functionality (HSMCI)
  PIO_Configure(g_APinDescription[MCDA2_Pin].pPort, PIO_PERIPH_A, g_APinDescription[MCDA2_Pin].ulPin, PIO_DEFAULT);    // Set pin for Peripheral A functionality (HSMCI)
  PIO_Configure(g_APinDescription[MCDA3_Pin].pPort, PIO_PERIPH_A, g_APinDescription[MCDA3_Pin].ulPin, PIO_DEFAULT);    // Set pin for Peripheral A functionality (HSMCI)
  //PIO_Configure(g_APinDescription[SD_CD_Pin].pPort, PIO_INPUT, g_APinDescription[SD_CD_Pin].ulPin, PIO_PULLUP);    // Set card detect pin as input with pullup enabled

  // NEXT LINES VERY IMPORTANT
  digitalWrite(MCCDA_Pin, HIGH);
  digitalWrite(MCDA0_Pin, HIGH);
  digitalWrite(MCDA1_Pin, HIGH);
  digitalWrite(MCDA2_Pin, HIGH);
  digitalWrite(MCDA3_Pin, HIGH);
  
  /* Initialize SD MMC stack */
  sd_mmc_init();
}