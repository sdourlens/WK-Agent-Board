/*
	CPL.H - Driver Qualcomm QCA7000 CPL via SPI (vehicle version!)
	Author: Sebastien Dourlens
	Company: Witty Knowledge
	Version: 1.0
	For: ERCTEEL
	Project: ERCTEEL Multiprotocol ISO 15118 Communication Board 
*/

#ifndef __CPL_H__
#define __CPL_H__

#include <SPI.h>

/* ATMEL SAM3X pins */
#define CPL_FB_PWM  A5  // analogic input - A5 - pin 83
#define CPL_SELECT  48  // output - default 0 - SPI chip select pin - D48 - pin 97
#define CPL_INT     47  // input - D47 - pin 98

#define CPL_SOURCE 105  // 3.3V
#define CPL_GND    106  // GND
#define SPI_MISO    74  // STD input  * SPI Master In Slave Out pin  - D74 - pin 108
#define SPI_MOSI    75  // STD output * SPI Master Out Slave In pin - D75 - pin 109
#define SPI_SCK     76  // STD output * SPI Clock pin - D76 - pin 110

#define QCASPI_CMD_LEN               2
#define QCASPI_HW_PKT_LEN            4 
// 3163 = 0xc5b bytes 
#define QCASPI_HW_BUF_LEN        0xC5B  
#define QCASPI_SLAVE_RESET    (1 << 6)
#define QCASPI_GOOD_SIGNATURE   0xAA55

/* SPI registers */
// Write access internal register
#define SPI_REG_BFR_SIZE          0x4100
#define SPI_REG_SPI_CONFIG_WRITE  0x4400
#define SPI_REG_INTR_CAUSE_WRITE  0x4C00
#define SPI_REG_INTR_ENABLE_WRITE 0x4D00
// Read Access internal registers
#define SPI_REG_WRBUF_SPC_AVA     0xC200
#define SPI_REG_RDBUF_BYTE_AVA    0xC300
#define SPI_REG_SPI_CONFIG_READ   0xC400
#define SPI_REG_INTR_CAUSE_READ   0xCC00
#define SPI_REG_INTR_ENABLE_READ  0xCD00
#define SPI_REG_SIGNATURE         0xDA00
#define SPI_REG_RDBUF_WATERMARK   0xD200
#define SPI_REG_WRBUF_WATERMARK   0xD300
#define SPI_REG_ACTION_CTRL       0xDB00
// External
#define SPI_EXTERNAL_READ         0x8000
#define SPI_EXTERNAL_WRITE        0x0000

/* INTR_CAUSE/ENABLE register definition */
#define SPI_INT_WRBUF_BELOW_WM (1 << 10)
#define SPI_INT_CPU_ON         (1 << 6)
#define SPI_INT_ADDR_ERR       (1 << 3)
#define SPI_INT_WRBUF_ERR      (1 << 2)
#define SPI_INT_RDBUF_ERR      (1 << 1)
#define SPI_INT_PKT_AVLBL      (1 << 0)

/* ACTION_CTRL register definition */
#define SPI_ACTRL_PKT_AVA_SPLIT_MODE (1 << 8)
#define SPI_ACTRL_PKT_AVA_INTR_MODE (1 << 0)

/* RESET, RESTART, CHECK TIME OUT from 10 to 1000 */
#define CPL_RESET_TIMEOUT 100

uint16_t vInterruptCause;
uint8_t CPL_DataReceived=0;

/* 
 * Should be called maximum of times in a second (it takes 100 microseconds)
 * T=1 ms <=> F=100kHz 
 */
int cpl_read_state() {
static int i=0;
static int v[10]={0,0,0,0,0,0,0,0,0,0};
int m=0;
    v[i]=analogRead(A5);  /* delayMicroseconds(100); */
    i++; if (i==10) i=0;
    for(int j=0;j<10;j++) m=max(v[j],m);
  return m;
}

/*
 * display state each period (ms) or more (automatically calls cpl_read_state)
 * DIN 70121 standard
 */
char cpl_display_state() {
// unsigned long newtime=millis();
static char oldstate=0;
static char retour=0;
static char state=5;   // must be char, to check !!!?

  // check state
  int s=cpl_read_state();
  if (s>3840) state=1; // 3600
  if (s>3328) state=2; // 2900
  if (s>2816) state=3; // 2400
  if (s>2300) state=4; // 2100
  if (state==oldstate) return retour;
  oldstate=state;

  // if state changes, display state
  Serial.print("State="); Serial.print(s); 
  if (s>3840) { Serial.println(" - Connected to station without resistance"); retour=1; return 1; } // 3600
  if (s>3100) { Serial.println(" - Connected to station"); retour=1; return 1; } // 2900
  if (s>2800) { Serial.println(" - Disconnected");  retour=0; return 0; } // 2400
  if (s>2300) { Serial.println(" - Cable connected, no station"); retour=0; return 0; } // 2100
  Serial.println(" - Board error, check spi cable"); retour=0; return 0;
}

/*
 * read a register value.
 */
uint16_t cpl_read_register(uint16_t address)
{
  digitalWrite(CPL_SELECT, LOW);
  SPI.transfer(CPL_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
  SPI.transfer(CPL_SELECT,address & 0xFF, SPI_CONTINUE);
  uint8_t msb=SPI.transfer(CPL_SELECT,0xFF, SPI_CONTINUE);
  uint8_t lsb=SPI.transfer(CPL_SELECT,0xFF, SPI_LAST);
  digitalWrite(CPL_SELECT, HIGH);
  uint16_t data=lsb+(msb <<8);
  return data;
}

/*
 * write a register value.
 */
void cpl_write_register(uint16_t address,uint16_t value)
{
  digitalWrite(CPL_SELECT, LOW);
  SPI.transfer(CPL_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
  SPI.transfer(CPL_SELECT,address & 0xFF, SPI_CONTINUE);
  SPI.transfer(CPL_SELECT,(value & 0xFF00) >>8, SPI_CONTINUE); // msb
  SPI.transfer(CPL_SELECT,value & 0xFF, SPI_LAST); // lsb
  digitalWrite(CPL_SELECT, HIGH);   
}

/*
 * Dump the contents of all SPI slave registers
 */
void cpl_dump_regs()
{
/*
  struct reg {
    char *name;
    uint16_t address;
    };

  static struct reg regs[] = {
    { "  WRBUF_SPC_AVA", SPI_REG_WRBUF_SPC_AVA },
    { "  RDBUF_BYTE_AVA", SPI_REG_RDBUF_BYTE_AVA },
    { "  SPI_CONFIG", SPI_REG_SPI_CONFIG_READ },
    { "  INTR_CAUSE", SPI_REG_INTR_CAUSE_READ },
    { "  INTR_ENABLE", SPI_REG_INTR_ENABLE_READ },
    { "  RDBUF_WATERMARK", SPI_REG_RDBUF_WATERMARK },
    { "  WRBUF_WATERMARK", SPI_REG_WRBUF_WATERMARK },
    { "  SIGNATURE", SPI_REG_SIGNATURE },
    { "  ACTION_CTRL", SPI_REG_ACTION_CTRL }
  };
  Serial.println("CPL registers");
  for (int i = 0; i < (sizeof(regs) / sizeof(struct reg)); ++i) {
    uint16_t value = cpl_read_register(regs[i].address);
    Serial.print(String(regs[i].name));
    Serial.print("=");
    Serial.println(value,HEX);
  }
*/

//  if (!cpl_present()) { Serial.println(F("Qualcomm not ready!")); return; }  Serial.print(F("Qualcomm ready ")); 
  uint16_t writebuf,readbuf;
  writebuf=cpl_read_register(SPI_REG_WRBUF_SPC_AVA);
  Serial.print(F("Write buffer available: ")); Serial.print(writebuf,DEC);
  readbuf=cpl_read_register(SPI_REG_RDBUF_BYTE_AVA);
  Serial.print(F(" Byte(s) free - Read buffer:")); Serial.print(readbuf,DEC);
  Serial.println(F(" Byte(s) present"));
}

/*
 * Soft Reset Qualcomm chip
 */
void cpl_reset() {
uint16_t address;     

  address=SPI_REG_SPI_CONFIG_READ;
  digitalWrite(CPL_SELECT, LOW);
  SPI.transfer(CPL_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
  SPI.transfer(CPL_SELECT,address & 0xFF, SPI_CONTINUE);
  uint8_t msb=SPI.transfer(CPL_SELECT,0xFF, SPI_CONTINUE);
  uint8_t lsb=SPI.transfer(CPL_SELECT,0xFF, SPI_LAST);
  digitalWrite(CPL_SELECT, HIGH);
  
  lsb+=QCASPI_SLAVE_RESET;  
  address=SPI_REG_SPI_CONFIG_WRITE;    
  digitalWrite(CPL_SELECT, LOW);
  SPI.transfer(CPL_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
  SPI.transfer(CPL_SELECT,address & 0xFF, SPI_CONTINUE);
  SPI.transfer(CPL_SELECT,msb, SPI_CONTINUE);
  SPI.transfer(CPL_SELECT,lsb, SPI_LAST);
  digitalWrite(CPL_SELECT, HIGH);
}

/*
 * Disables all SPI interrupts and returns the old value of the
 * interrupt enable register.
 */
uint16_t cpl_int_disable() {
  uint16_t old_intr_enable = cpl_read_register( SPI_REG_INTR_ENABLE_READ);
  cpl_write_register(SPI_REG_INTR_ENABLE_WRITE, 0);
  return old_intr_enable;
}

/*
 * Enables only the SPI interrupts passed in the intr argument.
 * All others are disabled.
 * Returns the old value of the interrupt enable register.
 */
uint16_t cpl_int_enable(uint16_t intr) {
  uint16_t old_intr_enable = cpl_read_register( SPI_REG_INTR_ENABLE_READ);
  cpl_write_register(SPI_REG_INTR_ENABLE_WRITE, intr);
  return old_intr_enable;
}

/* 
 * manage CPL interruption over SPI
 */
void cpl_interrupt() {
  uint16_t intr_enable = cpl_int_disable();
  vInterruptCause = cpl_read_register(SPI_REG_INTR_CAUSE_READ);
  if (vInterruptCause!=0) {
    Serial.print("CPL interrupts: "); Serial.print(vInterruptCause,HEX);
  
    if (vInterruptCause & SPI_INT_CPU_ON) {
       Serial.println(" CPL Ready"); // perform a CPL_START
       intr_enable = (SPI_INT_CPU_ON | SPI_INT_PKT_AVLBL | SPI_INT_RDBUF_ERR | SPI_INT_WRBUF_ERR);
      }
  
    if (vInterruptCause & SPI_INT_RDBUF_ERR) {
      Serial.println(" CPL Read Buffer error! Reset."); // MUST RESET CPL
      // cpl_reset();
      }
  
    if (vInterruptCause & SPI_INT_WRBUF_ERR) {
      Serial.println(" CPL Write Buffer error! Reset."); // MUST RESET CPL
      // cpl_reset();
      }
  
    // can only handle other interrupts if sync has occured
    //if (qca->sync == QCASPI_SYNC_READY) { // CPL MUST BE STARTED!!
      if (vInterruptCause & SPI_INT_PKT_AVLBL) { // MUST READ DATA
        Serial.println(" CPL New data recieved!");
        CPL_DataReceived=1;
        } 
    //}
    }
  cpl_write_register(SPI_REG_INTR_CAUSE_WRITE, vInterruptCause);
  cpl_int_enable(intr_enable);
}

/* 
 * set CPL pins 
 */
void cpl_pins(){
  pinMode(CPL_FB_PWM, INPUT);
  pinMode(CPL_INT, INPUT); 
  delay(CPL_RESET_TIMEOUT);
  pinMode(CPL_SELECT, OUTPUT); // not selected: high
  digitalWrite(CPL_SELECT, HIGH);
  analogReadResolution(12); // 12 bits resolution - reading takes 100 microseconds
}

/* 
 * CPL start
 * station:with pwm,vehicle:no pwm
 */
void cpl_start() {
  cpl_pins();
  delay(CPL_RESET_TIMEOUT);
  SPI.begin(CPL_SELECT);      // Init SPI bus
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE3)); // Qualcomm SPI CLK does not exceeed 12 MHz
 // attachInterrupt(digitalPinToInterrupt(CPL_INT), cpl_interrupt, RISING);
  // init interrupt
 // uint16_t intr_enable = (SPI_INT_CPU_ON | SPI_INT_PKT_AVLBL | SPI_INT_RDBUF_ERR | SPI_INT_WRBUF_ERR);
 // cpl_int_enable(intr_enable);
}

/*
 * Only for station, not for vehicle
 */
void cpl_stop() {
}

/*
 * Checks if CPL (Qualcomm chip) is present
 */
char cpl_present() {
uint16_t data;

  data=cpl_read_register(SPI_REG_SIGNATURE);
  return(data==QCASPI_GOOD_SIGNATURE);
}

/* 
 * Check if Qualcomm is present and restart it 
 * higher level than cpl_present() function
 */
void cpl_check() {
int faults=0;

  while(!cpl_present()) { 
      Serial.println(F("Qualcomm not ready!"));  
      delay(CPL_RESET_TIMEOUT);
      faults++; 
      if (faults==10) { Serial.println(F("Reconnect to Qualcomm")); cpl_start(); faults=0; }      
      }
  // Read details
  cpl_dump_regs();
}

/* 
 * Read reception of the packet full at once
 * Get Ethernet Frame
 */
uint16_t cpl_read_ether(uint8_t *buf) 
{
uint16_t readbuf;
uint8_t *ptrbuf=buf;
uint16_t address=SPI_EXTERNAL_READ;

  readbuf=cpl_read_register(SPI_REG_RDBUF_BYTE_AVA);
  if (readbuf==0) return 0;
  cpl_write_register(SPI_REG_BFR_SIZE,readbuf);
  // read bytes
  digitalWrite(CPL_SELECT, LOW);
  SPI.transfer(CPL_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
  SPI.transfer(CPL_SELECT,address & 0xFF, SPI_CONTINUE);
  for(int i=0;i<readbuf-1;i++) { *ptrbuf=SPI.transfer(CPL_SELECT,0, SPI_CONTINUE); ptrbuf++; }
  *ptrbuf=SPI.transfer(CPL_SELECT,0, SPI_LAST);
  digitalWrite(CPL_SELECT, HIGH);
  return readbuf;
}

/* 
 * Send Ethernet Frame (ISO Layer 2 without FCS) to other CPL
 * return the transmitted byte number 
 */
uint16_t cpl_send_ether(uint8_t *buf,uint16_t len) 
{
uint16_t nb;
uint16_t address=SPI_EXTERNAL_WRITE;

	nb = cpl_read_register(SPI_REG_WRBUF_SPC_AVA);
	if (nb >= len + QCASPI_HW_PKT_LEN) {
    cpl_write_register(SPI_REG_BFR_SIZE,len);
   
		// transmit bytes
		digitalWrite(CPL_SELECT, LOW);
		SPI.transfer(CPL_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
		SPI.transfer(CPL_SELECT,address & 0xFF, SPI_CONTINUE);
		SPI.transfer(CPL_SELECT,buf,len);
		digitalWrite(CPL_SELECT, HIGH);
	}
	return nb;
}

#endif

