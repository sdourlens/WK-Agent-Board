
#ifndef TOR_h
#define TOR_h

#define TOR_PINS   8

#define TOR_E1     22 // input, 1 if current availabe on output PB26 pin1 
#define TOR_CMD1   37 // output, cmd VN5160S PC5 pin63
#define TOR_DIAG1 100 // input, state VN5160S PC10 pin117 

#define TOR_E2     24 // input, 1 if current availabe on output PA15 pin8
#define TOR_CMD2   38 // output, cmd VN5160S PC6 pin64
#define TOR_DIAG2  36 // input, state VN5160S PC4 pin116

#define TOR_E3     31 // input, 1 if current availabe on output PA7 pin26
#define TOR_CMD3   39 // output, cmd VN5160S PC7 pin65
#define TOR_DIAG3  46 // input, state VN5160S PC17 pin99

#define TOR_E4     71 // input, 1 if current availabe on output PA18 pin70
#define TOR_CMD4   40 // output, cmd VN5160S PC8 pin66
#define TOR_DIAG4  51 // input, state VN5160S PC12 pin94 

#define TOR_E5    103 // input, 1 if current availabe on output PC30 pin103
#define TOR_CMD5   41 // output, cmd VN5160S PC9 pin67
#define TOR_DIAG5  50 // input, state VN5160S PC13 pin95

#define TOR_E6    101 // input, 1 if current availabe on output PC27 pin138 
#define TOR_CMD6   13 // output, cmd VN5160S PB27 pin68
#define TOR_DIAG6  49 // input, state VN5160S PC14 pin96

#define TOR_E7    104 // input, 1 if current availabe on output PC11 pin93 
#define TOR_CMD7   44 // output, cmd VN5160S PC19 pin101
#define TOR_DIAG7  48 // input, state VN5160S PC15 pin97

#define TOR_E8     78 // input, 1 if current availabe on output PB23 pin142 
#define TOR_CMD8    6 // output, cmd VN5160S PC24 pin135
#define TOR_DIAG8  47 // input, state VN5160S PC16 pin98

int torEpin[TOR_PINS]={ TOR_E1,TOR_E2,TOR_E3,TOR_E4,TOR_E5,TOR_E6,TOR_E7,TOR_E8 };
int torCMDpin[TOR_PINS]={ TOR_CMD1,TOR_CMD2,TOR_CMD3,TOR_CMD4,TOR_CMD5,TOR_CMD6,TOR_CMD7,TOR_CMD8 };
int torDIAGpin[TOR_PINS]={ TOR_DIAG1,TOR_DIAG2,TOR_DIAG3,TOR_DIAG4,TOR_DIAG5,TOR_DIAG6,TOR_DIAG7,TOR_DIAG8 };

void init_tor()
{
  for(int i=0;i<TOR_PINS;i++) {
      pinMode(torDIAGpin[i],INPUT);
      pinMode(torEpin[i],INPUT);
      pinMode(torCMDpin[i],OUTPUT);
      write_tor(i+1,LOW);
  }
}

void reset_tor()
{
  for(int i=0;i<TOR_PINS;i++) {
      write_tor(i+1,LOW);
  }
}

/* DIAG/STATUS INFOS
 *  1=normal operation, current limitation
 *  1/0 = overtemperature
 *  0 = undervoltage
 */

// read tor value, tor state and write tor value 
#define read_tor(i) digitalRead(torEpin[i-1])
#define read_diag(i) digitalRead(torDIAGpin[i-1])
#define write_tor(i,b) digitalWrite(torCMDpin[i-1],b)

#endif
