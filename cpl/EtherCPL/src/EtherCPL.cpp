#include "utility/cpl.h"
#include "EtherCPL.h"
#include "Dhcp.h"

// don't make assumptions about the value of MAX_SOCK_NUM.
uint8_t EtherCPLClass::_state[MAX_SOCK_NUM] = { 
  0, 0, 0, 0 };
uint16_t EtherCPLClass::_server_port[MAX_SOCK_NUM] = { 
  0, 0, 0, 0 };

int EtherCPLClass::begin(uint8_t *mac_address, unsigned long timeout, unsigned long responseTimeout)
{
  static DhcpClass s_dhcp;
  _dhcp = &s_dhcp;

  // Initialise the basic info
  CPL.init();
  CPL.setMACAddress(mac_address);
  CPL.setIPAddress(IPAddress(0,0,0,0).raw_address());

  // Now try to get our config info from a DHCP server
  int ret = _dhcp->beginWithDHCP(mac_address, timeout, responseTimeout);
  if(ret == 1)
  {
    // We've successfully found a DHCP server and got our configuration info, so set things
    // accordingly
    CPL.setIPAddress(_dhcp->getLocalIp().raw_address());
    CPL.setGatewayIp(_dhcp->getGatewayIp().raw_address());
    CPL.setSubnetMask(_dhcp->getSubnetMask().raw_address());
    _dnsServerAddress = _dhcp->getDnsServerIp();
  }

  return ret;
}

void EtherCPLClass::begin(uint8_t *mac_address, IPAddress local_ip)
{
  // Assume the DNS server will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress dns_server = local_ip;
  dns_server[3] = 1;
  begin(mac_address, local_ip, dns_server);
}

void EtherCPLClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server)
{
  // Assume the gateway will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress gateway = local_ip;
  gateway[3] = 1;
  begin(mac_address, local_ip, dns_server, gateway);
}

void EtherCPLClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
{
  IPAddress subnet(255, 255, 255, 0);
  begin(mac_address, local_ip, dns_server, gateway, subnet);
}

void EtherCPLClass::begin(uint8_t *mac, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
  CPL.init();
  CPL.setMACAddress(mac);
  CPL.setIPAddress(local_ip.raw_address());
  CPL.setGatewayIp(gateway.raw_address());
  CPL.setSubnetMask(subnet.raw_address());
  _dnsServerAddress = dns_server;
}

int EtherCPLClass::maintain()
{
  int rc = DHCP_CHECK_NONE;
  if(_dhcp != NULL){
    //we have a pointer to dhcp, use it
    rc = _dhcp->checkLease();
    switch ( rc ){
      case DHCP_CHECK_NONE:
        //nothing done
        break;
      case DHCP_CHECK_RENEW_OK:
      case DHCP_CHECK_REBIND_OK:
        //we might have got a new IP.
        CPL.setIPAddress(_dhcp->getLocalIp().raw_address());
        CPL.setGatewayIp(_dhcp->getGatewayIp().raw_address());
        CPL.setSubnetMask(_dhcp->getSubnetMask().raw_address());
        _dnsServerAddress = _dhcp->getDnsServerIp();
        break;
      default:
        //this is actually a error, it will retry though
        break;
    }
  }
  return rc;
}

IPAddress EtherCPLClass::localIP()
{
  IPAddress ret;
  CPL.getIPAddress(ret.raw_address());
  return ret;
}

IPAddress EtherCPLClass::subnetMask()
{
  IPAddress ret;
  CPL.getSubnetMask(ret.raw_address());
  return ret;
}

IPAddress EtherCPLClass::gatewayIP()
{
  IPAddress ret;
  CPL.getGatewayIp(ret.raw_address());
  return ret;
}

IPAddress EtherCPLClass::dnsServerIP()
{
  return _dnsServerAddress;
}

EtherCPLClass Ethernet;
