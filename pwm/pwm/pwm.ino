// PWM digital test sample - SD

#include "ana.h"
#include "pwm.h"

void setup() {
  init_ana();
  init_pwm(1,1);
  Serial.begin(115200);
  Serial.println("Test PWM");
  delay(200);
  run_pwm(3); // start
}

void loop() {
  /*
  Serial.println("Run...");
  control(3,1,255,10000); */

  Serial.println("Switching dir1..."); 
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

  Serial.println("Switching dir2..."); // slowly to do not destroy the L6201PS
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

