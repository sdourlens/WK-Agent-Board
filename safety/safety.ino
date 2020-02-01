// ALL OR NOTHING SAFETY
// test with clignotant à LED

/*
TOR7: CMD7=PC19 pin 101 D44
      E7=PC11 pin 93    D104 
      DIAG7=pc15 pin 97 D48
ANA7: PA2 D61      
*/

#define TOR_E    104 // input, 1 if current availabe on output
#define TOR_CMD   44 // output, cmd VN5160S
#define TOR_DIAG  48 // input, state VN5160S
#define ANA       61 // input adc

int vcc=12;
int voltage=0;
volatile int output=LOW; // off=low,on=high
volatile int state=1; // 1=normal
volatile int change=0;
int i;

/* STATUS INFOS
 *  1=normal operation, current limitation
 *  1/0 = overtemperature
 *  x = undervoltage
 */
void diag() {
  state=1;
  if (output==HIGH) state=0;
  change=1;
}

void setup() {
  pinMode(TOR_E,INPUT);
  pinMode(TOR_DIAG,INPUT); // TRIGGER
  pinMode(TOR_CMD,OUTPUT);
  pinMode(ANA,INPUT);
  
  Serial.begin(115200);
  Serial.println("Test TOR: init.");

  // digitalWrite(TOR_CMD,LOW); already 0 in variant.cpp
  analogReadResolution(12); // 12V to 3.3V (12 bits: 0..4096)
  // HIGH to trigger the interrupt whenever the pin is high. 
  // LOW to trigger the interrupt whenever the pin is low
  // CHANGE to trigger the interrupt whenever the pin changes value
  // RISING to trigger when the pin goes from low to high,
  // FALLING for when the pin goes from high to low.
  //attachInterrupt(TOR_DIAG, diag, CHANGE);
}

void loop() {

  /* check on */
  
  Serial.println("\n\rOutput on");
  output=HIGH;
  digitalWrite(TOR_CMD,output);
  
  //Serial.print("DIAG t1: "); Serial.println(digitalRead(TOR_DIAG1));
  //  if (state1) Serial.println("Normal"); else Serial.println("Failure:overtemp?");

  Serial.print("INPUT: "); Serial.println(digitalRead(TOR_E));
  
  voltage=analogRead(ANA)*vcc/4096;
  Serial.print("ANA START: "); Serial.println(voltage);
  Serial.print("DIAG: ");
  for (i=0;i<100;i++) {
     Serial.print(digitalRead(TOR_DIAG));
    delay(10);
    }
  Serial.println(digitalRead(TOR_DIAG));

  delay(1000); 
  
  voltage=analogRead(ANA)*vcc/4096;
  Serial.print("ANA RUN: "); Serial.println(voltage);

  /* check off */
  
  Serial.println("\n\rOutput off");
  output=LOW;
  digitalWrite(TOR_CMD,output);
  
  Serial.print("INPUT:"); Serial.println(digitalRead(TOR_E));
  voltage=analogRead(ANA)*vcc/4096;
  Serial.print("ANA STOP: "); Serial.println(voltage);
  
  Serial.print("DIAG: ");
  for (i=0;i<100;i++) {
     Serial.print(digitalRead(TOR_DIAG));
     delay(10);
     }
  Serial.println(digitalRead(TOR_DIAG));  
  
  delay(1000); 
   /* 
  if (change==0) Serial.println("Normal"); else Serial.println("Failure");
  */
  voltage=analogRead(ANA)*vcc/4096;
  Serial.print("ANA STOPPED: "); Serial.println(voltage);
}

