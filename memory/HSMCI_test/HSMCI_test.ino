#include <SdFatHsmci.h>

// to display on USB PROG : use Serial
// to display on USB : use SerialUSB
 
// version 1.2 (to be removed for old versions 1.0 & 1.1 (old sandisk emmc)
// new version: emmc5.1 
#define __WKAGENT1_2__ 

#define FORMATFIRST 0

char test_file_name[] = "0:wkagent.txt";
Ctrl_status status;
FRESULT res;
FATFS fs;
FIL file_object;
TCHAR ReadData[10];
TCHAR SD_TimeStamp[12] = "Test Time\r\n";
TCHAR SD_Columns[15] = "Test Columns\r\n";
char filepath[300]="0:/";
int i = 0;

void setup() {
  // Initialize the USB serial port (programming port)
  Serial.begin(115200);

  // Initialize HSMCI pin, SD_MMC stack
  HSMCI_init();

  Serial.println("\n\r-- eMMC HSMCI Card Example with FatFs --");
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
      delay(200); Serial.print("Card install FAIL\n\r"); delay(200);
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
    printf("[FAIL] MOUNT res %d\r\n", res);
    goto main_end_of_test;
  }
  // TEST SD
  if (res!=FR_OK) {
    printf("[FAIL] MOUNT res %d\r\n", res);
    goto main_end_of_test;
  }
  Serial.print("[OK]\r\n");

#if (FORMATFIRST)
  // format
 Serial.print("Format disk (f_mkfs)...");
  res = f_mkfs(0, 0, 0, Hour,Minute,Second,Month,Day,Year);
  if (res) {
    printf("[FAIL] FORMAT res %d\r\n", res);
    goto main_end_of_test;      
  }
  else printf("[OK]\r\n\n", res);
#endif
 
    /* Get volume information and free clusters of drive 1 */
    fff=&fs;
    res = f_getfree("0:", &fre_clust, &fff);
    if (res) {
        printf("[FAIL] GETFREE res %d\r\n", res);
        goto main_end_of_test;
        }
    /* Get total sectors and free sectors */
    tot_sect = (fff->n_fatent - 2) * fff->csize;
    fre_sect = fre_clust * fff->csize;
    /* Print the free space (assuming 512 bytes/sector) */
    printf("\n\n%lu KiB total drive space.\n%lu KiB available.\n\n", tot_sect / 2, fre_sect / 2);


  Serial.print("Create a file (f_open)...");
  test_file_name[0] = LUN_ID_SD_MMC_0_MEM + '0';
  res = f_open(&file_object, (char const *)test_file_name, FA_CREATE_ALWAYS | FA_WRITE | FA_READ,Hour,Minute,Second,Month,Day,Year);
  if (res != FR_OK) {
    printf("[FAIL] OPEN FILE res %d\r\n", res);
    goto main_end_of_test;
  }  
  Serial.print("[OK]\r\n");

  Serial.print("Write to test file (f_puts)...");
  if (0 == f_puts("WK eMMC\n", &file_object)) {
    f_close(&file_object,Hour,Minute,Second,Month,Day,Year);
    Serial.print("[FAIL] WRITE\r\n");
    goto main_end_of_test;
  }
  for(i = 0; i<9; i++)
  {
    SD_TimeStamp[18] = i + '0';
    f_puts(SD_TimeStamp, &file_object);
    f_puts(SD_Columns, &file_object);
  }
  Serial.print("[OK]\r\n");
    
  Serial.print("Date Time insertion (f_sync)...");
  f_sync(&file_object,Hour,Minute,Second,Month,Day,Year);
  f_close(&file_object,Hour,Minute,Second,Month,Day,Year);
  
  Serial.print("[OK]\r\nOpen again...");
  res = f_open(&file_object, (char const *)test_file_name, FA_READ,Hour,Minute,Second,Month,Day,Year);
  if (res != FR_OK) {
    printf("[FAIL] OPEN2 res %d\r\n", res);
    goto main_end_of_test;
  }  
  Serial.print("[OK]\r\n");

  Serial.print("Read from the test file (f_gets)...");
  if (0 == f_gets(ReadData, sizeof(ReadData), &file_object)) {
    f_close(&file_object,Hour,Minute,Second,Month,Day,Year);
    Serial.print("[FAIL] GETS\r\n");
    goto main_end_of_test;
  }
  Serial.print("[OK]\r\n");
  
  //Serial.print("File data (1st line): "); Serial.println(ReadData);
  Serial.print("Close file ...");
  f_close(&file_object,Hour,Minute,Second,Month,Day,Year);
  Serial.print("[OK]\r\n");

  Serial.print("Scan file ...");
  res=scan_files(filepath);
  if (res != FR_OK) {
    Serial.print("[FAIL]");
  }  
  Serial.print("[OK*]\r\n");
  
  Serial.print("Memory test is successful.\n\r");
  while (1) { }

main_end_of_test:
  f_mount(LUN_ID_SD_MMC_0_MEM,NULL); // UNMOUNT
  Serial.print("Memory test failed.\n\r");
  while (1) { }
}

