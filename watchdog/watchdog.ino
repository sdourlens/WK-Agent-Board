/*
objective: ensure restart if program is no more responding (possible deadlock)

activate mcu watchdog (counter in microseconds)
each loop, restart watchdog else watchdog will reset the mcu and restart the program
*/

/* timeout = 1 ms */
#define WATCHDOG_TIMEOUT_MS 1

int i=1;

void setup() {
  // normally useless because by default in variant.cpp it is disabled, could maybe removed from variant.cpp due to 
  // WDT_Disable(WDT);

  Serial.begin(115200);
  Serial.println("Agent test reset");
  /* put your setup code here */

  // start watchdog
  WDT_Enable ( WDT, 0x2000 | WATCHDOG_TIMEOUT_MS | ( WATCHDOG_TIMEOUT_MS << 16 ) );
}

void loop() {
  WDT_Restart(WDT);
  Serial.println(i++);
  delay(10); /* lock of 100 ms : mcu will reset */
}
