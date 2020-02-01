// control PWM output as digital 
// to do: add pwm signal management
// see pwm_lib or atmel sample in parent directory
// Agent Board uses PWML4,PWMH4,PWML6 and PWMH6 of the SAM3X, and DAC0, DAC1 too

#define PIN_PWM4  45   // PC18 - pin 100 - channel 6 H
#define PIN_PWM2 108   // PC20 - pin 131 - channel 4 H
#define PIN_PWM1  9    // PC21 - pin 132 - channel 4 L
#define PIN_EN1   8    // PC22 - pin 133
#define PIN_PWM3  7    // PC23 - pin 134 - channel 6 L
#define PIN_EN2   3    // PC28 - pin 139


void init_pwm(char one,char two)
{
  if (one) { // first driver
    pinMode(PIN_EN1,OUTPUT);
    pinMode(PIN_PWM1,OUTPUT);
    pinMode(PIN_PWM2,OUTPUT);    
     
    digitalWrite(PIN_EN1, LOW);
    }
  if (two) { // second driver
    pinMode(PIN_EN2,OUTPUT);
    pinMode(PIN_PWM3,OUTPUT);
    pinMode(PIN_PWM4,OUTPUT);    
    digitalWrite(PIN_EN2, LOW);
    }
}

void run_pwm(char n) {
  if (n==1) digitalWrite(PIN_EN1, HIGH);
  if (n==2) digitalWrite(PIN_EN2, HIGH);  
  if (n==3) { digitalWrite(PIN_EN1, HIGH); digitalWrite(PIN_EN2, HIGH); }
}

// 1=first, 2=second, 3=1 and 
void stop_pwm(char n, int pwm_time) {
  // slow engine first depending on speed/current as linear in time (pwm_time)
  // TO DO
  if (n==1) digitalWrite(PIN_EN1, LOW);
  if (n==2) digitalWrite(PIN_EN2, LOW);  
  if (n==3) { digitalWrite(PIN_EN1, LOW); digitalWrite(PIN_EN2, LOW); }
  delay(pwm_time);
}

// 1=first, 2=second, 3=1 and 2
void emergency_stop_pwm(char n) {
  if (n==1) digitalWrite(PIN_EN1, LOW);
  if (n==2) digitalWrite(PIN_EN2, LOW);  
  if (n==3) { digitalWrite(PIN_EN1, LOW); digitalWrite(PIN_EN2, LOW); }
}

// brake: inverts signal to stop motor (if free wheel)
// TO DO
void brake(char n) {
}

// simple control function
// sens=0:stop, 1:normal,-1:inverse
// speed <=> duty cycle of pwm (0..255)
// time in ms (for us, use _delay_us())
void control(int n, int pwm_sens, int pwm_speed, int pwm_time) {
  int pin1,pin2,pin3,pin4;
  // STOP
  if (pwm_sens==0) { Serial.print("Stopped"); stop_pwm(n,pwm_time); return; }

  // RUN FORWARD or BACKWARD
  run_pwm(n);
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
