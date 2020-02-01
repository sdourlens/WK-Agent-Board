// logiciel de test pour les fabricants de cartes
// Copyright WITTY KNOWLEDGE 2018

#include "ana.h"
#include "audio.h"
#include "can.h"
#include "pwm.h"
#include "tor.h"
#include "mem.h"
#include "eth.h" 
#include "rfid.h" 

#define NBMENU 12
enum E_menu { M_CPU=1,M_MEM,M_COM,M_CAN,M_ETH,M_AUDIO,M_JTAG,M_PWM,M_SPI,M_TOR,M_PWMDIG,M_ANA };  
const char *S_menu[NBMENU]= { "CPU","MEMORY","SERIAL COM","CAN BUS","ETHERNET","AUDIO", "JTAG","PWM SYNC&COMPLEMENTARY SQUARE 1Hz","SPI","TOR DIGITAL I/O 12V","PWM as 4 DIGITAL OUTPUTS","ANALOGIC INPUTS 12V" };                
int menu=0;
int count_cpu=0;

// -------------------------------------------------------------------------

// manage keys reception
int wait_key() {
int k1=0,k2=0;
  while (Serial.available() == 0);
  k1=Serial.read();
  delay(200);
  if (Serial.available()>0) k2=Serial.read();
  delay(200);
  Serial.flush();
  if (k2>0) k1=(k1-'0')*10+(k2-'0'); else k1-='0';
  // Serial.println(k1,DEC);
  return k1;
}

// display menu
int display_choose_menu()
{
  Serial.flush(); delay(200);
  Serial.println("Send the key:\n 1. CPU\n 2. MEM\n 3. COM\n 4. CAN\n 5. ETH\n 6. AUDIO\n 7. JTAG\n 8. PWM\n 9. SPI\n10. TOR\n11. PWMDIG\n12. ANA");
  return wait_key();
}

// display chosen test
void display_test(int m) {
 int i;
  for (i=0; i<NBMENU; i++) {
    if (i==m-1) {
        Serial.print("Testing "); Serial.print(S_menu[i]); Serial.println("...");
    }
  }
}

// -------------------------------------------------------------------------

void init_cpu(){count_cpu=0; }
// void init_mem(){ } in mem.h
void init_com(){  
  Serial.println("Test Serial1 & Serial2");
  Serial1.begin(115200);
  Serial2.begin(115200);
  }
// void init_can(){ } in can.h
// void init_eth(){ } in eth.h
// void init_audio(){ } in audio.h
void init_jtag(){ }
// void init_pwm(){ } it is in pwm.h
// void init_spi(){ } it is in rfid.h
// void init_tor(){ } it is in tor.h
// void init_pwmdig(){  } it is in pwm.h
// void init_ana(){ } // it is in ana.h

// -------------------------------------------------------------------------

void test_cpu() { 
  Serial.println("Testing CPU. Press any key to stop");
  Serial.flush();
  while(Serial.available()==0) {
    Serial.print(++count_cpu,DEC); Serial.print(" "); delay(1000);
    }
  Serial.flush();
  }
  
// void test_mem() { } in mem.h

void test_com() {
  char in;
  for (i=1;i<30;i++) {
      Serial2.write(i);
      in=0;
      while(Serial1.available()) {
        in=1; Serial.print("Serial1: "); Serial.println(Serial1.read());
        }
      if (in) {  i++; Serial1.write(i); }
      delay(500);
      in=0;
      while(Serial2.available()) {
        in=1; Serial.print("Serial2: "); Serial.println(Serial2.read());
        }    
      if (in) { i++; Serial2.write(i); };
      delay(500);
      }
}

// void test_can() { } in can.h
// void test_eth() { } in test.h
// void test_audio() { } in audio.h
void test_jtag() {
    Serial.println("Testing JTAG (loop). Press any key to stop");
  while(Serial.available()==0) {
    Serial.print(++count_cpu,DEC); Serial.print(" "); delay(1000);
    }
  Serial.flush();
 }
void test_pwm() { 
    run_pwm();
    while(Serial.available()==0); // necessary in test mode !
    Serial.flush();
  }
  
// void test_spi() { } in rfid.h

void test_pwmdig() { 
    run_pwmdig(3); 
    Serial.println("Switching direction 1..."); 
    digitalWrite(PIN_PWM1, HIGH);
    digitalWrite(PIN_PWM2, LOW);
    digitalWrite(PIN_PWM3, HIGH);
    digitalWrite(PIN_PWM4, LOW);
  
    Serial.print("HBridge1: "); 
    for(int i=0;i<10;i++) { // 10 seconds
          for(int j=1;j<=12;j++) { Serial.print(read_ana(j)); Serial.print(" "); delay(1); } 
          Serial.println("");
          delay(1000);
          }        
  
    Serial.println("Switching direction 2..."); // slowly to do not destroy the L6201PS
    digitalWrite(PIN_PWM1, LOW);
    digitalWrite(PIN_PWM2, LOW);
    digitalWrite(PIN_PWM3, LOW);
    digitalWrite(PIN_PWM4, LOW);
    delay(2000);
    digitalWrite(PIN_PWM1, LOW);
    digitalWrite(PIN_PWM2, HIGH);
    digitalWrite(PIN_PWM3, LOW);
    digitalWrite(PIN_PWM4, HIGH);
  
    Serial.print("HBridge1: "); 
    for(int i=0;i<10;i++) { // 10 seconds
          for(int j=1;j<=12;j++) { Serial.print(read_ana(j)); Serial.print(" "); delay(1);} 
          Serial.println("");
          delay(1000);
          }
    }

void test_ana() {
    Serial.print("ANA Ports (value:0..4095): "); 
    for(int i=0;i<10;i++) { // 10 seconds
          for(int j=1;j<=12;j++) { Serial.print(read_ana(j)); Serial.print(" "); delay(1); } 
          Serial.println("");
          delay(1000);
          }          
  }

// -------------------------------------------------------------------------

void reset_cpu(){ } 
// void reset_mem(){ } in mem.h
void reset_com(){ Serial1.end(); Serial2.end(); }
void reset_can(){ }
void reset_eth(){ }
// void reset_audio(){ } in audio.h
void reset_jtag(){ }
//void reset_pwm(){ } in pwm.h
// void reset_spi(){ } in rfid.h
// void reset_tor(){ } in tor.h
void reset_pwmdig(){ stop_pwm(3,1000); } 
void reset_ana(){ } 

// -------------------------------------------------------------------------

// init hardware for test t
int init_hardware(int t) { 
  switch(t) {
    case 1: init_cpu(); break; // ok
    case 2: init_mem(); break; // ok
    case 3: init_com(); break; // ok
    case 4: init_can(); break; // ok CAN & DAC
    case 5: init_eth(); break; // ok
    case 6: init_audio(); break; //ok AUDIO & DAC & AMPLI
    case 7: init_jtag(); break; // not in this prog. 
    case 8: init_ana(); init_pwm(1,1); break; 
    case 9: init_spi(); break; // ok with RFID
    case 10: init_tor(); break; // ok
    case 11: init_ana(); init_pwm(1,1); break; // ok
    case 12: init_ana(); break; // ok
    default: return 0;
  }
  return t;
}

// run test t, display output: FAILURE=x, OK=0
void run_test(int t) {
  switch(t) {
    case 1: test_cpu(); break; // ok
    case 2: test_mem(); break; // ok
    case 3: test_com(); break; // ok
    case 4: test_can(); break; // ok
    case 5: test_eth(); break; // ok
    case 6: test_audio(); break; // ok
    case 7: test_jtag(); break; // not in this program
    case 8: test_pwm(); break; // ok
    case 9: test_spi(); break; // RFID board test ok
    case 10: test_tor(); break; // ok
    case 11: test_pwmdig(); break; // ok
    case 12: test_ana(); break; // same as 10 or 11 (reading values)
  }
}

// init hardware for test t
void reset_hardware(int t) { 
  switch(t) {
    case 1: reset_cpu(); break; 
    case 2: reset_mem(); break;
    case 3: reset_com(); break;
    case 4: reset_can(); break;
    case 5: reset_eth(); break;
    case 6: reset_audio(); break;
    case 7: reset_jtag(); break;
    case 8: reset_pwm(); break;
    case 9: reset_spi(); break;
    case 10: reset_tor(); break;
    case 11: reset_pwmdig(); break;
    case 12: reset_ana(); break;
  }
}

// ----------------------------- MAIN ---------------------------------

void setup() {
  // audio shutdown
  reset_audio();
  // init usb serial communication (cable USB, pas USB PROG)
  Serial.begin(115200);
  // display welcome sentence
  Serial.println("WITTY KNOWLEDGE (c) 2018 - TESTING SOFTWARE v1.0\n");
}

void loop() {
    menu=display_choose_menu(); // waiting keys
    menu=init_hardware(menu);
    run_test(menu);
    reset_hardware(menu);
    if (menu) Serial.println("\nEnd of test. Send any key to access the menu\n");
    else Serial.println("\It is not a valid option!\n");
}
