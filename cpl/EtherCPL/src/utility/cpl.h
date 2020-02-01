/*
 * Copyright (c) 2017 by ERCTEEL. All rights reserved.
 */

#ifndef	CPL_H_INCLUDED
#define	CPL_H_INCLUDED

#include <SPI.h>
#include "qca.h"

#define MAX_SOCK_NUM 4

typedef uint8_t SOCKET;

// 4 next lines used for compatibility with arduino Ethernet Class
#define IDM_OR  0x8000
#define IDM_AR0 0x8001
#define IDM_AR1 0x8002
#define IDM_DR  0x8003

// interruption management
#define IR_CONFLICT     0x80   // 1 if ARP request with same source IP
#define IR_UNREACHABLE  0x40   // 1 if reception of ICMP unreachable during UDP transmission
#define IR_PPPOE        0x20   // not managed
#define IR_SOCKET4      0x08
#define IR_SOCKET3      0x04
#define IR_SOCKET2      0x02
#define IR_SOCKET1      0x01

// mode
#define MR_RST   		0x80
#define MR_PB    		0x10
#define MR_PPPOE 		0x08 // not managed
#define MR_LB    		0x04 // not managed
#define MR_AI    		0x02 // not managed
#define MR_IND   		0x01 // not managed

#define SNMR_CLOSE  	0x00
#define SNMR_TCP     	0x01
#define SNMR_UDP     	0x02
#define SNMR_IPRAW  	0x03
#define SNMR_MACRAW  	0x04
#define SNMR_PPPOE   	0x05
#define SNMR_ND      	0x20
#define SNMR_MULTI    	0x80   /* multicast: any port */


enum SockCMD {
  Sock_OPEN      = 0x01,
  Sock_LISTEN    = 0x02,
  Sock_CONNECT   = 0x04,
  Sock_DISCON    = 0x08,
  Sock_CLOSE     = 0x10,
  Sock_SEND      = 0x20,
  Sock_SEND_MAC  = 0x21,
  Sock_SEND_KEEP = 0x22,
  Sock_RECV      = 0x40
};

class SnIR {
public:
  static const uint8_t SEND_OK = 0x10;
  static const uint8_t TIMEOUT = 0x08;
  static const uint8_t RECV    = 0x04;
  static const uint8_t DISCON  = 0x02;
  static const uint8_t CON     = 0x01;
};

/* socket status */
class SnSR {
public:
  static const uint8_t CLOSED      = 0x00; // close
  static const uint8_t INIT        = 0x13; // open
  static const uint8_t LISTEN      = 0x14; // connection request 
  static const uint8_t SYNSENT     = 0x15; // connection request from us
  static const uint8_t SYNRECV     = 0x16; // connection request from pear
  static const uint8_t ESTABLISHED = 0x17; // send receive
  static const uint8_t FIN_WAIT    = 0x18; // termination
  static const uint8_t CLOSING     = 0x1A; // termination
  static const uint8_t TIME_WAIT   = 0x1B; // termination
  static const uint8_t CLOSE_WAIT  = 0x1C; // termination
  static const uint8_t LAST_ACK    = 0x1D; // termination
  static const uint8_t UDP         = 0x22;
  static const uint8_t IPRAW       = 0x32; // no IP header 
  static const uint8_t MACRAW      = 0x42;
  static const uint8_t PPPOE       = 0x5F;
};

// See RFC791
class IPPROTO {
public:
  static const uint8_t IP   = 0;
  static const uint8_t ICMP = 1;
  static const uint8_t IGMP = 2;
  static const uint8_t GGP  = 3;
  static const uint8_t TCP  = 6;
  static const uint8_t PUP  = 12;
  static const uint8_t UDP  = 17;
  static const uint8_t IDP  = 22;
  static const uint8_t GRE  = 47; // Router Passthrough
  static const uint8_t ND   = 77;
  static const uint8_t L2TP = 115;
  static const uint8_t RAW  = 255;
};

class IPFLAG {
public:
  static const uint8_t R = 0x00;  // not used, reserved
  static const uint8_t M = 0x1;   // may fragments, they follow
  static const uint8_t D = 0x2;   // do not fragment
};


typedef struct {
	uint8_t mode;
	uint8_t gw[4];
	uint8_t mask[4];
	uint8_t mac[6];
	uint8_t ip[4];
	uint8_t inter;
	uint8_t intmask;
	uint16_t timeout;
	uint8_t retry;
	uint8_t rx_size;
	uint8_t tx_size;
	uint8_t auth;
	uint8_t timer;
	uint8_t magic;
	uint8_t noip[4];
	uint16_t noport;
} ty_reg;

typedef struct {
	uint8_t mode;
	uint8_t cmd;
	uint8_t inter;
	uint8_t status;
	uint16_t port; // local
	uint8_t dmac[6]; // destination
	uint8_t dip[4];  // destination
	uint16_t dport;
	uint16_t maxsegsize;
	uint8_t protocol;
	uint8_t ip_tos;
	uint8_t ip_ttl;
	uint16_t tx_freesize;
	uint16_t tx_readptr;
	uint16_t tx_writeptr;
	uint16_t rx_recievedsize;
	uint16_t rx_readptr;
	uint16_t rx_writeptr;
	uint8_t readbuf[QCAMAXSIZE];
	uint8_t writebuf[QCAMAXSIZE];
} ty_sock;

class CPLClass {

public:
	void init();

	/**
	* @brief	This function is being used for copy the data form Receive buffer of the chip to application buffer.
	* 
	* It calculate the actual physical address where one has to read
	* the data from Receive buffer. Here also take care of the condition while it exceed
	* the Rx memory uper-bound of socket.
	*/
	void read_data(SOCKET s, volatile uint16_t src, volatile uint8_t * dst, uint16_t len);

	/**
	* @brief	 This function is being called by send() and sendto() function also. 
	* 
	* This function read the Tx write pointer register and after copy the data in buffer update the Tx write pointer
	* register. User should read upper byte first and lower byte later to get proper value.
	*/

	void send_data_processing(SOCKET s, const uint8_t *data, uint16_t len);
	/**
	* @brief A copy of send_data_processing that uses the provided ptr for the
	*        write offset.  Only needed for the "streaming" UDP API, where
	*        a single UDP packet is built up over a number of calls to
	*        send_data_processing_ptr, because TX_WR doesn't seem to get updated
	*        correctly in those scenarios
	* @param ptr value to use in place of TX_WR.  If 0, then the value is read
	*        in from TX_WR
	* @return New value for ptr, to be used in the next call
	*/
	void send_data_processing_offset(SOCKET s, uint16_t data_offset, const uint8_t *data, uint16_t len);

	/**
	* @brief	This function is being called by recv() also.
	* 
	* This function read the Rx read pointer register
	* and after copy the data from receive buffer update the Rx write pointer register.
	* User should read upper byte first and lower byte later to get proper value.
	*/
	void recv_data_processing(SOCKET s, uint8_t *data, uint16_t len, uint8_t peek = 0);

	void setGatewayIp(uint8_t *_addr);
	void getGatewayIp(uint8_t *_addr);
	void setSubnetMask(uint8_t *_addr);
	void getSubnetMask(uint8_t *_addr);
	void setMACAddress(uint8_t *_addr);
	void getMACAddress(uint8_t *_addr);
	void setIPAddress(uint8_t *_addr);
	void getIPAddress(uint8_t *_addr);
	void setRetransmissionTime(uint16_t _timeout);
	void setRetransmissionCount(uint8_t _retry);

	void execCmdSn(SOCKET s, SockCMD _cmd);  
	uint16_t getTXFreeSize(SOCKET s);
	uint16_t getRXReceivedSize(SOCKET s);

// CONTROL functions - TO DO
void CPLClass::writeSnCR(SOCKET s,uint8_t data) {  PHY_SOCKET[s].cmd=data; }     // execute Command for the given socket
uint8_t CPLClass::readSnCR(SOCKET s) { return  PHY_SOCKET[s].cmd; }     // command is executed for the socket

// READ registry functions
uint8_t CPLClass::readMR(void) { return PHY_REG.mode; }
void CPLClass::readGAR(void *data) { memcpy(data,PHY_REG.gw,4); }
void CPLClass::readSUBR(void *data) { memcpy(data,PHY_REG.mask,4); }
void CPLClass::readSHAR(void *data) { memcpy(data,PHY_REG.mac,6); } 
void CPLClass::readSIPR(void *data) { memcpy(data,PHY_REG.ip,4); } // Source IP address
uint8_t CPLClass::readIR(void) { return PHY_REG.inter; }  // Interrupt
uint8_t CPLClass::readIMR(void) { return PHY_REG.intmask; }  // Interrupt Mask
uint16_t CPLClass::readRTR(void) { return PHY_REG.timeout; }  // Timeout address
uint8_t CPLClass::readRCR(void) { return PHY_REG.retry; } // Retry count
uint8_t CPLClass::readRMSR(void) { return PHY_REG.rx_size; } // Receive memory size
uint8_t CPLClass::readTMSR(void) { return PHY_REG.tx_size; }  // Transmit memory size
uint8_t CPLClass::readPATR(void) { return PHY_REG.auth; }  // Authentication type address in PPPoE mode
uint8_t CPLClass::readPTIMER(void) { return PHY_REG.timer; }  // PPP LCP Request Timer
uint8_t CPLClass::readPMAGIC(void) { return PHY_REG.magic; } // PPP LCP Magic Number
void CPLClass::readUIPR(void *data) { memcpy(data,PHY_REG.noip,4); } // Unreachable IP address in UDP mode
uint16_t CPLClass::readUPORT(void) { return PHY_REG.noport; }   // Unreachable Port address in UDP mode

// WRITE registry functions
void CPLClass::writeMR(uint8_t data) { PHY_REG.mode=data; }
uint16_t CPLClass::writeGAR(uint8_t *data) { memcpy(PHY_REG.gw,data,4); return 4; }
uint16_t CPLClass::writeSUBR(uint8_t *data) { memcpy(PHY_REG.mask,data,4); return 4; }
uint16_t CPLClass::writeSHAR(uint8_t *data) { memcpy(PHY_REG.mac,data,6); return 6; } 
uint16_t CPLClass::writeSIPR(uint8_t *data) { memcpy(PHY_REG.ip,data,4); return 4; } // Source IP address
void CPLClass::writeIR(uint8_t data) { PHY_REG.inter=data; }  // Interrupt
void CPLClass::writeIMR(uint8_t data) { PHY_REG.intmask=data; }  // Interrupt Mask
void CPLClass::writeRTR(uint16_t data) { PHY_REG.timeout=data; }  // Timeout address
void CPLClass::writeRCR(uint8_t data) { PHY_REG.retry=data; } // Retry count
void CPLClass::writeRMSR(uint8_t data) { PHY_REG.rx_size=data; } // Receive memory size
void CPLClass::writeTMSR(uint8_t data) { PHY_REG.tx_size=data; }  // Transmit memory size
void CPLClass::writePATR(uint8_t data) { PHY_REG.auth=data; }  // Authentication type address in PPPoE mode
void CPLClass::writePTIMER(uint8_t data) { PHY_REG.timer=data; }  // PPP LCP Request Timer
void CPLClass::writePMAGIC(uint8_t data) { PHY_REG.magic=data; } // PPP LCP Magic Number
uint16_t CPLClass::writeUIPR(uint8_t *data) { memcpy(PHY_REG.noip,data,4); return 4; } // Unreachable IP address in UDP mode
void CPLClass::writeUPORT(uint16_t data) { PHY_REG.noport=data; }   // Unreachable Port address in UDP mode

// READ socket functions
uint8_t CPLClass::readSnMR(SOCKET s) { return  PHY_SOCKET[s].mode; } // Mode
uint8_t CPLClass::readSnIR(SOCKET s) { return  PHY_SOCKET[s].inter; }    // Interrupt
uint8_t CPLClass::readSnSR(SOCKET s) { return  PHY_SOCKET[s].status; }   // Status
uint8_t CPLClass::readSnPORT(SOCKET s) { return  PHY_SOCKET[s].port; }    // Source Port
void CPLClass::readSnDHAR(SOCKET s, void *data) { memcpy(data,PHY_SOCKET[s].dmac,6); }     // Destination Hardw Addr		
void CPLClass::readSnDIPR(SOCKET s, void *data) { memcpy(data,PHY_SOCKET[s].dip,4); }    // Destination IP Addr
uint8_t CPLClass::readSnDPORT(SOCKET s) { return  PHY_SOCKET[s].dport; }    // Destination Port
uint8_t CPLClass::readSnMSSR(SOCKET s) { return  PHY_SOCKET[s].maxsegsize; }     // Max Segment Size
uint8_t CPLClass::readSnPROTO(SOCKET s) { return  PHY_SOCKET[s].protocol; }     // Protocol in IP RAW Mode
uint8_t CPLClass::readSnTOS(SOCKET s) { return  PHY_SOCKET[s].ip_tos; }     // IP TOS
uint8_t CPLClass::readSnTTL(SOCKET s) { return  PHY_SOCKET[s].ip_ttl; }      // IP TTL

// WRITE socket functions
void CPLClass::writeSnMR(SOCKET s,uint8_t data) {  PHY_SOCKET[s].mode=data; } // Mode
void CPLClass::writeSnIR(SOCKET s,uint8_t data) {   PHY_SOCKET[s].inter=data; }    // Interrupt
void CPLClass::writeSnSR(SOCKET s,uint8_t data) {   PHY_SOCKET[s].status=data; }   // Status
void CPLClass::writeSnPORT(SOCKET s,uint8_t data) {   PHY_SOCKET[s].port=data; }    // Source Port
uint16_t CPLClass::writeSnDHAR(SOCKET s, void *data) { memcpy(PHY_SOCKET[s].dmac,data,6); return 6; } // Destination MAC @	
uint16_t CPLClass::writeSnDIPR(SOCKET s, void *data) { memcpy(PHY_SOCKET[s].dip,data,4); return 4; } // Destination IP Addr
void CPLClass::writeSnDPORT(SOCKET s,uint8_t data) { PHY_SOCKET[s].dport=data; }    // Destination Port
void CPLClass::writeSnMSSR(SOCKET s,uint8_t data) { PHY_SOCKET[s].maxsegsize=data; }     // Max Segment Size
void CPLClass::writeSnPROTO(SOCKET s,uint8_t data) { PHY_SOCKET[s].protocol=data; }     // Protocol in IP RAW Mode
void CPLClass::writeSnTOS(SOCKET s,uint8_t data) { PHY_SOCKET[s].ip_tos=data; }     // IP TOS
void CPLClass::writeSnTTL(SOCKET s,uint8_t data) { PHY_SOCKET[s].ip_ttl=data; }      // IP TTL

// size
int8_t CPLClass::readSnTX_FSR(SOCKET s) {  return PHY_SOCKET[s].tx_freesize; } // TX Free Size
void CPLClass::writeSnTX_FSR(SOCKET s,uint16_t data) { PHY_SOCKET[s].tx_freesize=data; }    // TX size to write
uint8_t CPLClass::readSnRX_RSR(SOCKET s) {  return PHY_SOCKET[s].rx_recievedsize; }  // RX Free Size
void CPLClass::writeSnRX_RSR(SOCKET s,uint16_t data) { PHY_SOCKET[s].rx_recievedsize=data; }    // RX Size to read
int8_t CPLClass::readSnTX_RD(SOCKET s) {  return PHY_SOCKET[s].tx_readptr; } // TX Free Size
void CPLClass::writeSnTX_RD(SOCKET s,uint16_t data) { PHY_SOCKET[s].tx_readptr=data; }    // TX size to write
uint8_t CPLClass::readSnRX_RD(SOCKET s) {  return PHY_SOCKET[s].rx_readptr; }  // RX Free Size
void CPLClass::writeSnRX_RD(SOCKET s,uint16_t data) { PHY_SOCKET[s].rx_readptr=data; }    // RX Size to read
uint8_t CPLClass::readSnTX_WR(SOCKET s) {  return PHY_SOCKET[s].tx_writeptr; } // TX Free Size
void CPLClass::writeSnTX_WR(SOCKET s,uint16_t data) { PHY_SOCKET[s].tx_writeptr=data; }    // TX size to write
uint8_t CPLClass::readSnRX_WR(SOCKET s) {  return PHY_SOCKET[s].rx_writeptr; }  // RX Free Size
void CPLClass::writeSnRX_WR(SOCKET s,uint16_t data) { PHY_SOCKET[s].rx_writeptr=data; }    // RX Size to read

// --------------------------------------------

uint16_t CPLClass::getTXFreeSize(SOCKET s) { return PHY_SOCKET[s].tx_freesize; }
uint16_t CPLClass::getRXReceivedSize(SOCKET s) { return PHY_SOCKET[s].rx_recievedsize; }
void CPLClass::setGatewayIp(uint8_t *_addr) { writeGAR(_addr);  }
void CPLClass::getGatewayIp(uint8_t *_addr){ readGAR(_addr); }
void CPLClass::setSubnetMask(uint8_t *_addr)  { writeSUBR(_addr); }
void CPLClass::getSubnetMask(uint8_t *_addr) { readSUBR(_addr); }
void CPLClass::setMACAddress(uint8_t *_addr)  { writeSHAR(_addr); }
void CPLClass::getMACAddress(uint8_t *_addr) { readSHAR(_addr); }
void CPLClass::setIPAddress(uint8_t *_addr)  { writeSIPR(_addr); }
void CPLClass::getIPAddress(uint8_t *_addr) { readSIPR(_addr); }
void CPLClass::setRetransmissionTime(uint16_t _timeout) { writeRTR(_timeout); }
void CPLClass::setRetransmissionCount(uint8_t _retry) {  writeRCR(_retry); }
	
	
private:
  static const uint8_t  RST = 7; // Reset BIT
  static const int SOCKETS = MAX_SOCK_NUM;
  static const uint16_t SMASK = 0x07FF; // Tx buffer MASK
  static const uint16_t RMASK = 0x07FF; // Rx buffer MASK
  static const uint16_t RSIZE = TX_RX_MAX_BUF_SIZE; // Max Rx buffer size
public:
  static const uint16_t SSIZE = TX_RX_MAX_BUF_SIZE; // Max Tx buffer size
private:
  // registry of our CPL PHY_QUALCOMM Emulator
  ty_reg PHY_REG;
  ty_sock PHY_SOCKET[MAX_SOCK_NUM];
};

extern CPLClass CPL;

#endif
