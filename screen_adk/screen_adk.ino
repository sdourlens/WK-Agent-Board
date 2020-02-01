#include <adk.h>

#define RCVSIZE 128

// Accessory descriptor. It's how Arduino identifies itself to Android.
char applicationName[] = "com.wittyknowledge.wkscreen"; // the app on your phone
char accessoryName[] = "WK Screen"; // your Arduino board
char companyName[] = "Witty Knowledge";

// Make up anything you want for these
char versionNumber[] = "1.0";
char serialNumber[] = "1";
char url[] = "http://labs.arduino.cc/uploads/ADK/ArduinoTerminal/ThibaultTerminal_ICS_0001.apk";
boolean isReady=true;
boolean gotReady=false;
boolean numberComplete = false;
byte bytesToSend[]={0,0};

USBHost Usb;
ADK adk(&Usb, companyName, applicationName, accessoryName, versionNumber, url, serialNumber);


void setup()
{
  Serial.begin(115200); // USB1 port
  cpu_irq_enable();
  Serial.println("ADK demo start");
}

void loop(){
  
  delay(10);
  uint8_t buf[RCVSIZE];
  uint32_t nbread = 0;
  char helloworld[] = "WK Agent Board (c) WITTY KNOWLEDGE!\r\n";

  Usb.Task();

  if (adk.isReady()){
    gotReady=true;
    /* Write hello string to ADK */
    adk.write(strlen(helloworld), (uint8_t *)helloworld);
    }

    /* Read data from ADK and print to UART */
    adk.read(&nbread, RCVSIZE, buf);
    if (nbread > 0)
    {
      //adk.write(strlen(helloworld), (uint8_t *)helloworld);
      Serial.print("RCV : ");
      for (uint32_t i = 0; i < nbread; ++i)  Serial.print(buf[i]);
      Serial.println("");
    }
 /* }else{
    gotReady=false;
  } */
  
  if (gotReady!=isReady){
    isReady=gotReady;
    if (isReady)  Serial.println("ADK is ready");
    else  Serial.println("ADK is not ready");
    }

}

