// Read the microcontroller inernal temperature in deg celsius
// Uses ADC15 to read temp value
// Sebastien Dourlens
// WITTY KNOWLEDGE

float readTemperature()
{
  uint32_t ulValue = 0;
  float currTemp = 0.0;
  adc_enable_channel( ADC, adc_channel_num_t(15) );
  adc_start( ADC );
  while ((adc_get_status(ADC) & ADC_ISR_DRDY) != ADC_ISR_DRDY) 	;
  ulValue = adc_get_latest_value(ADC);
  // (ulValue * 3.3 V / 4096 steps resolution / 0.00265 V/�C) - ( 0.8 / 0.00265 V/�C - 27�C)
  // see  Table 46-37 in Atmel Doc 11057B SAM3X/A
  currTemp = (0.304025390625*ulValue)-274,89;
  return currTemp;
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  adc_enable_ts(ADC);
}

void loop() {
  float sensorValue = readTemperature();
  // print out the value you read:
  Serial.println(sensorValue);
  delay(1500);        // delay in between reads for stability
}
