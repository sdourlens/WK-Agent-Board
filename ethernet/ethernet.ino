// Arduino Due - EMAC Sample 1
// Brief EMAC example for Arduino Due
// This example demonstrates how to configure and use
// the EMAC peripheral.
// By Sebastien Dourlens @2015

// Required libraries
#include "variant.h"
#include <include/emac.h>
#include <source/emac.c>
#include "mini_ip.h"  // D:\Architecture\carte agent\ethernet\

/** Number of buffer for RX */
#define EMAC_RX_BUFFERS  16

/** Number of buffer for TX */
#define EMAC_TX_BUFFERS  8

/** MAC PHY operation max retry count */
#define MAC_PHY_RETRY_MAX 1000000

/** MAC address definition.  The MAC address must be unique on the network. */
#define ETHERNET_CONF_ETHADDR0                        0x00
#define ETHERNET_CONF_ETHADDR1                        0x00
#define ETHERNET_CONF_ETHADDR2                        0x01
#define ETHERNET_CONF_ETHADDR3                        0x00
#define ETHERNET_CONF_ETHADDR4                        0x00
#define ETHERNET_CONF_ETHADDR5                        0x02

/** WAN Address: 192.168.0.2 */
/* The IP address being used. */
#define ETHERNET_CONF_IPADDR0                         192
#define ETHERNET_CONF_IPADDR1                         168
#define ETHERNET_CONF_IPADDR2                         1
#define ETHERNET_CONF_IPADDR3                         2

/** WAN gateway: 192.168.0.1 */
/** The gateway address being used. */
#define ETHERNET_CONF_GATEWAY_ADDR0                   192
#define ETHERNET_CONF_GATEWAY_ADDR1                   168
#define ETHERNET_CONF_GATEWAY_ADDR2                   1
#define ETHERNET_CONF_GATEWAY_ADDR3                   254

/** The network mask being used. */
#define ETHERNET_CONF_NET_MASK0                       255
#define ETHERNET_CONF_NET_MASK1                       255
#define ETHERNET_CONF_NET_MASK2                       255
#define ETHERNET_CONF_NET_MASK3                       0

/** EMAC PHY address : should be 1 not 0 for DP8... */
#define BOARD_EMAC_PHY_ADDR 1
/*! EMAC RMII mode */
#define BOARD_EMAC_MODE_RMII 1
/** Ethernet MII/RMII mode */
#define ETH_PHY_MODE  BOARD_EMAC_MODE_RMII

/** DP PHY OUI MSB */
#define MII_OUI_MSB     0x2000 
/** DP PHY OUI LSB */
#define MII_OUI_LSB     0x5c90   

#define   DP83848_PHY_REG_BMCR   0x00
#define   DP83848_PHY_REG_BMSR   0x01
#define   DP83848_PHY_REG_PHYIDR1   0x02
#define   DP83848_PHY_REG_PHYIDR2   0x03
#define   DP83848_PHY_REG_ANAR   0x04
#define   DP83848_PHY_REG_ANLPAR   0x05
#define   DP83848_PHY_REG_ANER   0x06
#define   DP83848_PHY_REG_ANNPTR   0x07
#define   DP83848_PHY_REG_PHYSTS   0x10
#define   DP83848_PHY_REG_MICR   0x11
#define   DP83848_PHY_REG_MISR   0x12
#define   DP83848_PHY_REG_FCSCR   0x14
#define   DP83848_PHY_REG_RECR   0x15
#define   DP83848_PHY_REG_PCSR   0x16
#define   DP83848_PHY_REG_RBR   0x17
#define   DP83848_PHY_REG_LEDCR   0x18
#define   DP83848_PHY_REG_PHYCR   0x19
#define   DP83848_PHY_REG_10BTSCR   0x1A
#define   DP83848_PHY_REG_CDCTRL1   0x1B 
#define   DP83848_PHY_REG_EDCR   0x1D

#define   BMCR_RESET   (1 << 15)
#define   BMCR_LOOPBACK   (1 << 14)
#define   BMCR_SPEED_SEL   (1 << 13)
#define   BMCR_AN_EN   (1 << 12)
#define   BMCR_POWER_DOWN   (1 << 11)
#define   BMCR_ISOLATE   (1 << 10)
#define   BMCR_RESTART_AN   (1 << 9)
#define   BMCR_DUPLEX_MODE   (1 << 8)
#define   BMCR_COL_TEST   (1 << 7)
 
#define   BMSR_100BT4   (1 << 15)
#define   BMSR_100BTX_FD   (1 << 14)
#define   BMSR_100BTX   (1 << 13)
#define   BMSR_10BT_FD   (1 << 12)
#define   BMSR_10BT   (1 << 11)
#define   BMSR_NO_PREAMBLE   (1 << 6)
#define   BMSR_AN_COMPLETE   (1 << 5)
#define   BMSR_REMOTE_FAULT   (1 << 4)
#define   BMSR_AN_ABLE   (1 << 3)
#define   BMSR_LINK_STATUS   (1 << 2)
#define   BMSR_JABBER_DETECT   (1 << 1)
#define   BMSR_EXTENDED_CAP   (1 << 0)
 
#define   ANAR_NP   (1 << 15)
#define   ANAR_RF   (1 << 13)
 
#define   ANAR_ASM_DIR   (1 << 11)
#define   ANAR_PAUSE   (1 << 10)
#define   ANAR_100BT4   (1 << 9)
#define   ANAR_100BTX_FD   (1 << 8)
#define   ANAR_100BTX   (1 << 7)
#define   ANAR_10BT_FD   (1 << 6)
#define   ANAR_10BT   (1 << 5)
#define   ANAR_SELECTOR4   (1 << 4)
#define   ANAR_SELECTOR3   (1 << 3)
#define   ANAR_SELECTOR2   (1 << 2)
#define   ANAR_SELECTOR1   (1 << 1)
#define   ANAR_SELECTOR0   (1 << 0)
 
#define   ANLPAR_NP   (1 << 15)
#define   ANLPAR_ACK   (1 << 14)
#define   ANLPAR_RF   (1 << 13)
#define   ANLPAR_ASM_DIR   (1 << 11)
#define   ANLPAR_PAUSE   (1 << 10)
#define   ANLPAR_100BT4   (1 << 9)
#define   ANLPAR_100BTX_FD   (1 << 8)
#define   ANLPAR_100BTX   (1 << 7)
#define   ANLPAR_10BT_FD   (1 << 6)
#define   ANLPAR_10BT   (1 << 5)
#define   ANLPAR_SELECTOR4   (1 << 4)
#define   ANLPAR_SELECTOR3   (1 << 3)
#define   ANLPAR_SELECTOR2   (1 << 2)
#define   ANLPAR_SELECTOR1   (1 << 1) 
#define   ANLPAR_SELECTOR0   (1 << 0)

#define   ANER_PDF   (1 << 4)
#define   ANER_LP_NP_ABLE   (1 << 3)
#define   ANER_NP_ABLE   (1 << 2)
#define   ANER_PAGE_RX   (1 << 1)
#define   ANER_LP_AN_ABLE   (1 << 0)
 
#define   ANNPTR_NP   (1 << 15)
#define   ANNPTR_MP   (1 << 13)
#define   ANNPTR_ACK2   (1 << 12)
#define   ANNPTR_TOG_TX   (1 << 11)
#define   ANNPTR_CODE10   (1 << 10)
#define   ANNPTR_CODE9   (1 << 9)
#define   ANNPTR_CODE8   (1 << 8)
#define   ANNPTR_CODE7   (1 << 7)
#define   ANNPTR_CODE6   (1 << 6)
#define   ANNPTR_CODE5   (1 << 5)
#define   ANNPTR_CODE4   (1 << 4)
#define   ANNPTR_CODE3   (1 << 3)
#define   ANNPTR_CODE2   (1 << 2)
#define   ANNPTR_CODE1   (1 << 1)
#define   ANNPTR_CODE0   (1 << 0)
 
#define   PHYSTS_MDIX_MODE   (1 << 14)
#define   PHYSTS_RX_ERROR_LATCH   (1 << 13)
#define   PHYSTS_POLARITY_STATUS   (1 << 12)
#define   PHYSTS_FALSE_CARRIER_SENSE   (1 << 11)
#define   PHYSTS_SIGNAL_DETECT   (1 << 10)
#define   PHYSTS_DESCRAMBLER_LOCK   (1 << 9)
#define   PHYSTS_PAGE_RECEIVED   (1 << 8)
#define   PHYSTS_MII_INTERRUPT   (1 << 7)
#define   PHYSTS_REMOTE_FAULT   (1 << 6)
#define   PHYSTS_JABBER_DETECT   (1 << 5)
#define   PHYSTS_AN_COMPLETE   (1 << 4)
#define   PHYSTS_LOOPBACK_STATUS   (1 << 3)
#define   PHYSTS_DUPLEX_STATUS   (1 << 2)
#define   PHYSTS_SPEED_STATUS   (1 << 1)
#define   PHYSTS_LINK_STATUS   (1 << 0)
 
#define   MICR_TINT   (1 << 2)
#define   MICR_INTEN   (1 << 1)
#define   MICR_INT_OE   (1 << 0)
 
#define   MISR_ED_INT   (1 << 14)
#define   MISR_LINK_INT   (1 << 13)
#define   MISR_SPD_INT   (1 << 12)
#define   MISR_DUP_INT   (1 << 11)
#define   MISR_ANC_INT   (1 << 10)
#define   MISR_FHF_INT   (1 << 9)
#define   MISR_RHF_INT   (1 << 8)
 
#define   MISR_ED_INT_EN   (1 << 6)
#define   MISR_LINK_INT_EN   (1 << 5)
#define   MISR_SPD_INT_EN   (1 << 4)
#define   MISR_DUP_INT_EN   (1 << 3)
#define   MISR_ANC_INT_EN   (1 << 2)
#define   MISR_FHF_INT_EN   (1 << 1) 
#define   MISR_RHF_INT_EN   (1 << 0)

#define   FCSCR_FCSCNT7   (1 << 7)
#define   FCSCR_FCSCNT6   (1 << 6)
#define   FCSCR_FCSCNT5   (1 << 5)
#define   FCSCR_FCSCNT4   (1 << 4)
#define   FCSCR_FCSCNT3   (1 << 3)
#define   FCSCR_FCSCNT2   (1 << 2)
#define   FCSCR_FCSCNT1   (1 << 1)
#define   FCSCR_FCSCNT0   (1 << 0)
 
#define   RECR_RXERCNT7   (1 << 7)
#define   RECR_RXERCNT6   (1 << 6)
#define   RECR_RXERCNT5   (1 << 5)
#define   RECR_RXERCNT4   (1 << 4)
#define   RECR_RXERCNT3   (1 << 3)
#define   RECR_RXERCNT2   (1 << 2)
#define   RECR_RXERCNT1   (1 << 1)
#define   RECR_RXERCNT0   (1 << 0)
 
#define   PCSR_TQ_EN   (1 << 10)
#define   PCSR_SD_FORCE_PMA   (1 << 9)
#define   PCSR_SD_OPTION   (1 << 8)
#define   PCSR_DESC_TIME   (1 << 7)
#define   PCSR_FORCE_100_OK   (1 << 5)
#define   PCSR_NRZI_BYPASS   (1 << 2)
 
#define   RBR_RMII_MODE   (1 << 5)
#define   RBR_RMII_REV1_0   (1 << 4)
#define   RBR_RX_OVF_STS   (1 << 3)
#define   RBR_RX_UNF_STS   (1 << 2) 
#define   RBR_ELAST_BUF1   (1 << 1)
#define   RBR_ELAST_BUF0   (1 << 0)

#define   LEDCR_DRV_SPDLED   (1 << 5)
#define   LEDCR_DRV_LNKLED   (1 << 4)
#define   LEDCR_DRV_ACTLED   (1 << 3)
#define   LEDCR_SPDLED   (1 << 2)
#define   LEDCR_LNKLED   (1 << 1)
#define   LEDCR_ACTLED   (1 << 0)

#define   PHYCR_MDIX_EN   (1 << 15)
#define   PHYCR_FORCE_MDIX   (1 << 14)
#define   PHYCR_PAUSE_RX   (1 << 13)
#define   PHYCR_PAUSE_TX   (1 << 12) 
#define   PHYCR_BIST_FE   (1 << 11)
#define   PHYCR_PSR_15   (1 << 10)
#define   PHYCR_BIST_STATUS   (1 << 9)
#define   PHYCR_BIST_START   (1 << 8)
#define   PHYCR_BP_STRETCH   (1 << 7)
#define   PHYCR_LED_CNFG1   (1 << 6)
#define   PHYCR_LED_CNFG0   (1 << 5)
#define   PHYCR_PHYADDR4   (1 << 4)
#define   PHYCR_PHYADDR3   (1 << 3)
#define   PHYCR_PHYADDR2   (1 << 2) 
#define   PHYCR_PHYADDR1   (1 << 1)
#define   PHYCR_PHYADDR0   (1 << 0)

#define   _10BTSCR_10BT_SERIAL   (1 << 15)
#define   _10BTSCR_SQUELCH2   (1 << 11)
#define   _10BTSCR_SQUELCH1   (1 << 10)
#define   _10BTSCR_SQUELCH0   (1 << 9)
#define   _10BTSCR_LOOPBACK_10_DIS   (1 << 8)
#define   _10BTSCR_LP_DIS   (1 << 7)
#define   _10BTSCR_FORCE_LINK_10   (1 << 6)
#define   _10BTSCR_POLARITY   (1 << 4)
#define   _10BTSCR_HEARTBEAT_DIS   (1 << 1)
#define   _10BTSCR_JABBER_DIS   (1 << 0)
 
#define   CDCTRL1_BIST_ERROR_COUNT7   (1 << 15)
#define   CDCTRL1_BIST_ERROR_COUNT6   (1 << 14)
#define   CDCTRL1_BIST_ERROR_COUNT5   (1 << 13)
#define   CDCTRL1_BIST_ERROR_COUNT4   (1 << 12)
#define   CDCTRL1_BIST_ERROR_COUNT3   (1 << 11)
#define   CDCTRL1_BIST_ERROR_COUNT2   (1 << 10)
#define   CDCTRL1_BIST_ERROR_COUNT1   (1 << 9)
#define   CDCTRL1_BIST_ERROR_COUNT0   (1 << 8)
#define   CDCTRL1_BIST_CONT_MODE   (1 << 5)
#define   CDCTRL1_CDPATTEN_10   (1 << 4)
#define   CDCTRL1_10MEG_PATT_GAP   (1 << 2)
#define   CDCTRL1_CDPATTSEL1   (1 << 1)
#define   CDCTRL1_CDPATTSEL0   (1 << 0)
 
#define   EDCR_ED_EN   (1 << 15)
#define   EDCR_ED_AUTO_UP   (1 << 14)
#define   EDCR_ED_AUTO_DOWN   (1 << 13)
#define   EDCR_ED_MAN   (1 << 12)
#define   EDCR_ED_BURST_DIS   (1 << 11)
#define   EDCR_ED_PWR_STATE   (1 << 10)
#define   EDCR_ED_ERR_MET   (1 << 9)
#define   EDCR_ED_DATA_MET   (1 << 8)
#define   EDCR_ED_ERR_COUNT3   (1 << 7)
#define   EDCR_ED_ERR_COUNT2   (1 << 6)
#define   EDCR_ED_ERR_COUNT1   (1 << 5)
#define   EDCR_ED_ERR_COUNT0   (1 << 4)
#define   EDCR_ED_DATA_COUNT3   (1 << 3)
#define   EDCR_ED_DATA_COUNT2   (1 << 2)
#define   EDCR_ED_DATA_COUNT1   (1 << 1)
#define   EDCR_ED_DATA_COUNT0   (1 << 0)

// The MAC address used for the test
static uint8_t gs_uc_mac_address[] =
		{ ETHERNET_CONF_ETHADDR0, ETHERNET_CONF_ETHADDR1, ETHERNET_CONF_ETHADDR2,
			ETHERNET_CONF_ETHADDR3, ETHERNET_CONF_ETHADDR4, ETHERNET_CONF_ETHADDR5
};

// The IP address used for test (ping ...)
static uint8_t gs_uc_ip_address[] =
		{ ETHERNET_CONF_IPADDR0, ETHERNET_CONF_IPADDR1,
			ETHERNET_CONF_IPADDR2, ETHERNET_CONF_IPADDR3 };

// The EMAC driver instance
static emac_device_t gs_emac_dev;

// Buffer for ethernet packets
static volatile uint8_t gs_uc_eth_buffer[EMAC_FRAME_LENTGH_MAX];

emac_options_t emac_option;
char netjob;
uint32_t ul_value=0;
uint8_t uc_rc = EMAC_TIMEOUT;

// EMAC interrupt handler.
void EMAC_Handler(void)
{
	emac_handler(&gs_emac_dev);
}

void init_mac() {
    volatile uint32_t ul_delay;

    // Display MAC & IP settings
    printf("init PHY\n-- MAC %x:%x:%x:%x:%x:%x\n",
      gs_uc_mac_address[0], gs_uc_mac_address[1], gs_uc_mac_address[2],
      gs_uc_mac_address[3], gs_uc_mac_address[4], gs_uc_mac_address[5]);

    printf("-- IP  %d.%d.%d.%d\n", gs_uc_ip_address[0], gs_uc_ip_address[1],
      gs_uc_ip_address[2], gs_uc_ip_address[3]);

    // Wait for PHY to be ready (CAT811: Max400ms)
    ul_delay = SystemCoreClock / 1000 / 3 * 400;
    while (ul_delay--); /* */

    // Enable EMAC clock
    pmc_enable_periph_clk(ID_EMAC);

    // Fill in EMAC options
    emac_option.uc_copy_all_frame = 0;
    emac_option.uc_no_boardcast = 0;
    memcpy(emac_option.uc_mac_addr, gs_uc_mac_address, 6);
    gs_emac_dev.p_hw = EMAC;
  
    printf("Init EMAC driver structure\n");
    // Init EMAC driver structure
    emac_dev_init(EMAC, &gs_emac_dev, &emac_option);

    // Enable Interrupt
    NVIC_EnableIRQ(EMAC_IRQn);
}

// init phy 
char init_phy(char doreset,char doint,uint32_t mck) {
    emac_enable_management(EMAC, 1);

    // RMII mode
    emac_enable_rmii(EMAC, ETH_PHY_MODE);

    /* Configure EMAC runtime clock */
    uc_rc = emac_set_clock(EMAC, mck);
    if (uc_rc != EMAC_OK) {
       Serial.println("Emac clock failed");
       return 0;
      }

    /* Check the current PHY address */
    if (emac_phy_read(EMAC, BOARD_EMAC_PHY_ADDR, DP83848_PHY_REG_PHYIDR1, &ul_value) != EMAC_OK) {
       Serial.println("Read ID phy failed (emac read issue)");
       return 0;
      }
    if (ul_value != MII_OUI_MSB) {
       Serial.println("PHY is not a DP83848, not an agent board?");
       return 0;
      }

    // reset settings
    if (doreset) {
        // Reset PHY transceiver
        ul_value=BMCR_RESET;
        if (emac_phy_write(EMAC, BOARD_EMAC_PHY_ADDR, DP83848_PHY_REG_BMCR, ul_value) != EMAC_OK) {
           Serial.println("Reset phy failed (emac write issue)");
           return 0;
          }
        // Wait for the reset to complete
        do {
           if (emac_phy_read(EMAC,BOARD_EMAC_PHY_ADDR, DP83848_PHY_REG_BMCR, &ul_value) != EMAC_OK) {
              Serial.println("Reset phy failed (emac read issue)\r\n");
              return 0;
             }
          } while(ul_value & BMCR_RESET);
      }

    // interrupt for link status change
    if (doint) {
       //Configure PWR_DOWN/INT pin as an interrupt output
       ul_value=MICR_INTEN | MICR_INT_OE;
       if (emac_phy_write(EMAC, BOARD_EMAC_PHY_ADDR, DP83848_PHY_REG_MICR, ul_value) != EMAC_OK) {
           Serial.println("Init interrupt phy failed (emac write issue)");
           return 0;
          }       
       //The PHY will generate interrupts when link status changes are detected
        ul_value=MISR_LINK_INT_EN;
        if (emac_phy_write(EMAC, BOARD_EMAC_PHY_ADDR, DP83848_PHY_REG_MISR, ul_value) != EMAC_OK) {
           Serial.println("Enable interrupt phy failed (emac write issue)");
           return 0;
          }
    }

    // very important to read phy RXD
    emac_enable_transceiver_clock(EMAC, true);

    emac_enable_management(EMAC, 0);
    //Successful initialization
    return 1;
}

char set_phy(char autoneg, char b_speed,char b_duplex) {
    emac_enable_management(EMAC, 1);

    if (autoneg) {
       ul_value=BMCR_AN_EN; 
       if (emac_phy_write(EMAC, BOARD_EMAC_PHY_ADDR, DP83848_PHY_REG_BMCR, ul_value) != EMAC_OK) {
                 Serial.println("Autoneg failed (emac write issue)");
                 emac_enable_management(EMAC, 0);
                 return 0;
                }       
       if (emac_phy_read(EMAC, BOARD_EMAC_PHY_ADDR, DP83848_PHY_REG_BMSR, &ul_value) != EMAC_OK) {
                 Serial.println("Autoneg failed (emac read1 issue)");
                 emac_enable_management(EMAC, 0);
                 return 0;
                }
       if ((ul_value & BMSR_AN_COMPLETE)==0) { // second times
          delay(1000);
          if (emac_phy_read(EMAC, BOARD_EMAC_PHY_ADDR, DP83848_PHY_REG_BMSR, &ul_value) != EMAC_OK) {
                 Serial.println("Autoneg failed (emac read2 issue)");
                 emac_enable_management(EMAC, 0);
                 return 0;
                }
           if ((ul_value & BMSR_AN_COMPLETE)==0) {
                 Serial.println("Autoneg not complete failed");
                 emac_enable_management(EMAC, 0);
                 return 0;
            }
         }        
       // check given speed & duplex
       b_speed=1; b_duplex=1;
       // set emac depending on autonegociation
       emac_set_speed(EMAC, b_speed);
       emac_enable_full_duplex(EMAC, b_duplex);      
      }
    else {
          ul_value=0x2100; // fixed for now, To be changed for 10 mb/s or halfduplex
          if (emac_phy_write(EMAC, BOARD_EMAC_PHY_ADDR, DP83848_PHY_REG_BMCR, ul_value) != EMAC_OK) {
                 Serial.println("Set phy failed (emac write issue)");
                 emac_enable_management(EMAC, 0);
                 return 0;
                }
          // wait a little 1s
          delay(1000);
          // set emac
          emac_set_speed(EMAC, b_speed);
          emac_enable_full_duplex(EMAC, b_duplex);
      }
    emac_enable_management(EMAC, 0);
    return 1;
}

char check_link() {
  uint32_t v;
      emac_enable_management(EMAC, 1);
      // Read basic status register
      if (emac_phy_read(EMAC,BOARD_EMAC_PHY_ADDR, DP83848_PHY_REG_PHYSTS, &v) != EMAC_OK) {
              Serial.println("Read status failed (emac read issue)");
              emac_enable_management(EMAC, 0);
              return 0;
              }
      emac_enable_management(EMAC, 0);
      //Retrieve current link state
      //printf("state: %lx\n",v);      
      return (v & PHYSTS_LINK_STATUS); 
}

void display_phy() {
  uint32_t ul_value1 = 0;
  uint32_t ul_value2 = 0;
  uint8_t i;
  char sp,fd,lb,ac,rel,ps,fcsl,sd,dl,pr,mp,rf;

  char *names[32]={ "BMCR","BMSR*","PHYIDR1*","PHYIDR2*","ANAR","ANLPAR","ANLPARNP",
                    "ANER","ANNPTR","PHYSTS*","MICR","MISR*" };

  emac_enable_management(EMAC, 1);
  for(i = 0; i < 8; i++){
    //Display current PHY register
    emac_phy_read(EMAC,BOARD_EMAC_PHY_ADDR, i, &ul_value1);
    printf("%s: %lx\n", names[i], ul_value1);
    }
  for(i = 16; i < 19; i++){
    //Display current PHY register values
    emac_phy_read(EMAC,BOARD_EMAC_PHY_ADDR, i, &ul_value1);
    printf("%s: %lx\n", names[i-8], ul_value1);
    }
    
  emac_phy_read(EMAC,BOARD_EMAC_PHY_ADDR, DP83848_PHY_REG_PHYSTS, &ul_value1); // PHYSTS reg reading
  sp= ((ul_value1 & PHYSTS_SPEED_STATUS) ==0);
  fd=((ul_value1 & PHYSTS_DUPLEX_STATUS) >0);
  lb=((ul_value1 & PHYSTS_LOOPBACK_STATUS) >0);
  ac=((ul_value1 & PHYSTS_AN_COMPLETE) >0);
  rel=((ul_value1 & PHYSTS_RX_ERROR_LATCH) >0);
  ps=((ul_value1 & PHYSTS_POLARITY_STATUS) >0);
  fcsl=((ul_value1 & PHYSTS_FALSE_CARRIER_SENSE) >0);
  sd=((ul_value1 & PHYSTS_SIGNAL_DETECT) >0);
  dl=((ul_value1 & PHYSTS_DESCRAMBLER_LOCK) >0);
  pr=((ul_value1 & PHYSTS_PAGE_RECEIVED) >0);
  mp=((ul_value1 & PHYSTS_MII_INTERRUPT) >0);
  rf=((ul_value1 & PHYSTS_REMOTE_FAULT) >0);

  printf("PHY: link %s\nstate: %s,%s,%s,%s\n%s,%s,%s,%s\n%s,%s,%s,%s\n",
         (ul_value1 & 1) ? "up":"down", 
         sp ? "100BASE-TX" : "10BASE-T",
         fd ? "Full-Duplex" : "Half-Duplex",
         lb ? "Loopback enabled" : "normal",
         ac ? "Auto-negotation complete" : "Auto-negotation not complete",
         rel ? "Latch Error" : "Latch ok",
         ps ? "Inverted polarity" : "Correct Polarity",
         fcsl ? "False Carrier" : "No False Carrier",
         sd ? "100Base-TX signal detected" : "No 100Base-TX signal detected",
         dl ? "Descrambler lock" : "No descrambler lock",
         pr ? "New link word page" : "Same link word page",
         mp ? "Interrupt pending" : "No interrupt",
         rf ? "Remote Fault" : "No remote fault");   

  emac_phy_read(EMAC,BOARD_EMAC_PHY_ADDR, DP83848_PHY_REG_BMSR, &ul_value2); // read up link in BMSR 
  sp= ((ul_value2 & BMSR_100BTX_FD)>0);
  ac=((ul_value2 & BMSR_AN_COMPLETE) >0);
  fd=((ul_value2 & BMSR_NO_PREAMBLE) >0);
  lb=((ul_value2 & BMSR_REMOTE_FAULT) >0);
  rel=((ul_value2 & BMSR_AN_ABLE) >0);

  printf("BMSR: link %s\nstate: %s,%s,%s,%s,%s\n",
         (ul_value2 & BMSR_LINK_STATUS) ? "up":"down", 
         sp ? "100BT FD" : "Not 100BT FD",
         lb ? "Remote Fault" : "No remote fault",
         ac ? "Auto-negotation complete" : "Auto-negotation not complete",
         fd ? "No Preamble" : "Preamble",
         rel ? "AutoNeg Able" : "No AutoNeg Able");   
         
  emac_enable_management(EMAC, 0);
}

/************************************ MAC LAYER *********************************/


static uint16_t emac_icmp_checksum(uint16_t *p_buff, uint32_t ul_len)
{
  uint32_t i, ul_tmp;

  for (i = 0, ul_tmp = 0; i < ul_len; i++, p_buff++) {

    ul_tmp += SWAP16(*p_buff);
  }
  ul_tmp = (ul_tmp & 0xffff) + (ul_tmp >> 16);

  return (uint16_t) (~ul_tmp);
}

/**
 * \brief Display the IP packet.
 *
 * \param p_ip_header Pointer to the IP header.
 * \param ul_size    The data size.
 */
static void emac_display_ip_packet(p_ip_header_t p_ip_header, uint32_t ul_size)
{
  printf("======= IP %4d bytes, HEADER ==========\n\r", (int)ul_size);
  printf(" IP Version        = v%d", (p_ip_header->ip_hl_v & 0xF0) >> 4);
  printf("\n\r Header Length     = %d", p_ip_header->ip_hl_v & 0x0F);
  printf("\n\r Type of service   = 0x%x", p_ip_header->ip_tos);
  printf("\n\r Total IP Length   = 0x%X",
      (((p_ip_header->ip_len) >> 8) & 0xff) +
      (((p_ip_header->ip_len) << 8) & 0xff00));
  printf("\n\r ID                = 0x%X",
      (((p_ip_header->ip_id) >> 8) & 0xff) +
      (((p_ip_header->ip_id) << 8) & 0xff00));
  printf("\n\r Header Checksum   = 0x%X",
      (((p_ip_header->ip_sum) >> 8) & 0xff) +
      (((p_ip_header->ip_sum) << 8) & 0xff00));
  puts("\n\r Protocol          = ");

  switch (p_ip_header->ip_p) {
  case IP_PROT_ICMP:
    puts("ICMP");
    break;

  case IP_PROT_IP:
    puts("IP");
    break;

  case IP_PROT_TCP:
    puts("TCP");
    break;

  case IP_PROT_UDP:
    puts("UDP");
    break;

  default:
    printf("%d (0x%X)", p_ip_header->ip_p, p_ip_header->ip_p);
    break;
  }

  printf("\n\r IP Src Address    = %d:%d:%d:%d",
      p_ip_header->ip_src[0],
      p_ip_header->ip_src[1],
      p_ip_header->ip_src[2], p_ip_header->ip_src[3]);

  printf("\n\r IP Dest Address   = %d:%d:%d:%d",
      p_ip_header->ip_dst[0],
      p_ip_header->ip_dst[1],
      p_ip_header->ip_dst[2], p_ip_header->ip_dst[3]);
  puts("\n\r----------------------------------------\r");
}

/**
 * \brief Process the received ARP packet; change address and send it back.
 *
 * \param p_uc_data  The data to process.
 * \param ul_size The data size.
 */
static void emac_process_arp_packet(uint8_t *p_uc_data, uint32_t ul_size)
{
  uint32_t i;
  uint8_t ul_rc = EMAC_OK;

  p_ethernet_header_t p_eth = (p_ethernet_header_t) p_uc_data;
  p_arp_header_t p_arp = (p_arp_header_t) (p_uc_data + ETH_HEADER_SIZE);

  if (SWAP16(p_arp->ar_op) == ARP_REQUEST) {
    printf("-- IP  %d.%d.%d.%d\n\r",
        p_eth->et_dest[0], p_eth->et_dest[1],
        p_eth->et_dest[2], p_eth->et_dest[3]);

    printf("-- IP  %d.%d.%d.%d\n\r",
        p_eth->et_src[0], p_eth->et_src[1],
        p_eth->et_src[2], p_eth->et_src[3]);

    /* ARP reply operation */
    p_arp->ar_op = SWAP16(ARP_REPLY);

    /* Fill the destination address and source address */
    for (i = 0; i < 6; i++) {
      /* Swap ethernet destination address and ethernet source address */
      p_eth->et_dest[i] = p_eth->et_src[i];
      p_eth->et_src[i] = gs_uc_mac_address[i];
      p_arp->ar_tha[i] = p_arp->ar_sha[i];
      p_arp->ar_sha[i] = gs_uc_mac_address[i];
    }
    /* Swap the source IP address and the destination IP address */
    for (i = 0; i < 4; i++) {
      p_arp->ar_tpa[i] = p_arp->ar_spa[i];
      p_arp->ar_spa[i] = gs_uc_ip_address[i];
    }
    ul_rc = emac_dev_write(&gs_emac_dev, p_uc_data, ul_size, NULL);
    if (ul_rc != EMAC_OK) {
      printf("E: ARP Send - 0x%x\n\r", ul_rc);
    }
  }
}

/**
 * \brief Process the received IP packet; change address and send it back.
 *
 * \param p_uc_data  The data to process.
 * \param ul_size The data size.
 */
static void emac_process_ip_packet(uint8_t *p_uc_data, uint32_t ul_size)
{
  uint32_t i;
  uint32_t ul_icmp_len;
  int32_t ul_rc = EMAC_OK;

  p_ethernet_header_t p_eth = (p_ethernet_header_t) p_uc_data;
  p_ip_header_t p_ip_header = (p_ip_header_t) (p_uc_data + ETH_HEADER_SIZE);

  p_icmp_echo_header_t p_icmp_echo =
      (p_icmp_echo_header_t) ((int8_t *) p_ip_header +
      ETH_IP_HEADER_SIZE);
  printf("-- IP  %d.%d.%d.%d\n\r", p_eth->et_dest[0], p_eth->et_dest[1],
      p_eth->et_dest[2], p_eth->et_dest[3]);

  printf("-- IP  %d.%d.%d.%d\n\r",
      p_eth->et_src[0], p_eth->et_src[1], p_eth->et_src[2],
      p_eth->et_src[3]);
  switch (p_ip_header->ip_p) {
  case IP_PROT_ICMP:
    if (p_icmp_echo->type == ICMP_ECHO_REQUEST) {
      p_icmp_echo->type = ICMP_ECHO_REPLY;
      p_icmp_echo->code = 0;
      p_icmp_echo->cksum = 0;

      /* Checksum of the ICMP message */
      ul_icmp_len = (SWAP16(p_ip_header->ip_len) - ETH_IP_HEADER_SIZE);
      if (ul_icmp_len % 2) {
        *((uint8_t *) p_icmp_echo + ul_icmp_len) = 0;
        ul_icmp_len++;
      }
      ul_icmp_len = ul_icmp_len / sizeof(uint16_t);

      p_icmp_echo->cksum = SWAP16(
          emac_icmp_checksum((uint16_t *)p_icmp_echo, ul_icmp_len));
      /* Swap the IP destination  address and the IP source address */
      for (i = 0; i < 4; i++) {
        p_ip_header->ip_dst[i] =
            p_ip_header->ip_src[i];
        p_ip_header->ip_src[i] = gs_uc_ip_address[i];
      }
      /* Swap ethernet destination address and ethernet source address */
      for (i = 0; i < 6; i++) {
        /* Swap ethernet destination address and ethernet source address */
        p_eth->et_dest[i] = p_eth->et_src[i];
        p_eth->et_src[i] = gs_uc_mac_address[i];
      }
      /* Send the echo_reply */
      ul_rc = emac_dev_write(&gs_emac_dev, p_uc_data,
          SWAP16(p_ip_header->ip_len) + 14, NULL);
      if (ul_rc != EMAC_OK) {
        printf("E: ICMP Send - 0x%x\n\r", ul_rc);
      }
    }
    break;

  default:
    break;
  }
}

/**
 * \brief Process the received EMAC packet.
 *
 * \param p_uc_data  The data to process.
 * \param ul_size The data size.
 */
static void emac_process_eth_packet(uint8_t *p_uc_data, uint32_t ul_size)
{
  uint16_t us_pkt_format;

  p_ethernet_header_t p_eth = (p_ethernet_header_t) (p_uc_data);
  p_ip_header_t p_ip_header = (p_ip_header_t) (p_uc_data + ETH_HEADER_SIZE);
  ip_header_t ip_header;
  us_pkt_format = SWAP16(p_eth->et_protlen);

  switch (us_pkt_format) {
  /* ARP Packet format */
  case ETH_PROT_ARP:
    /* Process the ARP packet */
    emac_process_arp_packet(p_uc_data, ul_size);

    break;

  /* IP protocol frame */
  case ETH_PROT_IP:
    /* Backup the header */
    memcpy(&ip_header, p_ip_header, sizeof(ip_header_t));

    /* Process the IP packet */
    emac_process_ip_packet(p_uc_data, ul_size);

    /* Dump the IP header */
    emac_display_ip_packet(&ip_header, ul_size);
    break;

  default:
    printf("=== Default w_pkt_format= 0x%X===\n\r", us_pkt_format);
    break;
  }
}

/************************************ MAIN **************************************/

void setup()
{
  Serial.begin(115200);
  // reset phy off
  pinMode(10,OUTPUT); // reset (D10 - PC29)
  digitalWrite(10, HIGH);
  pinMode(125,INPUT); // interrupt (D125 is ETHERNET_INT)

  // init mac & phy layers
  printf("Init MAC PHY driver\n");
  init_mac();
  netjob=1;
  if (!init_phy(0,0,SystemCoreClock)) { Serial.println("PHY Initialize ERROR!"); netjob=0; } else netjob=1;
  // 100 Mb/s Full Duplex
  if (!set_phy(1,1,1)) { Serial.println("PHY Settings ERROR!"); netjob=0; } else netjob=1;  
  display_phy();
}

void loop()
{  
 static char l=-1,lasterr=EMAC_OK,cl;
  uint32_t error;
  uint32_t ul_frm_size=0;

  if (netjob) {
    //clearAndHome();
     // Display ethernet link state
     cl=check_link();
     if (cl!=l) {
       if (cl) {
            display_phy();
            Serial.println("link up : 100Mb/s Full Duplex");              
            }      
        else {
            Serial.println("link down! Reset Phy & Mac layers.");
            
            }
      l=cl;
      //display_phy();       
      }

      // Read Ethernet packets
      if (cl) {
        // Process packets
        error=emac_dev_read(&gs_emac_dev, (uint8_t *) gs_uc_eth_buffer,sizeof(gs_uc_eth_buffer), &ul_frm_size);
        if (error!=EMAC_OK  && error!=lasterr) {
            // ERROR            
            if (error==EMAC_PARAM) Serial.println("Error reading packets: EMAC PARAM");
           // if (error==EMAC_RX_NULL) Serial.println("EMAC RX NULL");
            if (error==EMAC_SIZE_TOO_SMALL) Serial.println("Error reading packets: EMAC SIZE TOO SMALL");
            lasterr=error;
            }
        if (error==EMAC_OK)
           {
           Serial.println("Reception");
           lasterr=EMAC_OK; 
           if (ul_frm_size > 0) { // Handle input frame
              emac_process_eth_packet((uint8_t *) gs_uc_eth_buffer, ul_frm_size);
              }
           }
        }

    // TEMPO CHECK LINK
    if (cl==0) delay(1000);
    }
}

