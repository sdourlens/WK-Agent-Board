void disableERASE() {
  // SAM8X3E manual, chapter 23.8.5
  // System I/O Configuration Register bit 12
  //  1: ERASE function selected (Default at reset).
  //  0: PC0 function selected
  REG_CCFG_SYSIO = (REG_CCFG_SYSIO & 0b11111111111111111110111111111111);
  // and now configure PC0 as INPUT
  pmc_enable_periph_clk( ID_PIOC ) ;
  PIO_Configure(PIOC, PIO_INPUT, PIO_PC0, 0) ;
}
