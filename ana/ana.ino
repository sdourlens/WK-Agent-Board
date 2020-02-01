/* WK AGENT
 *  important note: ANA pins work at 12V, not 3.3V
 */

#include "ana.h"
#define TOR_CMD1   37 // output, cmd VN5160S

void setup(){
  Serial.begin(115200);
  pinMode(TOR_CMD1,OUTPUT); // we need 12V output
  digitalWrite(TOR_CMD1,HIGH);
  init_ana();
}

char buf[256];

void loop(){
  Serial.println(read_ana(1));
  delay(100);
}
