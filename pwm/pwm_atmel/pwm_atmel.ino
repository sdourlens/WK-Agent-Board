#include "pwm_lib.h"

using namespace arduino_due::pwm_lib;
pwm<pwm_pin::PWML0_PB16> pwm_dac1;

#define PWM_PERIOD_DAC1 100000 //  1kHz 
#define PWM_DUTY_DAC1    50000 //  50%

#define CPL_FB_PWM  A5  // analogic input - atmel pin 83 - arduino D59/A5

uint32_t dutycycle=0;

void setup() {
  Serial.begin(115200);
  pinMode(CPL_FB_PWM,INPUT);
  analogReadResolution(12);
  pwm_dac1.start(PWM_PERIOD_DAC1,dutycycle); 
}

void loop() {
  int a=analogRead(CPL_FB_PWM);
  Serial.println(a);
  
  // change: 0,25%,50%,75%,100%
  dutycycle+=25000;
  if (dutycycle>PWM_PERIOD_DAC1) dutycycle=1;
  pwm_dac1.set_duty(dutycycle);
  delay(1000);
}


