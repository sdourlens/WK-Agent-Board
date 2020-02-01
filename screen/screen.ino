// WK Screen Demo
#include "screen.h"

unsigned char idconsole=1;
int c=0;

// reception handler by interruption
void screencallback(void) {
	  String s=Screen.readmessage(idconsole);
    if (s.length()>0) Serial.println(String("message received:")+s);
    // not in console ;-)
    String d=Screen.readdata(idconsole);
    if (d.length()>0) Serial.println(String("data received:")+d);
}

void setup()
{
  String helloworld = "WK Agent Board (c) WITTY KNOWLEDGE!\r\nWK Screen Test\r\n";
  Serial.begin(115200);
  Serial.print(helloworld); // emission
  
  // init the screen connection with callback
  Screen.begin(SCREENSPEED, screencallback);
  
  // send the console window settings
  //  E_NAME, E_POINTER, E_BACKGROUND, E_TITLE, E_CONTENT, E_CLASS, E_LEFT,E_TOP, E_HEIGHT,E_WIDTH, E_URL, E_CANMOVE, E_CANCLOSE, E_CANRESIZE, E_STATE
  Screen.sendconfig(idconsole,"console\nnull\n\nConsole\nAndroid:>\nconsole\n0\n0\n800\n400\n\n1\n0\n1\nactive");
  
  // display it
  Screen.show(idconsole);
  Screen.sendmessage(idconsole,helloworld);
}

// exchange text
void loop(){
  /*
  if (Screen.available()){ // reception by scrutation
    Serial.print(Screen.read());
    }
   */

  delay(1000);
  c++;
  Screen.sendmessage(idconsole,String(c,DEC));
}
