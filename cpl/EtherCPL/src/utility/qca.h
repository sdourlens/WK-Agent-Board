/*
	QCA.H - Driver Qualcomm QCA7000 CPL via SPI 
	Author: Sébastien Dourlens
	Company: Witty Knowledge
	Version: 1.0
	For: ERCTEEL
	Project: ERCTEEL Multiprotocol ISO 15118 Communication Board 
*/

#ifndef __QCA_H__
#define __QCA_H__

#include <SPI.h>

/* ATMEL SAM3X pins */
#define QCA_PWM     67  // output - default 0 - D67 - pin 77
#define QCA_FB_PWM  34  // analogic input - D34 - pin 59
#define QCA_ENABLE  49  // output - default 0 - D49 - pin 96
#define QCA_SELECT  48  // output - default 0 - SPI chip select pin - D48 - pin 97
#define QCA_INT     47  // input - D47 - pin 98

#define QCA_SOURCE 105  // 3.3V
#define QCA_GND    106  // GND
#define SPI_MISO    74  // STD input * SPI Master In Slave Out pin  - D74 - pin 108
#define SPI_MOSI    75  // STD output * SPI Master Out Slave In pin - D75 - pin 109
#define SPI_SCK     76  // STD output * SPI Clock pin - D76 - pin 110

#define QCA7K_SPI_READ     (1 << 15)
#define QCA7K_SPI_WRITE    (0 << 15)
#define QCA7K_SPI_INTERNAL (1 << 14)
#define QCA7K_SPI_EXTERNAL (0 << 14)
#define QCASPI_CMD_LEN     2
#define QCASPI_HW_PKT_LEN  4 
#define QCASPI_HW_BUF_LEN  0xC5B
#define QCASPI_SLAVE_RESET (1 << 6)
// 3163 = 0xc5b bytes to send

/* SPI registers */
#define	SPI_REG_BFR_SIZE_WRITE   0x4100
#define SPI_REG_WRBUF_SPC_AVA    0xC200
#define SPI_REG_RDBUF_BYTE_AVA   0xC300
#define SPI_REG_SPI_CONFIG_READ  0xC400
#define SPI_REG_SPI_CONFIG_WRITE 0x4400
#define SPI_EXTERNAL_READ        0x8000
#define SPI_EXTERNAL_WRITE       0x0000
#define SPI_REG_INTR_CAUSE       0x0C00
#define SPI_REG_INTR_ENABLE      0x0D00
#define SPI_REG_RDBUF_WATERMARK  0x1200
#define SPI_REG_WRBUF_WATERMARK  0x1300
#define SPI_REG_SIGNATURE        0xDA00
#define SPI_REG_ACTION_CTRL      0x1B00

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

#define QCASPI_GOOD_SIGNATURE 0xAA55

/* RESET, RESTART, CHECK TIME OUT from 10 to 1000 */
#define QCA_RESET_TIMEOUT 100

enum QCA_PWM_STATE { EP_DISCONNECTED,EP_NOTREADY,EP_READY,EP_READYVENTIL,EP_STATIONOFF,EP_BOARDERROR };

/* set proc pins */
void qca_pins(){
  pinMode(QCA_PWM, OUTPUT); // this line is not mandatory
  pinMode(QCA_FB_PWM, INPUT);
  pinMode(QCA_INT, INPUT); 
  pinMode(QCA_ENABLE, OUTPUT);  
  digitalWrite(QCA_ENABLE, LOW); // disable & reset: low
  delay(QCA_RESET_TIMEOUT);
  pinMode(QCA_SELECT, OUTPUT); // not selected: high
  digitalWrite(QCA_SELECT, HIGH);
}

/* 12-bits PWM Generator */
void qca_pwm_run() {
uint8_t dutycycle=100; // STATES A..D
	analogWriteResolution(12); 
	analogWrite(QCA_PWM, dutycycle*4096/100); 
}

void qca_pwm_stop() {
uint8_t dutycycle=50; // STATE E - charger not available
	analogWriteResolution(12);	
	analogWrite(QCA_PWM,dutycycle*4096/100); 
}

/* DIN 70121 standard
feedback pwm combo : 0-3.3V => fluctuations 2V à 3.3V
pwm_combo : pwm 0-3.3V
0    -> -12V  => State F : station court circuit ou indisponible (possibilité de carte agent déconnectée - HE10 déconnecté ?)
1.7V -> 0V    => State E : station court circuit ou indisponible (géré par soft)
2.1V -> 3V    => State D : Véhicule prêt pour la charge avec ventilation
2.5  -> 6V    => State C : Véhicule prêt pour la charge sans ventilation
3V   -> 9V    => State B : Véhicule connecté mais pas prêt à charger
3.3V -> +12V  => State A : pas de véhicule connecté

Autre cas : Période T = 1 ms, Fréquence F = 1 kHz
*/
char qca_pwm_feeback() {
int v=0;

  analogReadResolution(12); // 12 bits resolution - reading takes 100 microseconds
  v=analogRead(QCA_FB_PWM); 
  if (v<2100) return EP_BOARDERROR;
  if (v<2606) return EP_STATIONOFF;
  if (v<3103) return EP_READYVENTIL;
  if (v<3724) return EP_READY;
  if (v<4096) return EP_NOTREADY;
  return EP_DISCONNECTED;
}

// check analogic port for state of connexion on PWM line
char qca_check_link() {
char result=0;
	char state=qca_pwm_feeback();
	switch(state) {
			case EP_DISCONNECTED: Serial.println("Disconnected cable"); break;
			case EP_NOTREADY:     Serial.println("Connected, Vehicle not ready"); break;
			case EP_READY:        Serial.println("Connected, Vehicle ready no ventilation"); result=1; break;
			case EP_READYVENTIL:  Serial.println("Connected, Vehicle ready with ventilation"); result=1; break;
			case EP_STATIONOFF:   Serial.println("Station not ready"); break;
			case EP_BOARDERROR:   Serial.println("Board Error or Station unavailable"); break;			
	}
	return result;
}

void qca_start() {
  qca_pins();
  qca_pwm_run();
  digitalWrite(QCA_ENABLE, HIGH); // enabled: high
  delay(QCA_RESET_TIMEOUT); 
  SPI.begin(QCA_SELECT);      // Init SPI bus
  SPI.setBitOrder(QCA_SELECT,MSBFIRST);
  SPI.setDataMode(QCA_SELECT,SPI_MODE3);
}

void qca_stop() {
  qca_pwm_stop();
  digitalWrite(QCA_ENABLE, LOW); // disable & reset: low
}

char qca_present() {
uint16_t address=SPI_REG_SIGNATURE;     
uint16_t data;
  
  digitalWrite(QCA_SELECT, LOW);
  SPI.transfer(QCA_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
  SPI.transfer(QCA_SELECT,address & 0xFF, SPI_CONTINUE);
  uint8_t msb=SPI.transfer(QCA_SELECT,0xFF, SPI_CONTINUE);
  uint8_t lsb=SPI.transfer(QCA_SELECT,0xFF, SPI_LAST);
  digitalWrite(QCA_SELECT, HIGH);  
  data=lsb+(msb <<8);
  return(data==QCASPI_GOOD_SIGNATURE);
}

void qca_reset() {
uint16_t address;     

  address=SPI_REG_SPI_CONFIG_READ;
  digitalWrite(QCA_SELECT, LOW);
  SPI.transfer(QCA_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
  SPI.transfer(QCA_SELECT,address & 0xFF, SPI_CONTINUE);
  uint8_t msb=SPI.transfer(QCA_SELECT,0xFF, SPI_CONTINUE);
  uint8_t lsb=SPI.transfer(QCA_SELECT,0xFF, SPI_LAST);
  digitalWrite(QCA_SELECT, HIGH);
  
  lsb+=QCASPI_SLAVE_RESET;  
  address=SPI_REG_SPI_CONFIG_WRITE;    
  digitalWrite(QCA_SELECT, LOW);
  SPI.transfer(QCA_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
  SPI.transfer(QCA_SELECT,address & 0xFF, SPI_CONTINUE);
  SPI.transfer(QCA_SELECT,msb, SPI_CONTINUE);
  SPI.transfer(QCA_SELECT,lsb, SPI_LAST);
  digitalWrite(QCA_SELECT, HIGH);
}

uint16_t qca_writebytes() {
uint16_t address=SPI_REG_WRBUF_SPC_AVA;     

  digitalWrite(QCA_SELECT, LOW);
  SPI.transfer(QCA_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
  SPI.transfer(QCA_SELECT,address & 0xFF, SPI_CONTINUE);
  uint8_t msb=SPI.transfer(QCA_SELECT,0xFF, SPI_CONTINUE);
  uint8_t lsb=SPI.transfer(QCA_SELECT,0xFF, SPI_LAST);
  digitalWrite(QCA_SELECT, HIGH);
  return lsb+(msb <<8);  
}

uint16_t qca_readbytes() {   
uint16_t address=SPI_REG_RDBUF_BYTE_AVA;     

  digitalWrite(QCA_SELECT, LOW);
  SPI.transfer(QCA_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
  SPI.transfer(QCA_SELECT,address & 0xFF, SPI_CONTINUE);
  uint8_t msb=SPI.transfer(QCA_SELECT,0xFF, SPI_CONTINUE);
  uint8_t lsb=SPI.transfer(QCA_SELECT,0xFF, SPI_LAST);
  digitalWrite(QCA_SELECT, HIGH);  
  return lsb+(msb <<8);
}

void qca_check() {
int faults=0;
uint16_t writebuf,readbuf;

  while(!qca_present()) { 
      Serial.println(F("Qualcomm not ready!"));  
      delay(QCA_RESET_TIMEOUT);
      faults++; 
      if (faults==10) { Serial.println(F("Reconnect to Qualcomm")); qca_start(); faults=0; }      
      }
  // Read details
  Serial.print(F("Qualcomm ready ")); 
  writebuf=qca_writebytes();
  Serial.print(F("- Write buffer:")); Serial.print(writebuf,DEC);
  readbuf=qca_readbytes();
  Serial.print(F(" byte(s) free - Read buffer:")); Serial.print(readbuf,DEC);
  Serial.println(F(" byte(s) present"));
  Serial.println("");
}

void qca_writesize(uint16_t buffersize){
uint16_t address=SPI_REG_BFR_SIZE_WRITE;    

  digitalWrite(QCA_SELECT, LOW);
  SPI.transfer(QCA_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
  SPI.transfer(QCA_SELECT,address & 0xFF, SPI_CONTINUE);
  SPI.transfer(QCA_SELECT,(buffersize & 0xFF00) >>8, SPI_CONTINUE);
  SPI.transfer(QCA_SELECT,buffersize & 0xFF, SPI_LAST);
  digitalWrite(QCA_SELECT, HIGH);
}

/* read full buffer 64kB max! */
uint16_t qca_readbuffer(uint8_t *buf)
{
uint16_t readbuf;
uint8_t *ptrbuf=buf;
uint16_t address=SPI_EXTERNAL_READ;

  readbuf=qca_readbytes();
  if (readbuf==0) return 0;
  qca_writesize(readbuf);
  // read bytes
  digitalWrite(QCA_SELECT, LOW);
  SPI.transfer(QCA_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
  SPI.transfer(QCA_SELECT,address & 0xFF, SPI_CONTINUE);
  for(int i=0;i<readbuf;i++) *(ptrbuf++)=SPI.transfer(QCA_SELECT,0, SPI_CONTINUE);
  *ptrbuf=SPI.transfer(QCA_SELECT,0xFF, SPI_LAST);
  digitalWrite(QCA_SELECT, HIGH);
   
  return readbuf;
}

/* returns the transmitted char number */
uint16_t qca_writebuffer(uint8_t *buf,uint16_t len) 
{
uint16_t available;
uint8_t *ptrbuf=buf;
uint16_t address=SPI_EXTERNAL_WRITE;

	available = qca_writebytes();	
	if (available >= len + QCASPI_HW_PKT_LEN) {
		qca_writesize(len);
		// transmit bytes
		digitalWrite(QCA_SELECT, LOW);
		SPI.transfer(QCA_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
		SPI.transfer(QCA_SELECT,address & 0xFF, SPI_CONTINUE);
		for(int i=0;i<available-1;i++) SPI.transfer(QCA_SELECT,*(ptrbuf++), SPI_CONTINUE);
		SPI.transfer(QCA_SELECT,*ptrbuf, SPI_LAST);
		digitalWrite(QCA_SELECT, HIGH);
		*buf=0;
		return len;
	}
	else return 0;
}

/* read full buffer 64kB max! */
uint8_t qca_readchar( uint8_t *ptrbuf)
{
uint16_t readbuf;
uint16_t address=SPI_EXTERNAL_READ;

  readbuf=qca_readbytes();
  if (readbuf==0) return 0;
  qca_writesize(readbuf);
  // read byte
  digitalWrite(QCA_SELECT, LOW);
  SPI.transfer(QCA_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
  SPI.transfer(QCA_SELECT,address & 0xFF, SPI_CONTINUE);
  *ptrbuf=SPI.transfer(QCA_SELECT,0xFF, SPI_LAST);
  digitalWrite(QCA_SELECT, HIGH);
   
  return 1;
}

/* returns the transmitted char number */
uint8_t qca_writechar(uint8_t *c) 
{
uint16_t available;
uint16_t address=SPI_EXTERNAL_WRITE;

	available = qca_writebytes();	
	if (available >= 1 + QCASPI_HW_PKT_LEN) {
		qca_writesize(1);
		// transmit bytes
		digitalWrite(QCA_SELECT, LOW);
		SPI.transfer(QCA_SELECT,(address & 0xFF00) >>8, SPI_CONTINUE);
		SPI.transfer(QCA_SELECT,address & 0xFF, SPI_CONTINUE);
		SPI.transfer(QCA_SELECT,c, SPI_LAST);
		digitalWrite(QCA_SELECT, HIGH);
		return 1;
	}
	else return 0;
}

#endif
