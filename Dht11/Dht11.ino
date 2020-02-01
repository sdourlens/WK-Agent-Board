
// G=gnd, V=vcc 3.3V, S=source to connect to CS SPI (externe)

#include <Dht11.h>

#define PIN_DHT11 77 // CS SPI EXT. if not used as SPI !

dht11 dht;
char v;
int t,h;

void setup() {
  Serial.begin(115200);
  dht.init(PIN_DHT11);
  }
  
void loop() {
  v=dht.read();
  switch(v) {
    case DHTLIB_OK: 
      Serial.print("Humidite: ");
      h=dht.humidity;
      t=dht.temperature;
      Serial.print(h);
      Serial.print("% Temperature: ");
      Serial.print(t);
      Serial.println(" DegC"); 
      break;
    case DHTLIB_ERROR_CHECKSUM : Serial.println("Verification Erreur"); break;
    case DHTLIB_ERROR_TIMEOUT : Serial.println("Delai depasse"); break;
  }    
  delay(1000);
}
