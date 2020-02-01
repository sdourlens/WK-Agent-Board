/*
 * Copyright (c) 2017 by ERCTEEL. All rights reserved.
 * Author: Sébastien Dourlens, WITTY KNOWLEDGE
 */
 
#include <stdio.h>
#include <string.h>

#include "cpl.h"

// CPL controller instance
CPLClass CPL;

#define TX_RX_MAX_BUF_SIZE 2048
#define TX_BUF 0x1100
#define RX_BUF (TX_BUF + TX_RX_MAX_BUF_SIZE)
#define TXBUF_BASE 0x4000
#define RXBUF_BASE 0x6000

// ---------- CPL SPECIFIC -------------
uint8_t *allowed_ip[],wlai=0; // whitelist, all ip allowed if 0
uint8_t *allowed_ip[],blai=0; // blacklist, all ip allowed if 0

// socket managed by QCA
#define QCAMAXSIZE TX_RX_MAX_BUF_SIZE 
uint8_t tempreadbuf[QCAMAXSIZE];
char blocked_ping=0;
char resetting=0;

// ---------------------------

void CPLClass::init(void)
{
	// qca init: reset, ...
	qca_start();

	memset(&PHY_REG,0,sizeof(ty_reg));
	PHY_REG.timeout=200; // milliseconds
	PHY_REG.noip[0]=169; 
	PHY_REG.noip[1]=1; 
	PHY_REG.noip[2]=1; 
	PHY_REG.noip[3]=1; 
	PHY_REG.noport=50000;
	PHY_REG.rx_size=2048;
	PHY_REG.tx_size=2048;
	PHY_REG.retry=3;

	// default mode: CLOSE  = 0x00
	memset(PHY_SOCKET,0,sizeof(ty_sock));
	for(int i=0;i<MAX_SOCK_NUM;i++) {
		PHY_SOCKET[i].maxsegsize=1500; // max segment size
		PHY_SOCKET[i].ip_ttl=255;
		PHY_SOCKET[i].ip_ttl=255;
		PHY_SOCKET[i].ip_ttl=255;
	}
}

// CONTROL functions - TO DO
void CPLClass::writeSnCR(SOCKET s,uint8_t data) {  PHY_SOCKET[s].cmd=data; }     // execute Command for the given socket
uint8_t CPLClass::readSnCR(SOCKET s) { return  PHY_SOCKET[s].cmd; }     // command is executed for the socket

// READ registry functions
uint8_t CPLClass::readMR(void) { return PHY_REG.mode; }   
uint8_t CPLClass::readIR(void) { return PHY_REG.inter; }
void CPLClass::readGAR(void *data) { memcpy(data,PHY_REG.gw,4); }
void CPLClass::readSUBR(void *data) { memcpy(data,PHY_REG.mask,4); }
void CPLClass::readSHAR(void *data) { memcpy(data,PHY_REG.mac,6); } 
void CPLClass::readSIPR(void *data) { memcpy(data,PHY_REG.ip,4); } // Source IP address
uint8_t CPLClass::readIMR(void) { return PHY_REG.intmask; }  // Interrupt Mask
uint16_t CPLClass::readRTR(void) { return PHY_REG.timeout; }  // Timeout - value 1 = 100us, basis 200ms
uint8_t CPLClass::readRCR(void) { return PHY_REG.retry; } // Retry count 
uint8_t CPLClass::readRMSR(void) { return PHY_REG.rx_size; } // Receive memory size 2048
uint8_t CPLClass::readTMSR(void) { return PHY_REG.tx_size; }  // Transmit memory size 2048
uint8_t CPLClass::readPATR(void) { return PHY_REG.auth; // PAP=0xc023,CHAP=0xc223 }  // Authentication type address in PPPoE mode - not used
uint8_t CPLClass::readPTIMER(void) { return PHY_REG.timer; //200x25ms=5s }  // PPPOE LCP Request Timer -  not used
uint8_t CPLClass::readPMAGIC(void) { return PHY_REG.magic; } // PPP LCP Magic Number - not used
void CPLClass::readUIPR(void *data) { memcpy(data,PHY_REG.noip,4); } // Unreachable IP address in UDP mode
uint16_t CPLClass::readUPORT(void) { return PHY_REG.noport; }   // Unreachable Port address in UDP mode

// WRITE registry functions
void CPLClass::writeMR(uint8_t data) { PHY_REG.mode=data; } // Mode
uint16_t CPLClass::writeGAR(uint8_t *data) { memcpy(PHY_REG.gw,data,4); return 4; }
uint16_t CPLClass::writeSUBR(uint8_t *data) { memcpy(PHY_REG.mask,data,4); return 4; }
uint16_t CPLClass::writeSHAR(uint8_t *data) { memcpy(PHY_REG.mac,data,6); return 6; } 
uint16_t CPLClass::writeSIPR(uint8_t *data) { memcpy(PHY_REG.ip,data,4); return 4; } // Source IP address
void CPLClass::writeIR(uint8_t data) { PHY_REG.inter &= ~data; }  // Interrupt (clear bit when 1 on it) OK
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
void CPLClass::writeSnMR(SOCKET s,uint8_t data) {  PHY_SOCKET[s].mode=data; } // Mode multicasting,no delayed ACK,protocol
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

// local R/W buffers management

void CPLClass::send_data_processing(SOCKET s, const uint8_t *data, uint16_t len)
{  // This is same as having no offset in a call to send_data_processing_offset
  send_data_processing_offset(s, 0, data, len);
}

void CPLClass::recv_data_processing(SOCKET s, uint8_t *data, uint16_t len, uint8_t peek)
{
  uint16_t ptr;
  ptr = readSnRX_RD(s);
  read_data(s, ptr, data, len);
  if (!peek)
  {
    ptr += len;
    writeSnRX_RD(s, ptr);
  }
}

void CPLClass::read_data(SOCKET s, volatile uint16_t src, volatile uint8_t *dst, uint16_t len)
{
  uint16_t size;
  uint16_t src_mask;
  uint16_t src_ptr;

  src_mask = src & RMASK;
  src_ptr = src_mask;

  if( (src_mask + len) > RSIZE ) 
  {
    size = RSIZE - src_mask;
    memcpy( (uint8_t *)dst,&readbuf[s][src_ptr], size);
    dst += size;
    memcpy( (uint8_t *) dst,&readbuf[s][src_ptr], len - size);
  } 
  else
    memcpy((uint8_t *) dst,&readbuf[s][src_ptr], len);
}

void CPLClass::send_data_processing_offset(SOCKET s, uint16_t data_offset, const uint8_t *data, uint16_t len)
{
  uint16_t ptr = readSnTX_WR(s);
  ptr += data_offset;
  uint16_t offset = ptr & SMASK;

  if (offset + len > SSIZE) 
  {
    // Wrap around circular buffer
    uint16_t size = SSIZE - offset;
    memcpy(&writebuf[s][offset], data, size);
    memcpy(writebuf[s], data + size, len - size);
  } 
  else {
    memcpy(&writebuf[s][offset], data, len);
  }

  ptr += len;
  writeSnTX_WR(s, ptr);
}

void CPLClass::execCmdSn(SOCKET s, SockCMD _cmd) {
	// Send command to socket
	writeSnCR(s, _cmd);
	// Wait for command to complete
	while (readSnCR(s));
}

// ----------------------------------------------------------------------------------------------------
// IP MANAGEMENT

// parse data, if state ok, find socket number, store infos in structure PHY_SOCKET, return socket number
// indicates reception is complete or more than buffer size in state
// normally if too much data in buffer, sending a retry to emitter 
SOCKET parse_ethernet(uint8_t *buf,uint16_t len) {
	
	return MAX_SOCK_NUM+1;
}

char connect_tcp(int s) { 
	// try to contact destination tcp:port, and wait ACCEPT
	// wait for answer yes/no	
	return 1;
}

char connect_udp(int s) {
	// try to contact destination udp:port, and wait ACCEPT
	// wait for answer yes/no
	return 1;
}

char connect_ipraw(int s) {
	// try to contact destination udp:port, and wait ACCEPT
	// wait for answer yes/no
	return 1;
}

char connect_macraw(int s) {
	// try to contact destination udp:port, and wait ACCEPT
	// wait for answer yes/no
	return 1;
}

void disconnect(int s) {
	// send a disconnection	
}

// check if allowed to connect
char allow_ip(uint8_t *ip) { 
int ok=0;

	// same network to respect IP protocol for TCP/UDP
	for(int i=0;i<4;i++)
		if ((ip[0] & PHY_REG.mask[0])!=(PHY_REG.ip[0] & PHY_REG.mask[0])) return 0;

	// whitelist allowed ip (if nbai=0, all allowed)
	if (wlai)
		for(int i=0;i<wlai;i++)
			if (ip[0] == allowed_ip[i][0] && ip[1] == allowed_ip[i][1] && ip[2] == allowed_ip[i][2] && ip[3] == allowed_ip[i][3])
				return 1;
	
	// blacklist allowed ip (if nbai=0, all allowed)
	if (blai)
		for(int i=0;i<blai;i++)
			if (ip[0] == refused_ip[i][0] && ip[1] == refused_ip[i][1] && ip[2] == refused_ip[i][2] && ip[3] == refused_ip[i][3])
				return 0;
	
	return 1;	
}

// check if port to open is existing in this socket
// if not return MAX_SOCK_NUM+1
SOCKET allow_port(uint16_t connection_port) { 
	for(int i=0;i<MAX_SOCK_NUM;i++) if (PHY_SOCKET[i].status=SnSR::LISTEN && connection_port==PHY_SOCKET[i].port) return i;
	return MAX_SOCK_NUM+1;
}

// we have receive a connection request from destination (ip/port filtering, no mac filtering)
char connected(int s) {	
	if (PHY_SOCKET[s].status=SnSR::SYNRECV) {
		if (allow_ip(PHY_SOCKET[s].dip) && allow_port(dport)!=MAX_SOCK_NUM+1) // allowed
			return 1;
	}
	
	return 0;
}

// we have receive a close or deconnect message from destination so close
char disconnected(s) {
	if (PHY_SOCKET[s].status=SnSR::CLOSED || 
		PHY_SOCKET[s].status=SnSR::FIN_WAIT ||
		PHY_SOCKET[s].status=SnSR::CLOSING ||
		PHY_SOCKET[s].status=SnSR::TIME_WAIT ||
		PHY_SOCKET[s].status=SnSR::CLOSE_WAIT ||
		PHY_SOCKET[s].status=SnSR::LAST_ACK)
		return 1;
	else
		return 0;
}

// -------------------------------------------------------------------------------------------------

// manage mode, status, interrupt
// to call in the main loop
uint8_t CPLClass::cpl_available() {
uint16_t len,len1,len2;
char result;

    // we should add Timeout management !
	// PHY_SOCKET[s].inter = SnIR::TIMEOUT; => SnSR=TIME_WAIT

	 // execute command on CPL
	if (PHY_REG.mode & MR_RST) init(); // RST = reset / restart cpl + reset memory for phy & socket
	if (PHY_REG.mode & MR_PB) blocked_ping=1;  // PB - ping block mode

    // execute command & send data for each opened socket
	for(int s=0;s<SOCKETS;s++) { 
		// check if disconnected by pear
		if (disconnected(s)) { PHY_SOCKET[s].status = SnSR::CLOSED; PHY_SOCKET[s].inter = SnIR::DISCON; }
		if (connected(s)) { PHY_SOCKET[s].status = SnSR::CLOSED; PHY_SOCKET[s].inter = SnIR::DISCON; }
	
	    // command requested
		switch (PHY_SOCKET[s].cmd) {
			case SockCMD.Sock_OPEN:  // always ok if socket free
  			    // To check mode SnMR.CLOSE, SnMR.TCP, SnMR.UDP, SnMR.MULTI	
				if (PHY_SOCKET[s].status!=SnSR::CLOSED) break; // not possible to open, still used
				if (PHY_SOCKET[s].mode & SNMR_MULTI) { /* udp multicast */ }
				if (PHY_SOCKET[s].mode & SNMR_TCP) { /* tcp */ PHY_SOCKET[s].status=SnSR::INIT; break; }
				if (PHY_SOCKET[s].mode & SNMR_UDP) { /* udp */ PHY_SOCKET[s].status=SnSR::INIT; break; }	
				if (PHY_SOCKET[s].mode & SNMR_IPRAW) { /* ipraw */ PHY_SOCKET[s].status=SnSR::INIT; break; }				
				if (PHY_SOCKET[s].mode & SNMR_MACRAW) { /* macraw */ PHY_SOCKET[s].status=SnSR::INIT; break; }				
				break;
			case SockCMD.Sock_LISTEN:  // 1 listen = wait for connection to my IP
				if (PHY_SOCKET[s].mode & SNMR_TCP || PHY_SOCKET[s].mode & SNMR_UDP) { 
					PHY_SOCKET[s].status=SnSR::LISTEN;
					}
				break;
			case SockCMD.Sock_CONNECT: // 1 connect to destination IP, only TCP
				if (PHY_SOCKET[s].mode & SNMR_TCP) result=connect_tcp(s);  /* tcp */ 
				if (PHY_SOCKET[s].mode & SNMR_UDP) result=connect_udp(s); /* udp */ 
				if (PHY_SOCKET[s].mode & SNMR_IPRAW) result=connect_ipraw(s); /* ip raw */ 
				if (PHY_SOCKET[s].mode & SNMR_MACRAW) {  result=connect_macraw(s);  /* macraw */  break; }				
				if (result) {
					PHY_SOCKET[s].inter = SnIR::CON; 
					}
				break;
			case SockCMD.Sock_DISCON: // 6 disconnect, only TCP
				disconnect(s);
				PHY_SOCKET[s].status=SnSR::CLOSED;
				PHY_SOCKET[s].inter = SnIR::DISCON; 
				break;
			case SockCMD.Sock_CLOSE: // 5 close
			    // send clisong, send closewait, send fin_wait
				PHY_SOCKET[s].status=SnSR::CLOSED;
				break;
			case SockCMD.Sock_SEND: // 4 send with ARP process
				PHY_SOCKET[s].inter = SnSR::SYNSENT;
				PHY_SOCKET[s].inter = SnIR::SEND_OK;
				break;
			case SockCMD.Sock_SEND_MAC: // 3 send mac without ARP process
				PHY_SOCKET[s].inter = SnSR::SYNSENT;
				PHY_SOCKET[s].inter = SnIR::SEND_OK;
				break;
			case SockCMD.Sock_SEND_KEEP: // 4 send, TCP only
				PHY_SOCKET[s].inter = SnSR::SYNSENT;
				PHY_SOCKET[s].inter = SnIR::SEND_OK;
				break;
			case SockCMD.Sock_RECV: // 3/4 receive
				PHY_SOCKET[s].inter = SnIR::RECV;
				if (PHY_SOCKET[s].status=SnSR::LISTEN) { // server mode 
					PHY_SOCKET[s].status=SnSR::ETABLISHED; // cannot listen for now, it is busy send/recv
					// server can read data
				}
				else { // client mode
					// nothing to do, buffer is read by application
				}
				break;
		}
		
		// send data	
		if (PHY_SOCKET[s].tx_freesize==SnSR.ESTABLISHED && PHY_SOCKET[s].tx_freesize!=SSIZE) {
			// add header & queue ???
			if (PHY_SOCKET[s].tx_writeptr!=0) { // two memory blocks to send - starting block
				len1=qca_writebuffer(&readbuf[MAX_SOCK_NUM][PHY_SOCKET[s].tx_writeptr],SSIZE-PHY_SOCKET[s].tx_writeptr);
				if (len1) PHY_SOCKET[s].tx_freesize+=PHY_SOCKET[s].tx_writeptr;
				if (PHY_SOCKET[s].tx_freesize!=SSIZE) { // ending block if exists
					len2=qca_writebuffer(readbuf[MAX_SOCK_NUM],SSIZE-PHY_SOCKET[s].tx_freesize);
					if (len2) { PHY_SOCKET[s].tx_freesize=SSIZE; PHY_SOCKET[s].tx_readptr=0; PHY_SOCKET[s].tx_writeptr=0; }
					}
			}
			else { // one memory block to send
				len=qca_writebuffer(readbuf[MAX_SOCK_NUM],SSIZE-PHY_SOCKET[s].tx_freesize);
				if (len) { PHY_SOCKET[s].tx_freesize=SSIZE; PHY_SOCKET[s].tx_readptr=0; PHY_SOCKET[s].tx_writeptr=0; }
			}
		}
		
		// adapt SnSR & SnIR for SYNRECV (outside request),SYNSENT (our request)
		// ESTABLISHED,FIN_WAIT,CLOSING,CLOSE_WAIT,LAST_ACK,UDP,IPRAW,MACRAW	
	}
	
	// read data from CPL and update buffer of sockets (read everything even if socket closed, to free the Qualcomm memory)
	uint16_t len=qca_readbuffer(tempreadbuf);	
	if (!len) { /* pb of reading: counter++ & retry */ }	
	else {
		// parse msg to check dest ip & dest port, if same as socket s, then we retrieve s
		SOCKET s=parse_ethernet(tempreadbuf,len);
		if (PHY_SOCKET[s].status==SnSR.CLOSED && PHY_SOCKET[s].status=SnSR.FIN_WAIT) return 0;
		PHY_SOCKET[s].rx_recievedsize=len;
		PHY_SOCKET[s].rx_readptr=0;
		PHY_SOCKET[s].rx_writeptr=len;
		// store in good socket buffer
		memcpy(readbuf[s],tempreadbuf,len);
	}
		
	return 1;
}

