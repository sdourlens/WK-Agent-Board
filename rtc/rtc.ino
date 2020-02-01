#include <rtc.h>

// Select the Slowclock source
//RTC_clock rtc_clock(RC);
RTC_clock rtc_clock(XTAL);

char* daynames[]={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

void setup() {
  Serial.begin(115200);
  rtc_clock.init();
  rtc_clock.set_time(9, 24, 9);
  rtc_clock.set_date(19, 11, 2015);
}

void loop() {
  Serial.print("At the third stroke, it will be ");
  Serial.print(rtc_clock.get_hours());
  Serial.print(":");
  Serial.print(rtc_clock.get_minutes());
  Serial.print(":");
  Serial.println(rtc_clock.get_seconds());
  Serial.print(daynames[rtc_clock.get_day_of_week()-1]);
  Serial.print(": ");
  Serial.print(rtc_clock.get_days());
  Serial.print(".");
  Serial.print(rtc_clock.get_months());
  Serial.print(".");
  Serial.println(rtc_clock.get_years());
}
