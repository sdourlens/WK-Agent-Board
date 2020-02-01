/* 
   WK SCREEN Library to work with WKSCREEN Android application
   screen.h
   Sebastien Dourlens
   WITTY KNOWLEDGE
   02/11/2017
   
   Note: 	Each frame message is composed of an ID, an action, and all other bytes for the message to transfer, and the termination/separator '\n'
			2 types of IDs: window (1..254) or I/O data for sensors/actuators transfer (255)
			4 types of message: I/O data specific to a window, config to display windows, action to control window, serial message 
			the message content cannot contain null character '\0'
			
	TO DO: manage ACK from WKScreen
*/

#ifndef __SCREEN__
#define __SCREEN__

// Serial 8 bits, No parity, 1 stop bit
#define SerialScreen SerialUSB
#define SCREENSPEED  115200
#define MAX_WINDOWS  254
#define CONSOLE_ID   1
#define DATA_ID      255
#define QUEUE_SIZE   MAX_WINDOWS+1

// types of message
enum E_WINMESSAGE { WM_MESSAGE=3, WM_DATA=2, WM_ACTION=1, WM_CONFIG=4 }; 

// variables for configuration
#define NBVARS 16
enum E_WINVAR { E_MESSAGE=1, E_NAME, E_POINTER, E_BACKGROUND, E_TITLE, E_CONTENT, E_CSS, E_LEFT,E_TOP, E_HEIGHT,E_WIDTH, E_URL, E_CANMOVE, E_CANCLOSE, E_CANRESIZE, E_STATE }; 
// Android intent e_name = app, intent e_content= extra args 

// building very high level functions
enum E_WINCONTENT { WC_EMPTY=0, WC_BACKGROUND=1, WC_CONSOLE, WC_WEB, WC_VIDEO, WC_GRAPH, WC_COMMAND, WC_INTENT }; 


class ScreenClass
{
  public:
  	char begin(long speed,void (* cb)(void));
    char begin(long speed);	
  	char available();
  	
  	String readline(unsigned char winid);
  
  	void senddata(unsigned char winid, String data);
  	String readdata(unsigned char winid);
  
  	void sendmessage(unsigned char winid, String message);
  	String readmessage(unsigned char winid);
  
  	void addqueue(unsigned char winid, String message);
  	void sendqueue(unsigned char winid);
  	String readqueue(unsigned char winid);
  
  	void sendconfig(unsigned char winid, String config);
  
  	void start();
  	void stop();
  
    // high level function to send to screen
    void show(unsigned char  sid);
    void hide(unsigned char  sid);
  
  	char received=0;
  	
  	void (*scb)(void);
    
  	String inputString="";  // later, will be a json object
  	char scbuse=0;
  	char stopped=1;
    String inputData="";                // later, will be a json object
    String outputData="";             // later, will be a json object
    String inputQueue[MAX_WINDOWS];  // later, will be a json object
    String outputQueue[MAX_WINDOWS]; // later, will be a json object
    	
  private:
  	long speed=115200L;
  	String config[MAX_WINDOWS];         // later, will be a json object
};


char ScreenClass::begin(long speed,void (* cb)(void)) {
	ScreenClass::scb=cb;
	ScreenClass::speed=speed;
	ScreenClass::scbuse=1;
	SerialScreen.begin(speed);
	ScreenClass::stopped=0;
}

char ScreenClass::begin(long speed) { 
	ScreenClass::speed=speed;
	scbuse=0;
	SerialScreen.begin(speed);
	ScreenClass::stopped=0;
}

char ScreenClass::available() {
	for(int i=0;i<MAX_WINDOWS;i++) if (inputQueue[i].length()>0) return 1;
	if (inputData.length()>0) return 2;
	return 0;
}

// a read removes the line / mode serial (with included type)
String ScreenClass::readline(unsigned char winid) {
String s;	
	int cr = ScreenClass::inputQueue[winid].indexOf('\n');
	if (cr==-1) return ""; 
	s=ScreenClass::inputQueue[winid].substring(0,cr-1);
	ScreenClass::inputQueue[winid].remove(0, cr);	
	return s;
}

//  read and remove next line of data
String ScreenClass::readdata(unsigned char winid) {
String s;	
    // must be a loop
	int cr = ScreenClass::inputQueue[winid].indexOf('\n');
	if (cr==-1) return ""; 
	s=ScreenClass::inputQueue[winid].substring(0,cr-1);
	ScreenClass::inputQueue[winid].remove(0, cr);	
	return s;
}

// read and remove next line of message
String ScreenClass::readmessage(unsigned char winid) {
String s;

	int cr = ScreenClass::inputQueue[winid].indexOf('\n');
	if (cr==-1) return ""; 
	s=ScreenClass::inputQueue[winid].substring(1,cr-1); // we remove winid
	ScreenClass::inputQueue[winid].remove(0, cr);
	return s;
}

// read all messages from queue
String ScreenClass::readqueue(unsigned char winid) {
String s=ScreenClass::inputQueue[winid];
	ScreenClass::inputQueue[winid].remove(0);
	return s;
}

void ScreenClass::addqueue(unsigned char winid, String message) {
	ScreenClass::outputQueue[winid]+=message+String("\n");
}

void ScreenClass::sendqueue(unsigned char winid) {
String header=String(winid);

	SerialScreen.print(header+ScreenClass::outputQueue[winid]);
	ScreenClass::outputQueue[winid].remove(0);
}

void ScreenClass::sendconfig(unsigned char winid, String config) {
String header=String("12");
unsigned char action=WM_CONFIG;

	ScreenClass::config[winid]=config;
  header.setCharAt(0,winid); header.setCharAt(1,action);
	SerialScreen.println(header+ScreenClass::config[winid]);
	ScreenClass::outputQueue[winid].remove(0);
}

void ScreenClass::sendmessage(unsigned char winid, String message) {
String header=String("12");
unsigned char action=WM_MESSAGE;

    header.setCharAt(0,winid); header.setCharAt(1,action);
	SerialScreen.print(header+message);
}

void ScreenClass::senddata(unsigned char winid, String data) {
String header=String("12");
unsigned char action=WM_DATA;
	
	header.setCharAt(0,winid); header.setCharAt(1,action);
	SerialScreen.print(header+data);
}

void ScreenClass::start() {
	SerialScreen.begin(ScreenClass::speed);
	ScreenClass::stopped=0;
}

void ScreenClass::stop() {
	ScreenClass::stopped=1;
}

// high level function to send to screen
void ScreenClass::show(unsigned char winid) {
}

void ScreenClass::hide(unsigned char winid) {
}


// instance
ScreenClass Screen;

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX.
  No delay inside loop. Multiple bytes of data may be available.
*/
void serialEvent() {
  if (Screen.stopped) return;
  while (SerialScreen.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    Screen.inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
    // dispatch line 
    unsigned char winid=Screen.inputString.charAt(0);
    if (winid==255) {
      Screen.inputData+=Screen.inputString; 
    }
    else {
      Screen.inputQueue[winid]+=Screen.inputString;
    }
    Screen.inputString.remove(0);
      if (Screen.scbuse) Screen.scb();  
    }
  }
}

#endif
