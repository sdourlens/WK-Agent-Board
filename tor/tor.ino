// test with LED blinker or CC motor
#include "ana.h"
#include "tor.h"

void setup() {
  init_ana();
  init_tor();
  Serial.begin(115200);
  Serial.println("Test TOR: init.");
}

void loop() {
  Serial.println("\n\rCMD1: 12V");  
  write_tor(1,HIGH);
  delay(2000);
  Serial.print("INPUT E1: "); Serial.println(read_tor(1)); // check output 1 or 0
  Serial.print("INPUT E2: "); Serial.println(read_tor(2)); // read input 1 or 0
  Serial.print("ANA1: "); Serial.println(read_ana(1)); // read voltage 0..4096
  Serial.print("DIAG1: "); Serial.println(read_diag(1)); //1=normal,0=failure
  delay(3000);
  
  Serial.println("\n\rCMD1: OV");  
  write_tor(1,LOW);
  delay(2000);
  Serial.print("INPUT E1: "); Serial.println(read_tor(1)); // check output 1 or 0
  Serial.print("INPUT E2: "); Serial.println(read_tor(2)); // read input 1 or 0
  Serial.print("ANA1: "); Serial.println(read_ana(1)); // read voltage
  Serial.print("DIAG1: "); Serial.println(read_diag(1)); //1=normal,0=failure  
  delay(8000);   
}

