// Control PWM 
// WK Agent Board uses PWML4,PWMH4,PWML6 and PWMH6 of the SAM3X, and DAC0, DAC1 too

#include "pwm_lib.h"

/* PWM generator */
#define PWM_PERIOD     1000 //  100 kHz  
#define PWM_DUTY        500 //  50% of PWM_PERIOD

// variants ok, périphérique B
#define PIN_PWM4  45   // PC18 - pin 100 - channel 6 H
#define PIN_PWM2 108   // PC20 - pin 131 - channel 4 H
#define PIN_PWM1  9    // PC21 - pin 132 - channel 4 L complementary of H
#define PIN_EN1   8    // PC22 - pin 133
#define PIN_PWM3  7    // PC23 - pin 134 - channel 6 L complementary of H
#define PIN_EN2   3    // PC28 - pin 139

using namespace arduino_due::pwm_lib;
pwm<pwm_pin::PWML4_PC21> pwm1;  
pwm<pwm_pin::PWMH4_PC20> pwm2;  
pwm<pwm_pin::PWML6_PC23> pwm3;  
pwm<pwm_pin::PWMH6_PC18> pwm4;  

// dutycycle is like this: 50% = PWM_PERIOD/2
void pwm_change_duty(uint32_t dutycycle) {
  if (dutycycle>PWM_PERIOD) return;
  pwm1.set_duty(dutycycle);
  pwm2.set_duty(dutycycle);
  pwm3.set_duty(dutycycle);
  pwm4.set_duty(dutycycle);
}

void pwm_change_period(uint32_t p) {
  pwm4.stop();
  pwm3.stop();
  pwm2.stop();
  pwm1.stop();
  pwm1.start(p,PWM_DUTY); 
  pwm2.start(p,PWM_DUTY); 
  pwm3.start(p,PWM_DUTY); 
  pwm4.start(p,PWM_DUTY); 
}

void init_pwm(char one,char two)
{
  Serial.println("Test PWM. Press any key to stop");
  if (one) { // first driver
    pinMode(PIN_EN1,OUTPUT);
    digitalWrite(PIN_EN1, 0);
    pinMode(PIN_PWM1,OUTPUT);
    digitalWrite(PIN_PWM1,0);
    pinMode(PIN_PWM2,OUTPUT);    
    digitalWrite(PIN_PWM2,0);     
    }
  if (two) { // second driver
    pinMode(PIN_EN2,OUTPUT);
    digitalWrite(PIN_EN2, 0);
    pinMode(PIN_PWM3,OUTPUT);
    digitalWrite(PIN_PWM3,0);
    pinMode(PIN_PWM4,OUTPUT);
    digitalWrite(PIN_PWM4,0); 
    }
}

// true PWM
void run_pwm() { // synchronized complementary pwm on PWM4 and PWM6
   pwm1.start(PWM_PERIOD,PWM_DUTY); 
   pwm2.start(PWM_PERIOD,PWM_DUTY); 
   pwm3.start(PWM_PERIOD,PWM_DUTY); 
   pwm4.start(PWM_PERIOD,PWM_DUTY); 
   pinMode(PIN_EN1, OUTPUT); digitalWrite(PIN_EN1,1);
   pinMode(PIN_EN2, OUTPUT); digitalWrite(PIN_EN2,1);
}

// true PWM
// 1=first, 2=second, 3=1 and 
void stop_pwm(char n, int pwm_time) {
  // slow engine first depending on speed/current as linear in time (pwm_time)
  if (n==1) digitalWrite(PIN_EN1, LOW);
  if (n==2) digitalWrite(PIN_EN2, LOW);  
  if (n==3) { digitalWrite(PIN_EN1, LOW); digitalWrite(PIN_EN2, LOW); }
  delay(pwm_time);
}

// true PWM
void reset_pwm() {
  pwm1.stop();
  pwm2.stop();
  pwm3.stop();
  pwm4.stop();  
  digitalWrite(PIN_EN1, LOW);
  digitalWrite(PIN_EN2, LOW);
}

// 1=first, 2=second, 3=1 and 2
void emergency_stop_pwm(char n) {
  if (n==1) digitalWrite(PIN_EN1, LOW);
  if (n==2) digitalWrite(PIN_EN2, LOW);  
  if (n==3) { digitalWrite(PIN_EN1, LOW); digitalWrite(PIN_EN2, LOW); }
  reset_pwm(); 
}

// digital loop
void run_pwmdig(char n) {
  if (n==1) digitalWrite(PIN_EN1, HIGH);
  if (n==2) digitalWrite(PIN_EN2, HIGH);  
  if (n==3) { digitalWrite(PIN_EN1, HIGH); digitalWrite(PIN_EN2, HIGH); }
}

// brake: inverts signal to stop motor (if free wheel)
// TO DO
void brake(char n) {
}

// simple control function (for pwm digital only)
// sens=0:stop, 1:normal,-1:inverse
// speed <=> duty cycle of pwm (0..255)
// time in ms (for us, use _delay_us())
void control(int n, int pwm_sens, int pwm_speed, int pwm_time) {
  int pin1,pin2,pin3,pin4;
  // STOP
  if (pwm_sens==0) { Serial.print("Stopped"); stop_pwm(n,pwm_time); return; }

  // RUN FORWARD or BACKWARD
  run_pwmdig(n);
  if (pwm_sens==-1) { pwm_sens=0; pin1=PIN_PWM1; pin2=PIN_PWM2; pin3=PIN_PWM3; pin4=PIN_PWM4; } 
  else { pwm_sens=1; pin1=PIN_PWM2; pin2=PIN_PWM1; pin3=PIN_PWM4; pin4=PIN_PWM3; }
  
  Serial.println("Running");
  if (n==1) { digitalWrite(pin2, LOW); digitalWrite(pin1, HIGH); } //analogWrite(pin1, pwm_speed); }
  if (n==2) { digitalWrite(pin4, LOW); digitalWrite(pin3, HIGH); } //analogWrite(pin3, pwm_speed); }
  if (n==3) { digitalWrite(pin2, LOW); digitalWrite(pin1, HIGH); //analogWrite(pin1, pwm_speed); 
              digitalWrite(pin4, LOW); digitalWrite(pin3, HIGH); } //analogWrite(pin3, pwm_speed); }
  
  // hold the motor at full speed for x ms
  delay(pwm_time);        
  Serial.println("End of control");         
}


