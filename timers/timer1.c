#include <arduino.h>


#define TIMER_CHANNEL (0)
void start_timer_TC1 (uint32_t count)
{
  /* Turn on the timer clock in the power management controller which are
     off by default for power saving:
     1) Disable write protection for pmc registers.
     2) Enable the specified peripheral clock. */
  pmc_set_writeprotect (false) ;

  /* Enable peripheral clock (PMC id column in the table above. */
  pmc_enable_periph_clk (ID_TC3) ;

  TC_Configure
  (TC1,                    /* Clock to use. */
   TIMER_CHANNEL,          /* Channel of the clock to use. */
   TC_CMR_WAVE             /* Waveform mode. */
   | TC_CMR_WAVSEL_UP_RC   /* The counter runs up, and resets when it equals
                                RC. */
   | TC_CMR_TCCLKS_TIMER_CLOCK4) ;  /* Run the clock at MCU / 128. */

  TC_SetRC (TC1, TIMER_CHANNEL, count) ;
  TC_Start (TC1, TIMER_CHANNEL) ;    /* Really start the timer. */

  /* Enable timer interrupts:
     1) IER = interrupt enable register.
     2) IDR = interrupt disable register. */
  TC1->TC_CHANNEL[TIMER_CHANNEL].TC_IER = TC_IER_CPCS ;
  TC1->TC_CHANNEL[TIMER_CHANNEL].TC_IDR = ~TC_IER_CPCS ;

  /* Enable the interrupt in the nested vector interrupt controller. */
  NVIC_EnableIRQ (TC3_IRQn) ;
}
