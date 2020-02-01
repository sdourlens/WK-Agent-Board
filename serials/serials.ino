// use Serial or SerialUSB to display numbers
// Serial1 and Serial2 are used to send numbers

void setup() {
  Serial.begin(115200);
  Serial.println("Test Serial1 & Serial2");
  Serial1.begin(115200);
  Serial2.begin(115200);
}

char in;
int i=1;

void loop() {

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

