// mem.h
#include <SdFatHsmci.h>

// works with all wk agent boards
#define __WKAGENT1_3__ 

#define FORMATFIRST 1

char test_file_name[] = "0://wkagent.txt";
Ctrl_status status;
FRESULT res;
FATFS fs;
FIL file_object;
TCHAR ReadData[10];
TCHAR SD_TimeStamp[12] = "Test Time\r\n";
TCHAR SD_Columns[15] = "Test Columns\r\n";
char filepath[300]="0:/"; // for scan files (root directory)
int i = 0;

void init_mem() {
  // Initialize HSMCI pin, SD_MMC stack
  HSMCI_init();
  Serial.println("\n-- eMMC HSMCI with FatFs TEST --");
}

int test_mem() {
  unsigned char Hour=18,Minute=39,Second=10,Month=7,Day=6;
   int Year=2018;
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
  else Serial.print("[OK]\r\n\n");
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
    printf("\n\n%lu kB total drive space.\n%lu kB available.\n\n", tot_sect / 2, fre_sect / 2);


  Serial.print("Create a file (f_open)...");
  test_file_name[0] = LUN_ID_SD_MMC_0_MEM + '0';
  res = f_open(&file_object,(const char *)test_file_name, FA_CREATE_ALWAYS | FA_WRITE | FA_READ,Hour,Minute,Second,Month,Day,Year);
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
    
  Serial.print("Date Time insertion (f_sync & f_close)...");
  f_sync(&file_object,Hour,Minute,Second,Month,Day,Year);
  f_close(&file_object,Hour,Minute,Second,Month,Day,Year);

  Serial.println("[OK]\r\nFile exists? ");
  res=scan_files(filepath);
  if (res != FR_OK) {
    Serial.print("[NO]");
  }
  
  Serial.print("[YES]\r\n");

  Serial.print("Open again...");
  res = f_open(&file_object, "wkagent.txt", FA_READ,Hour,Minute,Second,Month,Day,Year);
  if (res != FR_OK) {
    printf("[FAIL] OPEN2 res %d\r\n", res);
    goto main_end_of_test;
  }  
  Serial.print("[OK]\r\n");

  Serial.print("Read content from the test file (f_gets)...");
  if (0 == f_gets(ReadData, sizeof(ReadData), &file_object)) {
    f_close(&file_object,Hour,Minute,Second,Month,Day,Year);
    Serial.print("[FAIL] GETS\r\n");
    goto main_end_of_test;
  }
  Serial.println("File Content: ");
  Serial.println(ReadData);
  Serial.print("[OK]\r\n");
  
  //Serial.print("File data (1st line): "); Serial.println(ReadData);
  f_close(&file_object,Hour,Minute,Second,Month,Day,Year);
  
  Serial.print("Memory test is successful.\n\r");
  return 1;

main_end_of_test:
  Serial.print("Memory test failed.\n\r");
  return 0;
}

void reset_mem(){
	f_mount(LUN_ID_SD_MMC_0_MEM,NULL); // UNMOUNT
}


/* OUTPUT TERMINAL:

-- eMMC HSMCI Card Example with FatFs --

version:4
speed:50 MHz
8-bit bus width enabled.

Enable High Speed

SD/MMC card ready
15388672 KBytes
Write protected: NO
Mount disk (f_mount)...[OK]
Format disk (f_mkfs)...
[OK]

chk_mounted: no filesystem
chk_mounted: disk ready
chk_mounted: disk not write protected
chk_mounted: disk sector size=512 [OK]
chk_mounted: disk ok, checking fs...

check_fs disk read bootsector ok
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 01 00 0C FE FF 7B 3F 00 00 00 C1 9F D5 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 55 AA 
check_fs:valid boot record,fat not found

check_fs disk read bootsector ok
EB FE 90 4D 53 44 4F 53 35 2E 30 00 02 40 20 00 01 00 00 00 00 F8 00 00 3F 00 FF 00 3F 00 00 00 C1 9F D5 01 AE 0E 00 00 00 00 00 00 02 00 00 00 01 00 06 00 00 00 00 00 00 00 00 00 00 00 00 00 80 00 29 EA 94 66 4B 57 4B 20 41 47 45 4E 54 20 20 20 46 41 54 33 32 20 20 20 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 55 AA 
check_fs:found fat32

15386720 KiB total drive space.
15386688 KiB available.

Create a file (f_open)...[OK]
Write to test file (f_puts)...[OK]
Date Time insertion (f_sync)...[OK]
Open again...[OK]
Read from the test file (f_gets)...[OK]
Close file ...[OK]
Scan files:
0:/wkagent.txt
[OK]
Memory test is successful.

*/
