#define ANA_ERROR -1.0f
#define ANA_PINS 12

#define ANA_OUT1  9  // ANA9  
#define ANA_OUT2 10  // ANA10 
#define ANA_OUT3 11  // ANA11 
#define ANA_OUT4 12  // ANA12 

// ANA12 arduino pins 52 or 126 
// pins arduino(atmel): A0(AD7),A9(AD11),A10(AD12),A11(AD13),A4(AD3),A5(AD2),A7(AD0),A8(AD10),AD8, AD9, A6(AD1),AD14
// pins ATMEL:          PA16,  PB18,     PB19,     PB20,     PA6,    PA4,    PA2,    PB17,    PB12,PB13,PA3,    PB21
int ana2pin[ANA_PINS]={ 54,     63,      64,       65,       58,     59,     61,     62,      20,  21,  60,     52 };

//---------  Init analog resolution and reference -------------
void init_ana()
{
  // analogReference(DEFAULT); //IF NECESSARY
  analogReadResolution(12);
  for(int i=0;i<ANA_PINS;i++) pinMode(ana2pin[i],INPUT);
}

//---------  Read analog and convert in true physical value (See circuit on VANA) -------------
int read_ana(int pin)
{
  if(pin<1 || pin>ANA_PINS)   //bad parameters
  {
    return ANA_ERROR;
  }

  return analogRead(ana2pin[pin-1]);
}
