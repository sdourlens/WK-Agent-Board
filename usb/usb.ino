// use Serial or SerialUSB to display numbers


void setup() {
  Serial.begin(115200); // Arduino programming port (USB1)
  Serial.println("Test SerialUSB");
  SerialUSB.begin(115200); // Arduino Due Native port (USB2)
}

int i=1;

void loop() {

  SerialUSB.print(i);
  delay(1000);
  i++;
}

