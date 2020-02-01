#include <SdFatHsmci.h>

 
// version 1.2 (to be removed for old versions 1.0 & 1.1 (old sandisk emmc)
// new version: emmc5.1
#define __WKAGENT1_2__ 

char test_file_name[] = "0:agent.txt";
Ctrl_status status;
FRESULT res;
FATFS fs;
FIL file_object;
TCHAR ReadData[100];
TCHAR SD_TimeStamp[12] = "Test Time\r\n";
TCHAR SD_Columns[15] = "Test Columns\r\n";
int i = 0;

void setup() {
  // Initialize the USB serial port (programming port)
  Serial.begin(115200);

  // Initialize HSMCI pin, SD_MMC stack
  HSMCI_init();

  Serial.println("\n\r-- SD/MMC/SDIO Card Example on FatFs --");
}

void loop() {
  unsigned char Hour=18,Minute=39,Second=10,Month=11,Day=6;
   int Year=2017;
   DWORD fre_clust, fre_sect, tot_sect;
   FATFS *fff;

  // Wait card present and ready 
  do {
    status = sd_mmc_test_unit_ready(0);
    if (CTRL_FAIL == status) {
      Serial.print("Card install FAIL\n\r");
      // Serial.print("Please unplug and re-plug the card.\n\r");
      while (CTRL_NO_PRESENT != sd_mmc_check(0)) {
      }
    }
  } 
  while (CTRL_GOOD != status);

  // read capacity
  uint32_t nb=sd_mmc_get_capacity(0);
  Serial.print(nb);  Serial.print(" KBytes\r\nWrite protected: ");
  // check WP
  Serial.println((sd_mmc_is_write_protected(0)?"YES":"NO"));  

// mount ---------
  Serial.print("Mount disk (f_mount)...");
  memset(&fs, 0, sizeof(FATFS));
  res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
  if (FR_INVALID_DRIVE == res) {
    printf(" [FAIL] res %d\r\n", res);
    goto main_end_of_test;
  Serial.print(" [OK]\r\n");

// reading disk
uint32_t l;
for(l=0L;l<nb/512L;l++) {
  
}


//  Serial.print("Create a file (f_open)...\r\n");
//  test_file_name[0] = LUN_ID_SD_MMC_0_MEM + '0';
//  goto main_end_of_test;

     
  Serial.print("Test is successful.\n\r");

main_end_of_test:
  // f_mount(0, "0:", 0); // UNMOUNT
  Serial.print("End of test.\n\r");
  while (1) { }
}

