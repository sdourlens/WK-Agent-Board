/* Module bluetooth hc06bt
 * Sébastien Dourlens
 * Witty Knowledge
 * Note: mettre le module 3.3V sur MISO (74 RX) et MOSI (75 TX) du connecteur J5
 * Note2: Serial = USB; Serial1= RS1 et Serial2=RS2 sont en 12V !
 *    Serial 1
 *        pinMode(18, OUTPUT);                                               // Configure Tx as OUTPUT (Transmitter) D18 = pin 4
 *        pinMode(19, INPUT);                                                // Configure Rx as INPUT (Reciever) D19 = pin 3
 *     Serial 2
 *        pinMode(16, OUTPUT);                                             // Configure Tx as OUTPUT (Transmitter) 
 *        pinMode(17, INPUT);                                              // Configure Rx as INPUT (Reciever)
 */

// GESTION DES \n\r à faire !

// #include <SoftwareSerial.h>

SoftwareSerial BT(74, 75);

// define only if pin EN is connected to 5V to setup the BT
#define INITBT

// String command="";

#ifdef INITBT

/*
AT   OK  Does nothing!
AT+VERSION  OKlinvorV1.5  The firmware version
AT+NAMExyz  OKsetname   Sets the module name to "xyz"
AT+PIN1234  OKsetPIN  Sets the module PIN to 1234
AT+BAUD1  OK1200  Sets the baud rate to 1200
AT+BAUD2  OK2400  Sets the baud rate to 2400
AT+BAUD3  OK4800  Sets the baud rate to 4800
AT+BAUD4  OK9600  Sets the baud rate to 9600
AT+BAUD5  OK19200   Sets the baud rate to 19200
AT+BAUD6  OK38400   Sets the baud rate to 38400
AT+BAUD7  OK57600   Sets the baud rate to 57600
AT+BAUD8  OK115200  Sets the baud rate to 115200
AT+BAUD9  OK230400  Sets the baud rate to 230400
AT+BAUDA  OK460800  Sets the baud rate to 460800
AT+BAUDB  OK921600  Sets the baud rate to 921600
AT+BAUDC  OK1382400   Sets the baud rate to 1382400
*/

void init_bt() {
  BT.begin(38400);                                                  // Set Bluetooth baud rate to default baud rate 38400
  delay(3000);
/*  BT.print("\r\n+STWMOD=0\r\n");                                 // Set the Bluetooth to work in slave mode
  BT.print("\r\n+STNA=C-ZEN\r\n");                                 // Set Bluetooth name to Luma Mini
  BT.print("\r\n+STOAUT=1\r\n");                                   // Permit paired device to connect
  BT.print("\r\n+STAUTO=0\r\n");                                   // Auto-connection should be forbidden here
*/  

  BT.print("AT+NAMEC-ZEN 007\r\n");                                // Set Bluetooth name
  BT.flush();
  delay(600);
  BT.print("AT+PIN1234\r\n");                                      // Set pin code
  BT.flush();
  delay(600);
  BT.print("AT+BAUD8\r\n");                                        // Set speed
  BT.flush();
  delay(600);
/*  BT.print("\r\n+INQ=1\r\n");                                    // Make this Bluetooth Slave inquirable
  delay(2000);
*/
}
#endif

void setup(){
#ifdef INITBT
  init_bt();
#endif
  Serial.begin(115200);
  Serial.println("Type AT Commands or messages");
  BT.begin(115200);                                                  // Set Bluetooth baud rate to default baud rate 38400
  BT.print("AT+VERSION\n\r");
  BT.flush();
}

void loop() {
  // Read device output if available.
  BT.listen();
  if (BT.available()) {
    while(BT.available()) { // While there is more to be read, keep reading.
      Serial.print((char)BT.read()); 
    }    
  }
  
  // Read user input if available.
  if (Serial.available()){
    delay(10); // The delay is necessary to get this working!
    BT.write(Serial.read()+"\n\r"); 
  }  
}

