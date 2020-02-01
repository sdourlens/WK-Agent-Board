#include <Arduino.h>


#define    NWAVE               80

uint16_t  Sinewave[2][NWAVE] = {
  {
   +4095,   +4093,   +4089,   +4081,   +4070,   +4056,   +4038,   +4018,   +3995,   +3968,   +3939,   +3907,   +3872,   +3834,   +3793,   +3750,
   +3704,   +3656,   +3605,   +3551,   +3495,   +3438,   +3377,   +3315,   +3251,   +3185,   +3118,   +3048,   +2977,   +2905,   +2831,   +2757,
   +2681,   +2604,   +2526,   +2447,   +2368,   +2289,   +2209,   +2128,   +2048,   +1968,   +1887,   +1807,   +1728,   +1649,   +1570,   +1492,
   +1415,   +1339,   +1265,   +1191,   +1119,   +1048,    +978,    +911,    +845,    +781,    +719,    +658,    +601,    +545,    +491,    +440,
    +392,    +346,    +303,    +262,    +224,    +189,    +157,    +128,    +101,     +78,     +58,     +40,     +26,     +15,      +7,      +3,
  },
  {
      +1,      +3,      +7,     +15,     +26,     +40,     +58,     +78,    +101,    +128,    +157,    +189,    +224,    +262,    +303,    +346,
    +392,    +440,    +491,    +545,    +601,    +658,    +719,    +781,    +845,    +911,    +978,   +1048,   +1119,   +1191,   +1265,   +1339,
   +1415,   +1492,   +1570,   +1649,   +1728,   +1807,   +1887,   +1968,   +2048,   +2128,   +2209,   +2289,   +2368,   +2447,   +2526,   +2604,
   +2681,   +2757,   +2831,   +2905,   +2977,   +3048,   +3118,   +3185,   +3251,   +3315,   +3377,   +3438,   +3495,   +3551,   +3605,   +3656,
   +3704,   +3750,   +3793,   +3834,   +3872,   +3907,   +3939,   +3968,   +3995,   +4018,   +4038,   +4056,   +4070,   +4081,   +4089,   +4093
  }
};

uint16_t  SineFast[2][NWAVE] = {
  {
   +4095,   +3939,   +3495,   +2831,   +2048,   +1265,    +601,    +157,      +1,    +157,    +601,   +1265,   +2048,   +2831,   +3495,   +3939,
   +4095,   +3939,   +3495,   +2831,   +2048,   +1265,    +601,    +157,      +1,    +157,    +601,   +1265,   +2048,   +2831,   +3495,   +3939,
   +4095,   +3939,   +3495,   +2831,   +2048,   +1265,    +601,    +157,      +1,    +157,    +601,   +1265,   +2048,   +2831,   +3495,   +3939,
   +4095,   +3939,   +3495,   +2831,   +2048,   +1265,    +601,    +157,      +1,    +157,    +601,   +1265,   +2048,   +2831,   +3495,   +3939,
   +4095,   +3939,   +3495,   +2831,   +2048,   +1265,    +601,    +157,      +1,    +157,    +601,   +1265,   +2048,   +2831,   +3495,   +3939
  },
  {
   +4095,   +3939,   +3495,   +2831,   +2048,   +1265,    +601,    +157,      +1,    +157,    +601,   +1265,   +2048,   +2831,   +3495,   +3939,
   +4095,   +3939,   +3495,   +2831,   +2048,   +1265,    +601,    +157,      +1,    +157,    +601,   +1265,   +2048,   +2831,   +3495,   +3939,
   +4095,   +3939,   +3495,   +2831,   +2048,   +1265,    +601,    +157,      +1,    +157,    +601,   +1265,   +2048,   +2831,   +3495,   +3939,
   +4095,   +3939,   +3495,   +2831,   +2048,   +1265,    +601,    +157,      +1,    +157,    +601,   +1265,   +2048,   +2831,   +3495,   +3939,
   +4095,   +3939,   +3495,   +2831,   +2048,   +1265,    +601,    +157,      +1,    +157,    +601,   +1265,   +2048,   +2831,   +3495,   +3939
  }
};

            int       fast_mode  =    0;
            int       freq_inhz  = 1000; // Default Hz
            int       freq_intc  =    0;
            int       user_intf  =    0;

volatile   uint16_t   sptr       =    0;

//////////////// AMPLI & GAIN MANAGEMENT //////////////////////////

enum GAIN { G0DB=0,G15DB=4,G18DB=5,G12DB=3,G9DB=2,G6DB=1 };

// resolution: 8 to 12 bits
void ana_config(char resolution) {
  analogWriteResolution(resolution);  // set the analog output resolution to 12 bit (4096 levels)
  analogReadResolution(resolution);   // set the analog input resolution to 12 bit
}

// OE pin set on AUDIO Mode (vs CAN1)
void DAC0AudioMode(){
  pinMode(23,OUTPUT); 
  digitalWrite(23,LOW);
}

void Ampli_init(char left,char right) {
  DAC0AudioMode();
  ana_config(12);
  pinMode(29, OUTPUT);               // /SHUTDOWN=PD6 (D29)
  pinMode(11, OUTPUT);               // Ampli. A0=pd7 (D11)
  pinMode(12, OUTPUT);               // ENABLE==pd8 (D12)
  pinMode(30, OUTPUT);               // A1=pd9 (D30)
  if (right) pinMode(66, OUTPUT);    // DAC0/CANRX2= PB15 (D66)
  if (left) pinMode(67, OUTPUT);     // DAC1 PB16 (D67)
}

void Ampli_on(char gain) {
  // SHUTDOWN off
  digitalWrite(29,HIGH);
  // GAIN
  switch(gain) {
    case G0DB:
        digitalWrite(12, LOW); // off
        break;
    case G6DB:
        // A0 and A1 not connected
        digitalWrite(12, HIGH);
        break;              
    case G9DB:
        digitalWrite(11, HIGH);
        digitalWrite(12, HIGH);
        digitalWrite(30, HIGH);
        break;    
    case G12DB:
        digitalWrite(11, LOW);
        digitalWrite(12, HIGH);
        digitalWrite(30, HIGH);
        break;    
    case G15DB:
        digitalWrite(11, LOW);
        digitalWrite(12, HIGH);
        digitalWrite(30, LOW);
        break;    
    case G18DB:
        digitalWrite(11, HIGH);
        digitalWrite(12, HIGH);
        digitalWrite(30, LOW);
        break;  
  }
}

// use after ampli_init only
void Ampli_off() {
  pinMode(29, OUTPUT);               // /SHUTDOWN=PD6 (D29)
  pinMode(12, OUTPUT);               // ENABLE==pd8 (D12)
  digitalWrite(29,LOW); // shutdown on
  digitalWrite(12,LOW);  // 0DB
}

int tcToFreq( int tc_cntr)
{
  int freq_hz;     

  if( tc_cntr == 0 ) return 1000;
  if( fast_mode ) freq_hz = (420000000UL / tc_cntr) / (2 * NWAVE);
  else            freq_hz = ( 42000000UL / tc_cntr) / (2 * NWAVE);
  return freq_hz;   
}

int freqToTc( int freq_hz)
{
  int tc_cntr = 0;

  if( freq_hz == 0 ) return 25;
  if( fast_mode ) tc_cntr = (420000000UL / freq_hz) / (2 * NWAVE);
  else            tc_cntr = ( 42000000UL / freq_hz) / (2 * NWAVE);
  return tc_cntr;
}

void switch_mode( int mode)
{
  if( mode == 0 )
  {
    DACC->DACC_TPR  =  (uint32_t)  Sinewave[0];      // DMA buffer
    DACC->DACC_TCR  =  NWAVE;
    DACC->DACC_TNPR =  (uint32_t)  Sinewave[1];      // next DMA buffer
    DACC->DACC_TNCR =  NWAVE;
  }
  else
  {
    DACC->DACC_TPR  =  (uint32_t)  SineFast[0];      // DMA buffer
    DACC->DACC_TCR  =  NWAVE;
    DACC->DACC_TNPR =  (uint32_t)  SineFast[1];      // next DMA buffer
    DACC->DACC_TNCR =  NWAVE;
  }
}


void DACC_Handler(void)
{
  if((dacc_get_interrupt_status(DACC) & DACC_ISR_ENDTX) == DACC_ISR_ENDTX) {
    ++sptr;
    sptr &=  0x01;
    if(fast_mode == 0)
    {
      DACC->DACC_TNPR =  (uint32_t)  Sinewave[sptr];      // next DMA buffer
      DACC->DACC_TNCR =  NWAVE;
    }
  else
    {
      DACC->DACC_TNPR =  (uint32_t)  SineFast[sptr];      // next DMA buffer
      DACC->DACC_TNCR =  NWAVE;
    }
  }
}

void setup_pio_TIOA0() 
{
  PIOB->PIO_PDR = PIO_PB25B_TIOA0; 
  PIOB->PIO_IDR = PIO_PB25B_TIOA0; 
  PIOB->PIO_ABSR |= PIO_PB25B_TIOA0;
}

void TC_setup ()
{
  pmc_enable_periph_clk(TC_INTERFACE_ID + 0 *3 + 0);

  TcChannel * t = &(TC0->TC_CHANNEL)[0];           
  t->TC_CCR = TC_CCR_CLKDIS;                       
  t->TC_IDR = 0xFFFFFFFF;                           
  t->TC_SR;                                         
  t->TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 |         
              TC_CMR_WAVE |                         
              TC_CMR_WAVSEL_UP_RC |                 
              TC_CMR_EEVT_XC0 |     
              TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_CLEAR |
              TC_CMR_BCPB_CLEAR | TC_CMR_BCPC_CLEAR;
 
  t->TC_RC = freq_intc;
  t->TC_RA = freq_intc /2;
  t->TC_CMR = (t->TC_CMR & 0xFFF0FFFF) | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET;
  t->TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;   
}

void dac_setup ()
{
  pmc_enable_periph_clk (DACC_INTERFACE_ID) ; // start clocking DAC
  dacc_reset(DACC);
  dacc_set_transfer_mode(DACC, 0);
  dacc_set_power_save(DACC, 0, 1);            // sleep = 0, fastwkup = 1
  dacc_set_analog_control(DACC, DACC_ACR_IBCTLCH0(0x02) | DACC_ACR_IBCTLCH1(0x02) | DACC_ACR_IBCTLDACCORE(0x01));
  dacc_set_trigger(DACC, 1);
 
//  dacc_set_channel_selection(DACC, 1);
  //dacc_enable_channel(DACC, 1);
  dacc_set_channel_selection(DACC, 0);
  dacc_enable_channel(DACC, 0);

  NVIC_DisableIRQ(DACC_IRQn);
  NVIC_ClearPendingIRQ(DACC_IRQn);
  NVIC_EnableIRQ(DACC_IRQn);
  dacc_enable_interrupt(DACC, DACC_IER_ENDTX);
  
  DACC->DACC_TPR  =  (uint32_t)  Sinewave[0];      // DMA buffer
  DACC->DACC_TCR  =  NWAVE;
  DACC->DACC_TNPR =  (uint32_t)  Sinewave[1];      // next DMA buffer
  DACC->DACC_TNCR =  NWAVE;
  DACC->DACC_PTCR =  0x00000100;  //TXTEN - 8, RXTEN - 1.
}

void setup()
{
  Serial.begin (115200);
  DAC0AudioMode();
  Ampli_init(1,1);
  Ampli_on(G9DB);
  dac_setup();       
  freq_intc = freqToTc(freq_inhz);
  TC_setup();       
  setup_pio_TIOA0();
}

void loop()
{
  char in_Byte;
  int     temp;
         
  if (Serial.available() > 0) {
    in_Byte = Serial.read();
    // Message Format To Set  Frequency:  1000f + "send".
    if((in_Byte >= '0') && (in_Byte <= '9'))
    {
      user_intf = (user_intf * 10) + (in_Byte - '0');
    }
    else
    {
      if (in_Byte == 'f') // end delimiter
      {
        if ((user_intf > 20) && (user_intf < 100000))
        {
          freq_inhz = user_intf;
            if( freq_inhz > 10000 ) temp = 1;
            else                    temp = 0;
            if (temp != fast_mode)
            {
              fast_mode = temp;
              switch_mode(fast_mode);
            }
         
          freq_intc = freqToTc(freq_inhz);
          TC_setup();       
          Serial.print("Fast Mode = ");
          Serial.println(fast_mode, DEC);     
          Serial.print("freq_inhz = ");
          Serial.println(freq_inhz, DEC);     
          Serial.print("freq_intc = ");
          Serial.println(freq_intc, DEC);     
          Serial.print("approximation = ");
          temp = tcToFreq(freq_intc);
          Serial.println(temp, DEC);     
        }
      user_intf = 0; // reset to 0 ready for the next sequence of digits
      }   
    }
  }
}

