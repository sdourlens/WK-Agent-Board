/*
  ISO 15118 board communication library
	ERCTEEL
	06/2017
	v1.0
	Sebastien Dourlens
*/

#include "cpl.h"

#define LED1 61
#define SOF_LEN 8
#define EOF_LEN 2

// station MAC@
uint8_t destmac[]="\x00\x01\x87\x09\x27\x56";
char *destname="Station ERCTEEL";
// vehicle MAC@
uint8_t recmac[]="\x00\x01\x87\x09\x27\x6A";
char *recname="Vehicule";

char cable_connected=0;
uint8_t xbuf[1600],rbuf[1600];
uint16_t len_xbuf=0,len_rbuf=0;
uint8_t ETH_SOF[]="\xAA\xAA\xAA\xAA",ETH_EOF[]="\x55\x55",RSVD[]="\x00\x00";
char message[1000],sender[50],ethertype[50];

/* 
 * Create ISO Layer 2 packet to be sent
 * https://en.wikipedia.org/wiki/Ethernet_frame
 */
uint16_t pack_xbuf(char *msg, uint16_t len) {
uint16_t bodylen=len+14;
uint8_t add_zero_bytes=0;  
  if (bodylen+10>1522) return 0; // too long, packet will be refused (1518 with VLAN tag (4 bytes))
  if (bodylen<60) { add_zero_bytes=60-bodylen; bodylen=60; } // to fill body, must at least 60 bytes  
  // insert SOF+BODY LENGTH+RSVD
  xbuf[0]=ETH_SOF[0];
  xbuf[1]=ETH_SOF[1];
  xbuf[2]=ETH_SOF[2];
  xbuf[3]=ETH_SOF[3];
  xbuf[5]=(bodylen & 0xFF00) >>8;
  xbuf[4]=bodylen & 0xFF;
  xbuf[6]=RSVD[0];
  xbuf[7]=RSVD[1];
  // add mac header (without vlan tag, you may add it if you require it)
  for(int i=0;i<6;i++) xbuf[8+i]=destmac[i];
  for(int i=0;i<6;i++) xbuf[8+6+i]=recmac[i];
  // ETHER TYPE: https://en.wikipedia.org/wiki/EtherType
  // 0x0800 - IPv4
  // 0x0806 - ARP
  // 0x8035 - RARP
  // 0x86DD - IPv6
  xbuf[8+12]='\x08';
  xbuf[8+13]='\x00';
  // add data
  for(int i=0;i<len;i++) xbuf[8+14+i]=msg[i];
  // add zeros to get at least 60 bytes in body content 
  if (add_zero_bytes!=0) for(int i=0;i<add_zero_bytes;i++) xbuf[8+14+len+i]=0;
  // no CRC on 4 bytes - no Frame Check Sequence (FCS) => See Qualcomm documentation
  // add EOF
  xbuf[8+bodylen]=ETH_EOF[0];
  xbuf[9+bodylen]=ETH_EOF[1];
  return bodylen+10;
}


// max packet size if 1522 - returns the message & the message's size
uint16_t unpack_rbuf(uint8_t *packet,char *Message,char *Sender,char *Ethertype) {
uint16_t data_size=0; 
uint32_t message_size=(packet[0]<<24) + (packet[1]<<16) + (packet[2]<<8) + packet[3]; 

  //Serial.print("message size:");Serial.println(message_size,DEC);
  if (packet[4]!=ETH_SOF[0] || packet[5]!=ETH_SOF[1] || packet[6]!=ETH_SOF[2] || packet[7]!=ETH_SOF[3]) return 1;
  // size analysis
  data_size=((packet[9]<<8)+packet[8]);
  //Serial.print("data size:");Serial.println(data_size,DEC);
  if (data_size<60 || data_size>1522) return 2;
  data_size=((packet[9]<<8)+packet[8]);
  if (message_size!=data_size+10) return 3; // sizes issue, maybe incomplete message (contains several packets) 
  // name resolution
  if (packet[18]==destmac[0] && packet[19]==destmac[1] && packet[20]==destmac[2] && 
      packet[21]==destmac[3] && packet[22]==destmac[4] && packet[23]==destmac[5]) strcpy(Sender,destname);
  else strcpy(Sender,"unknown");
  // ethernet type
  strcpy(Ethertype,"Unknown Type");
  if (packet[24]==0x88 && packet[25]==0xE1) strcpy(Ethertype,"HomePlug");
  if (packet[24]==0x08 && packet[25]==0x00) strcpy(Ethertype,"IPv4");
  if (packet[24]==0x86 && packet[25]==0xDD) strcpy(Ethertype,"IPv6");
  if (packet[24]==0x08 && packet[25]==0x06) strcpy(Ethertype,"ARP");
  if (packet[24]==0x80 && packet[25]==0x35) strcpy(Ethertype,"RARP");
  // read string - remove chars after zero (just for demo)
  int i;
  for(i=0;i<data_size-14;i++) Message[i]=packet[i+26]; 
  Message[i]=0;
  return 0;
}

void setup() {
  pinMode(LED1, OUTPUT);
	Serial.begin(115200);   // Initialize serial communications with the PC
	cpl_start();
  cpl_check();
  Serial.println(F("Vehicle Board v1.0 (c) ERCTEEL"));
  Serial.println(F("Qualcomm ready... Enter h:hello, p:present, r:reset, s:state, message to send"));
  delay(100);
  while (!cpl_present()); cpl_dump_regs();
}

void loop() {
  // --------- CONNECTION -----------------
  cable_connected=cpl_display_state();
  if (cable_connected>0) { 
    digitalWrite(LED1,HIGH);     
	  // ----------- RECEPTION (BUSY) ----------------
    if (cpl_present()) {
    	  len_rbuf=cpl_read_ether(rbuf);
    	  if (len_rbuf!=0) {
    		   Serial.print("Data read: ");
    		   for(int i=0;i<len_rbuf;i++) { Serial.print(rbuf[i],HEX); Serial.print(" "); } 
    		   Serial.println(""); 
           // Serial.print("Size: "); Serial.print(len_rbuf,DEC);  Serial.println(""); 
       if (!unpack_rbuf(rbuf,message,sender,ethertype)) {
          Serial.print(ethertype); Serial.print(" Packet from ");
          Serial.print(sender); Serial.print(":");
          Serial.println(String(message));
       } 
       else 
          Serial.println("Packet error or incomplete");               		   
          }
    }   
    digitalWrite(LED1,LOW);
    }
  
  // ----------- IHM ----------------
  if (Serial.available()>0) { // COMMAND
       
       String c=Serial.readString();
       
       if (c=="h" || c=="H") {                
                Serial.print(F("Sending Hello: "));
                strcpy(message,"Hello, I am the bus 61, I need to charge now"); 
                len_xbuf=pack_xbuf(message,strlen(message));
                Serial.print(len_xbuf,DEC); Serial.print(" bytes... ");
                if (cpl_present()) {
                  if (cpl_send_ether(xbuf,len_xbuf)==0) 
                    Serial.println("Full buffer, retry later or reset chip");
                  else Serial.println("OK");
                }
       }       
       else if (c=="r" ||c=="R") {
            Serial.println(F("Reset chip"));
            Serial.read();
            cpl_reset(); delay(100); cpl_check();
            Serial.println(F("Communication restarted")); 
       }
      else if (c=="p" ||c=="P") {
            if (cpl_present()) Serial.println(F("CPL present")); else  Serial.println(F("CPL not present")); 
       }
      else if (c=="s" ||c=="S") {
            cpl_dump_regs();
       }
      else {
            Serial.print(F("Sending message: "));
            c.toCharArray(message,c.length()+1);
            len_xbuf=pack_xbuf(message,strlen(message));
            Serial.print(len_xbuf,DEC); Serial.print(" bytes... ");
            if (cpl_present()) {
              if (cpl_send_ether(xbuf,len_xbuf)==0) 
                Serial.println("Full buffer, retry later or reset chip");
              else Serial.println("OK");
              }
      }
       
  }
}

