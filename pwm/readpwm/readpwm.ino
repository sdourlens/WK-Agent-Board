#define LED1 61

void setup() {
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  Serial.begin(115200);
  analogReadResolution(12);
}

// should be called maximum of times in a second (it takes 100 microseconds)
// T=1 ms <=> F=100kHz
int read_state() {
static int i=0;
static int v[10]={0,0,0,0,0,0,0,0,0,0};
int m=0;
	  v[i]=analogRead(A5);  /* delayMicroseconds(100); */
    i++; if (i==10) i=0;
    for(int j=0;j<10;j++) m=max(v[j],m);
	return m;
}


// display state each period (ms) or more
void display_state(int s,unsigned long period) {

 Serial.print(" State="); Serial.print(s); 
 
  if (s>3900) { // 3400
    Serial.println(" - Connected to station without resistance");return;
  }
  if (s>3200) { Serial.println(" - Connected to station");return; // 3400-3800
  }
  if (s>2300) { Serial.println(" - Disconnected");return; // 2446
  }
  if (s>2000) { Serial.println(" - Cable connected, no station");return; // 2147
  }
 Serial.println(" - Board error, check spi cable");
}

char cpl_display_state() {
char state;   
static char oldstate=3;
static char retour=0;

  // check state
  int s=read_state(); // CPL_READ_STATE
  if (s>4000) state=1; 
  else if (s>3200) state=2; 
  else if (s>2300) state=3; 
  else if (s>2000) state=4;
  else state=5;
  if (state==oldstate) return retour;
  oldstate=state;
 
  // if state changes, display state
  Serial.print("State="); Serial.print(s); 
  switch(state) {
    case 1: Serial.println(" - Connected to station without resistance"); retour=1; return 1;
    case 2: Serial.println(" - Connected to station"); retour=1; return 1;
    case 3: Serial.println(" - Disconnected"); retour=0; return 0;
    case 4: Serial.println(" - Cable connected, no station"); retour=0; return 0;
    default: Serial.println(" - Board error, check spi cable"); retour=0; return 0;    
  }
}

void loop() {
  // int pwm_state=read_state();
  // display_state(pwm_state,1000);
  cpl_display_state();
  // let the time to analogread to get value (100 microseconds)
  delayMicroseconds(100); // maybe adjusted if ucontroller needs a part of these 100 us 
  
}
