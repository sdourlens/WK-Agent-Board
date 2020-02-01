/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "variant.h"
 
/* =============== AGENT BOARD ============================
 *   Arduino pin   |  PORT  | Agent Label
 * ----------------+--------+-------
 *   0       RXD0  |  PA8   | "RX0" PROG
 *   1       TXD0  |  PA9   | "TX0" PROG
 *   2       TIOA0 |  PB25  | not connected (arduino compatible PWM2)
 *   3       TIOA7 |  PC28  | ENABLE 3
 *   4       NPCS1 |  PA29  | not connected (arduino compatible PWM4, SPI-CS1:SD-CS)
 *   4       TIOB6 |  PC26  | not connected (arduino compatible)
 *   5       TIOA6 |  PC25  | not connected (arduino compatible PWM5)
 *   6       PWML7 |  PC24  | PWM4
 *   7       PWML6 |  PC23  | PWM3
 *   8       PWML5 |  PC22  | PWM2
 *   9       PWML4 |  PC21  | PWM1
 *  10       NPCS0 |  PA28  | SPI CS (J5) (arduino compatible PWM10, SPI-CS0:ETH-CS)
 *  10       TIOB7 |  PC29  | ETHERNET RESET (init pin shoud be added in main code) (diag compatible CAN1-INT)
 *  11       TIOA8 |  PD7   | A0
 *  12       TIOB8 |  PD8   | AUDIO ENABLE
 *  13       TIOB0 |  PB27  | TOR CMD6
 *  14       TXD3  |  PD4   | not connected 
 *  15       RXD3  |  PD5   | not connected 
 *  16       TXD2  |  PA13  | TXD1 
 *  17       RXD2  |  PA12  | RXD1 
 *  18       TXD1  |  PA11  | TXD0 
 *  19       RXD1  |  PA10  | RXD0 
 *  20       SDA   |  PB12  | ANA11
 *  21       SCL   |  PB13  | ANA12
 *  22       TCLK0 |  PB26  | TOR E1
 *  23             |  PA14  | OE: Select DAC0/Audio or CAN2
 *  24             |  PA15  | TOR E2
 *  25             |  PD0   | MCDA4
 *  26             |  PD1   | MCDA5
 *  27             |  PD2   | MCDA6
 *  28             |  PD3   | MCDA7
 *  29             |  PD6   | AUDIO SHDN
 *  30             |  PD9   | A1
 *  31             |  PA7   | TOR E3
 *  32             |  PD10  | not connected
 *  33             |  PC1   | PC1 ?
 *  34             |  PC2   | not connected
 *  35             |  PC3   | not connected
 *  36             |  PC4   | TOR DIAG2
 *  37             |  PC5   | TOR CMD1
 *  38             |  PC6   | TOR CMD2
 *  39             |  PC7   | TOR CMD3
 *  40             |  PC8   | TOR CMD4
 *  41             |  PC9   | TOR CMD5
 *  42             |  PA19  | 
 *  43             |  PA20  | 
 *  44             |  PC19  | TOR CMD7  (diag compatible SPI CAN2-CS)
 *  45             |  PC18  | TOR CMD8  (diag compatible SPI CAN2-INT)
 *  46             |  PC17  | TOR DIAG3 (diag compatible SPI SD-CS)
 *  47             |  PC16  | TOR DIAG8
 *  48             |  PC15  | TOR DIAG7
 *  49             |  PC14  | TOR DIAG6
 *  50             |  PC13  | TOR DIAG5
 *  51             |  PC12  | TOR DIAG4
 *  52       NPCS2 |  PB21  | ANA10 (arduino compatible AD14, SPI-CS2, RXD2)
 *  53       CANTX1|  PB14  | CANTX1
 *  54       A0    |  PA16  | ANA1 
 *  55       A1    |  PA24  | - MCDA3
 *  56       A2    |  PA23  | - MCDA2
 *  57       A3    |  PA22  | - MCDA1
 *  58       TIOB2 |  PA6   | ANA5 (arduino compatible A4)
 *  59             |  PA4   | ANA6 (arduino compatible A5)
 *  60       TIOB1 |  PA3   | ANA9 (arduino compatible A6, diag compatible LED2)
 *  61       TIOA1 |  PA2   | ANA7 (arduino compatible A7, diag compatible LED1)
 *  62             |  PB17  | ANA8 (arduino compatible A8)
 *  63             |  PB18  | ANA2 (arduino compatible A9)
 *  66       DAC0  |  PB15  | DAC0 (arduino compatible DAC0, not CANRX1)
 *  67       DAC1  |  PB16  | DAC1
 *  68       CANRX |  PA1   | CANRX0
 *  69       CANTX |  PA0   | CANTX0
 *  70       SDA1  |  PA17  | not connected (arduino compatible SCK0)
 *  71       SCL1  |  PA18  | TOR E4
 *  72             |  PC30  | TOR E5 (diag compatible CAN1-CS)
 *  73             |  PA21  | MCDA0
 *  74       MISO  |  PA25  | SPI MISO (J5) (diag compatible SPI-MISO)
 *  75       MOSI  |  PA26  | SPI MOSI (J5) (diag compatible SPI-MOSI)
 *  76       SCLK  |  PA27  | SPI CLK (J5)  (diag compatible SPI-SPCK)
 *  77       NPCS0 |  PA28  | SPI CS (J5) (arduino compatible SPI-CS0:ETH-CS)
 *  78       NPCS3 |  PB23  | TOR E8
 *  79             |  PB19  | ANA3 (TO DO) !
 *  80             |  PB20  | ANA4 (TO DO) !
 *  81             |  PA8-9 | UART (USB Serial PROG) 
 *  82             | PA10-11| USART0 (USB Serial1) 
 *  83             | PA12-13| USART1 (Serial2 direct connector) 
 *  84       USART3|        | (arduino compatible)
 *  85       USB   | PB10-11| USB UOTGID UOTGVBOF
 *  86       NPCS2 |        | (compatible arduino)
 *  87       NPCS1 | C26 A29| (compatible arduino SPI-CS1:SD-CS)
 *  88             |  PB15  | CANRX1 (compatible diag) not DAC0
 *  89             |  PB14  | CANTX1 (compatible diag)
 *  90             |  PA0-1 | CANTX0 CANRX0 BOTH (compatible diag)
 *  91             | PB14-15| CANRX1 TX1 BOTH (compatible diag)
 *  92             |        | (free for diag)
 *  93             |        | (free for diag)
 *  94             |        | (free for diag)
 *  95             |        | (free for diag)
 *  96             |        | (free for diag)
 *  97             |        | (free for diag)
 *  98             |        | (free for diag)
 *  99             |        | (free for diag)
 *  100            |  PC10  | TOR DIAG1  
 *  101            |  PC27  | TOR E6 !
 *  102            |  PA0   | 
 *  103            |  PA1   | - 
 *  104            |  PC11  | TOR E7 !
 *  105            |  PC8   | - 
 *  106            |  PC2   | - 
 *  107            |  PC6   | - 
 *  108            |  PC20  | ENABLE 1 !
 *  109            |  PA20  | MCCDA 
 *  110            |  PA19  | MCCK
 *  111            |  PA21  | MCDA0 
 *  112            |  PA22  | MCDA1
 *  113            |  PA23  | MCDA2
 *  114            |  PA34  | MCDA3
 *  115            |  PB0   | ETHERNET TX CLK
 *  116            |  PB1   | ETHERNET TX ENABLE
 *  117            |  PB2   | ETHERNET TX0
 *  118            |  PB3   | ETHERNET TX1
 *  119            |  PB4   | ETHERNET CRSDV
 *  120            |  PB5   | ETHERNET RX0
 *  121            |  PB6   | ETHERNET RX1
 *  122            |  PB7   | ETHERNET RXER
 *  123            |  PB8   | ETHERNET MDC
 *  124            |  PB9   | ETHERNET MDIO
 *  125            |  PA5   | ETHERNET TIOA2 INT
 *  126            |  PB21  | ANA10 
 *  127            |  PB11  | USB UOTG ID
 *  128            |  PB10  | USB UOTG VBOF 
 */
 
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Pins descriptions
 */
extern const PinDescription g_APinDescription[]=
{
  // 0 .. 53 - Digital pins
  // ----------------------
  // 0/1 - UART (Serial)
  { PIOA, PIO_PA8A_URXD,     ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                 NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // URXD
  { PIOA, PIO_PA9A_UTXD,     ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                 NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // UTXD

  // 2
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  { PIOC, PIO_PC28B_TIOA7,   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER     }, 
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 6 PWM4-1
  { PIOC, PIO_PC24B_PWML7,   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   NO_ADC, NO_ADC, PWM_CH7,     NOT_ON_TIMER }, // PWM4
  { PIOC, PIO_PC23B_PWML6,   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   NO_ADC, NO_ADC, PWM_CH6,     NOT_ON_TIMER }, // PWM3
  { PIOC, PIO_PC22B_PWML5,   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   NO_ADC, NO_ADC, PWM_CH5,     NOT_ON_TIMER }, // PWM2
  { PIOC, PIO_PC21B_PWML4,   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   NO_ADC, NO_ADC, PWM_CH4,     NOT_ON_TIMER }, // PWM1
  // 10
  { PIOC, PIO_PC29B_TIOB7,   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL), NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER     }, // ETH RST
  // { PIOD, PIO_PD7B_TIOA8,    ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER), NO_ADC, NO_ADC, NOT_ON_PWM,  TC2_CHA8     }, // A0 - arduino
  { PIOD, PIO_PD7,    ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // A0 
  { PIOD, PIO_PD8,    ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // AUDIO EN

  // 13 - TOR_CMD6
  { PIOB, PIO_PB27B_TIOB0,   ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL), NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER     }, // TOR CMD6

  // 14/15 - USART3 (Serial3)
  { PIOD, PIO_PD4B_TXD3,     ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TXD3
  { PIOD, PIO_PD5B_RXD3,     ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // RXD3

  // 16/17 - USART1 (Serial2)
  { PIOA, PIO_PA13A_TXD1,    ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TXD1
  { PIOA, PIO_PA12A_RXD1,    ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // RXD1

  // 18/19 - USART0 (Serial1)
  { PIOA, PIO_PA11A_TXD0,    ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TXD0
  { PIOA, PIO_PA10A_RXD0,    ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // RXD0

  // 20/21 - ANA (periph a or b ?), ADC0?, ADC1?
  { PIOB, PIO_PB12X1_AD8,    ID_PIOB, PIO_INPUT, PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC0,   ADC8, NOT_ON_PWM,  NOT_ON_TIMER }, // TWD1 - SDA0
  { PIOB, PIO_PB13X1_AD9,   ID_PIOB, PIO_INPUT, PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC1,   ADC9, NOT_ON_PWM,  NOT_ON_TIMER }, // TWCK1 - SCL0

  // 22
  { PIOB, PIO_PB26,    ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 22
  { PIOA, PIO_PA14,    ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 23 - OE
  { PIOA, PIO_PA15,    ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 24
  { PIOD, PIO_PD0,     ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 25

  // 26
  { PIOD, PIO_PD1,     ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 26
  { PIOD, PIO_PD2,     ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 27
  { PIOD, PIO_PD3,     ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 28
  { PIOD, PIO_PD6,     ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 29

  // 30
  { PIOD, PIO_PD9,     ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 30
  { PIOA, PIO_PA7,     ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 31
  { PIOD, PIO_PD10,    ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 32
  { PIOC, PIO_PC1,     ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 33

  // 34
  { PIOC, PIO_PC2,     ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 34
  { PIOC, PIO_PC3,     ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 35
  { PIOC, PIO_PC4,     ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 36
  { PIOC, PIO_PC5,     ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 37

  // 38
  { PIOC, PIO_PC6,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 38
  { PIOC, PIO_PC7,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 39
  { PIOC, PIO_PC8,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 40
  { PIOC, PIO_PC9,           ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 41

  // 42
  { PIOA, PIO_PA19,          ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 42
  { PIOA, PIO_PA20,          ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 43
  { PIOC, PIO_PC19,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 44
  { PIOC, PIO_PC18,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 45

  // 46
  { PIOC, PIO_PC17,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 46
  { PIOC, PIO_PC16,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 47
  { PIOC, PIO_PC15,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 48
  { PIOC, PIO_PC14,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 49

  // 50
  { PIOC, PIO_PC13,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 50
  { PIOC, PIO_PC12,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 51
  { PIOB, PIO_PB21X1_AD14,   ID_PIOB, PIO_INPUT, PIO_DEFAULT, PIN_ATTR_ANALOG,                      ADC2,   ADC14, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 52
  { PIOB, PIO_PB14,          ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN 53

  // 54 .. 65 - Analog pins
  // ----------------------
  { PIOA, PIO_PA16X1_AD7,    ID_PIOA, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC3,   ADC7,   NOT_ON_PWM,  NOT_ON_TIMER }, // AD0
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },

  // 58
  { PIOA, PIO_PA6X1_AD3,     ID_PIOA, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC4,   ADC3,   NOT_ON_PWM,  TC0_CHB2     }, // AD4
  { PIOA, PIO_PA4X1_AD2,     ID_PIOA, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC5,   ADC2,   NOT_ON_PWM,  NOT_ON_TIMER }, // AD5
  { PIOA, PIO_PA3X1_AD1,     ID_PIOA, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC6,   ADC1,   NOT_ON_PWM,  TC0_CHB1     }, // AD6
  { PIOA, PIO_PA2X1_AD0,     ID_PIOA, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC7,   ADC0,   NOT_ON_PWM,  TC0_CHA1     }, // AD7
  // 62
  { PIOB, PIO_PB17X1_AD10,   ID_PIOB, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC8,   ADC10,  NOT_ON_PWM,  NOT_ON_TIMER }, // AD8
  { PIOB, PIO_PB18X1_AD11,   ID_PIOB, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC9,   ADC11,  NOT_ON_PWM,  NOT_ON_TIMER }, // AD9
  { PIOB, PIO_PB19X1_AD12,   ID_PIOB, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC10,  ADC12,  NOT_ON_PWM,  NOT_ON_TIMER }, // AD10
  { PIOB, PIO_PB20X1_AD13,   ID_PIOB, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC11,  ADC13,  NOT_ON_PWM,  NOT_ON_TIMER }, // AD11  
  // 66/67 - DAC0/DAC1
  { PIOB, PIO_PB15X1_DAC0,   ID_PIOB, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC12,  DA0,    NOT_ON_PWM,  NOT_ON_TIMER }, // DAC0
  { PIOB, PIO_PB16X1_DAC1,   ID_PIOB, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC13,  DA1,    NOT_ON_PWM,  NOT_ON_TIMER }, // DAC1

  // 68/69 - CANRX0/CANTX0
  { PIOA, PIO_PA1A_CANRX0,   ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  ADC14,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // CANRX
  { PIOA, PIO_PA0A_CANTX0,   ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  ADC15,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // CANTX

  // 70/71 - TWI0
  { PIOA, PIO_PA17A_TWD0,    ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TWD0 - SDA1
  { PIOA, PIO_PA18A_TWCK0,   ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TWCK0 - SCL1

  // 72/73 - LEDs (compatibility arduino due)
  { PIOC, PIO_PC30,          ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // LED AMBER RXL
  { PIOA, PIO_PA21,          ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // LED AMBER TXL

  // 74/75/76 - SPI (full compatible arduino due)
  { PIOA, PIO_PA25A_SPI0_MISO,ID_PIOA,PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // MISO
  { PIOA, PIO_PA26A_SPI0_MOSI,ID_PIOA,PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // MOSI
  { PIOA, PIO_PA27A_SPI0_SPCK,ID_PIOA,PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPCK
  // 77 - SPI CS0
  { PIOA, PIO_PA28A_SPI0_NPCS0,ID_PIOA,PIO_PERIPH_A,PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // NPCS0
  // 78 - SPI CS3 (unconnected)
  { PIOB, PIO_PB23B_SPI0_NPCS3,ID_PIOB,PIO_PERIPH_B,PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // NPCS3

  // 79 - 80
  { PIOB, PIO_PB19,   ID_PIOB, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC10,  ADC12,    NOT_ON_PWM,  NOT_ON_TIMER }, // ANA3
  { PIOB, PIO_PB20,   ID_PIOB, PIO_INPUT,    PIO_DEFAULT, PIN_ATTR_ANALOG,                   ADC11,  ADC13,    NOT_ON_PWM,  NOT_ON_TIMER }, // ANA4
  // 81 - UART (USB Serial PROG) all pins
  { PIOA, PIO_PA8A_URXD|PIO_PA9A_UTXD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 82 - USART0 (Serial1 direct connector) all pins
  { PIOA, PIO_PA11A_TXD0|PIO_PA10A_RXD0, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 83 - USART1 (Serial2 direct connector) all pins
  { PIOA, PIO_PA13A_TXD1|PIO_PA12A_RXD1, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 84 
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 85 - USB
  { PIOB, PIO_PB11A_UOTGID|PIO_PB10A_UOTGVBOF, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ID - VBOF
  // 86 
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 87 
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 88/89 - CANRX1/CANTX1 (same physical pin for 66 possible conflict)/53(no conflict))
  { PIOB, PIO_PB15A_CANRX1, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // CANRX1
  { PIOB, PIO_PB14A_CANTX1, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // CANTX1
  // 90 .. 91 - "All CAN pins" masks
  // 90 - CAN0 all pins
  { PIOA, PIO_PA1A_CANRX0|PIO_PA0A_CANTX0, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  // 91 - CAN1 all pins
  { PIOB, PIO_PB15A_CANRX1|PIO_PB14A_CANTX1, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
 
   // 92 .. 99 placeholders, future-proofing.
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 100 .. 108 extra SAM3X8E pins, not wired on Due
  { PIOA, PIO_PC10, ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }, // PIN 100
  { PIOC, PIO_PC27, ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }, // PIN 101
  { PIOA, PIO_PA0, ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }, // PIN 102
  { PIOA, PIO_PA1, ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }, // PIN 103
  { PIOC, PIO_PC11, ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }, // PIN 104
  { PIOC, PIO_PC8B_PWML3, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM), NO_ADC, NO_ADC, PWM_CH3, NOT_ON_TIMER }, // PWM 105
  { PIOC, PIO_PC2B_PWML0, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM), NO_ADC, NO_ADC, PWM_CH0, NOT_ON_TIMER }, // PWM 106
  { PIOC, PIO_PC6B_PWML2, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM), NO_ADC, NO_ADC, PWM_CH2, NOT_ON_TIMER }, //PWM 107
  { PIOC, PIO_PC20, ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }, // 108
  // 109 .. 114 SD/eMMC on WK Agent Board
  { PIOA, PIO_PA20A_MCCDA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }, // PIN_HSMCI_MCCDA_GPIO 109
  { PIOA, PIO_PA19A_MCCK, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }, // PIN_HSMCI_MCCK_GPIO 110
  { PIOA, PIO_PA21A_MCDA0, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }, // PIN_HSMCI_MCDA0_GPIO 111
  { PIOA, PIO_PA22A_MCDA1, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }, // PIN_HSMCI_MCDA1_GPIO 112
  { PIOA, PIO_PA23A_MCDA2, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }, // PIN_HSMCI_MCDA2_GPIO 113
  { PIOA, PIO_PA24A_MCDA3, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }, // PIN_HSMCI_MCDA3_GPIO 114	
	// 115 .. 125 ETHERNET MAC on WK Agent Board
  { PIOB, PIO_PB0A_ETXCK,   ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ETXCK*
  { PIOB, PIO_PB1A_ETXEN,   ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ETXEN*
  { PIOB, PIO_PB2A_ETX0,   ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ETX0*
  { PIOB, PIO_PB3A_ETX1,   ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ETX1*
  { PIOB, PIO_PB4A_ECRSDV,   ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ECRSDV*
  { PIOB, PIO_PB5A_ERX0,   ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ERX0*
  { PIOB, PIO_PB6A_ERX1,   ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ERX1*
  { PIOB, PIO_PB7A_ERXER,   ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ERXER*
  { PIOB, PIO_PB8A_EMDC,   ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // EMDC*
  { PIOB, PIO_PB9A_EMDIO,   ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // EMDIO*
  { PIOA, PIO_PA5A_TIOA2,   ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC,  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // MDINTR* 
  // 126 .. 127
  { PIOB, PIO_PB21X1_AD14,   ID_PIOB, PIO_INPUT, PIO_DEFAULT, PIN_ATTR_ANALOG, ADC0,  ADC14,  NOT_ON_PWM,  NOT_ON_TIMER },  // ANA10 PIN 126 - PB21 with enabled ADC  
  { PIOB, PIO_PB11, ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }, // PIN 127
  { PIOB, PIO_PB10, ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },  // PIN 128
  // END
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }
};

#ifdef __cplusplus
}
#endif

/*
 * UART objects
 */
RingBuffer rx_buffer1;
RingBuffer tx_buffer1;

UARTClass Serial(UART, UART_IRQn, ID_UART, &rx_buffer1, &tx_buffer1);
void serialEvent() __attribute__((weak));
void serialEvent() { }

// IT handlers
void UART_Handler(void)
{
  Serial.IrqHandler();
}

// ----------------------------------------------------------------------------
/*
 * USART objects
 */
RingBuffer rx_buffer2;
RingBuffer rx_buffer3;
RingBuffer rx_buffer4;
RingBuffer tx_buffer2;
RingBuffer tx_buffer3;
RingBuffer tx_buffer4;

USARTClass Serial1(USART0, USART0_IRQn, ID_USART0, &rx_buffer2, &tx_buffer2);
void serialEvent1() __attribute__((weak));
void serialEvent1() { }
USARTClass Serial2(USART1, USART1_IRQn, ID_USART1, &rx_buffer3, &tx_buffer3);
void serialEvent2() __attribute__((weak));
void serialEvent2() { }
USARTClass Serial3(USART3, USART3_IRQn, ID_USART3, &rx_buffer4, &tx_buffer4);
void serialEvent3() __attribute__((weak));
void serialEvent3() { }

// IT handlers
void USART0_Handler(void)
{
  Serial1.IrqHandler();
}

void USART1_Handler(void)
{
  Serial2.IrqHandler();
}

void USART3_Handler(void)
{
  Serial3.IrqHandler();
}

// ----------------------------------------------------------------------------

void serialEventRun(void)
{
  if (Serial.available()) serialEvent();
  if (Serial1.available()) serialEvent1();
  if (Serial2.available()) serialEvent2();
  if (Serial3.available()) serialEvent3();
}

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void __libc_init_array(void);

void init( void )
{
  SystemInit();

  // Set Systick to 1ms interval, common to all SAM3 variants
  if (SysTick_Config(SystemCoreClock / 1000))
  {
    // Capture error
    while (true);
  }

  // Disable watchdog
  WDT_Disable(WDT);

  // Initialize C library
  __libc_init_array();

  // Disable pull-up on every pin (only arduino std pins)
  for (unsigned i = 0; i < PINS_COUNT; i++) digitalWrite(i, LOW);

  // Enable parallel access on PIO output data registers
  PIOA->PIO_OWER = 0xFFFFFFFF;
  PIOB->PIO_OWER = 0xFFFFFFFF;
  PIOC->PIO_OWER = 0xFFFFFFFF;
  PIOD->PIO_OWER = 0xFFFFFFFF;
  
  // Initialize Serial port U(S)ART pins
  PIO_Configure(g_APinDescription[PINS_UART].pPort,g_APinDescription[PINS_UART].ulPinType,g_APinDescription[PINS_UART].ulPin,g_APinDescription[PINS_UART].ulPinConfiguration);
  digitalWrite(0, HIGH); // Enable pullup for RX0
  PIO_Configure(g_APinDescription[PINS_USART0].pPort,g_APinDescription[PINS_USART0].ulPinType,g_APinDescription[PINS_USART0].ulPin,g_APinDescription[PINS_USART0].ulPinConfiguration); // Serial1 of Agent
  PIO_Configure(g_APinDescription[PINS_USART1].pPort,g_APinDescription[PINS_USART1].ulPinType,g_APinDescription[PINS_USART1].ulPin,g_APinDescription[PINS_USART1].ulPinConfiguration); // Serial2 of Agent
  PIO_Configure(g_APinDescription[PINS_USART3].pPort,g_APinDescription[PINS_USART3].ulPinType,g_APinDescription[PINS_USART3].ulPin,g_APinDescription[PINS_USART3].ulPinConfiguration);

  // Initialize USB pins
  PIO_Configure(g_APinDescription[PINS_USB].pPort,g_APinDescription[PINS_USB].ulPinType,g_APinDescription[PINS_USB].ulPin,g_APinDescription[PINS_USB].ulPinConfiguration);

  // OE
  PIO_Configure(g_APinDescription[23].pPort,g_APinDescription[23].ulPinType,g_APinDescription[23].ulPin,g_APinDescription[23].ulPinConfiguration);
  
  // Initialize EMAC pins
  PIO_Configure(g_APinDescription[PIN_EMAC_EREFCK].pPort,g_APinDescription[PIN_EMAC_EREFCK].ulPinType,g_APinDescription[PIN_EMAC_EREFCK].ulPin,g_APinDescription[PIN_EMAC_EREFCK].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_EMAC_ETXEN].pPort,g_APinDescription[PIN_EMAC_ETXEN].ulPinType,g_APinDescription[PIN_EMAC_ETXEN].ulPin,g_APinDescription[PIN_EMAC_ETXEN].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_EMAC_ETX0].pPort,g_APinDescription[PIN_EMAC_ETX0].ulPinType,g_APinDescription[PIN_EMAC_ETX0].ulPin,g_APinDescription[PIN_EMAC_ETX0].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_EMAC_ETX1].pPort,g_APinDescription[PIN_EMAC_ETX1].ulPinType,g_APinDescription[PIN_EMAC_ETX1].ulPin,g_APinDescription[PIN_EMAC_ETX1].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_EMAC_ECRSDV].pPort,g_APinDescription[PIN_EMAC_ECRSDV].ulPinType,g_APinDescription[PIN_EMAC_ECRSDV].ulPin,g_APinDescription[PIN_EMAC_ECRSDV].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_EMAC_ERX0].pPort,g_APinDescription[PIN_EMAC_ERX0].ulPinType,g_APinDescription[PIN_EMAC_ERX0].ulPin,g_APinDescription[PIN_EMAC_ERX0].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_EMAC_ERX1].pPort,g_APinDescription[PIN_EMAC_ERX1].ulPinType,g_APinDescription[PIN_EMAC_ERX1].ulPin,g_APinDescription[PIN_EMAC_ERX1].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_EMAC_ERXER].pPort,g_APinDescription[PIN_EMAC_ERXER].ulPinType,g_APinDescription[PIN_EMAC_ERXER].ulPin,g_APinDescription[PIN_EMAC_ERXER].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_EMAC_EMDC].pPort,g_APinDescription[PIN_EMAC_EMDC].ulPinType,g_APinDescription[PIN_EMAC_EMDC].ulPin,g_APinDescription[PIN_EMAC_EMDC].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_EMAC_EMDIO].pPort,g_APinDescription[PIN_EMAC_EMDIO].ulPinType,g_APinDescription[PIN_EMAC_EMDIO].ulPin,g_APinDescription[PIN_EMAC_EMDIO].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_TC0_TIOA2].pPort,g_APinDescription[PIN_TC0_TIOA2].ulPinType,g_APinDescription[PIN_TC0_TIOA2].ulPin,g_APinDescription[PIN_TC0_TIOA2].ulPinConfiguration);
	
  // Initialize SD/MMC 	
  PIO_Configure(g_APinDescription[PIN_PD0B_MCDA4].pPort,g_APinDescription[PIN_PD0B_MCDA4].ulPinType,g_APinDescription[PIN_PD0B_MCDA4].ulPin,g_APinDescription[PIN_PD0B_MCDA4].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_PD1B_MCDA5].pPort,g_APinDescription[PIN_PD1B_MCDA5].ulPinType,g_APinDescription[PIN_PD1B_MCDA5].ulPin,g_APinDescription[PIN_PD1B_MCDA5].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_PD2B_MCDA6].pPort,g_APinDescription[PIN_PD2B_MCDA6].ulPinType,g_APinDescription[PIN_PD2B_MCDA6].ulPin,g_APinDescription[PIN_PD2B_MCDA6].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_PD3B_MCDA7].pPort,g_APinDescription[PIN_PD3B_MCDA7].ulPinType,g_APinDescription[PIN_PD3B_MCDA7].ulPin,g_APinDescription[PIN_PD3B_MCDA7].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_PA21A_MCDA0].pPort,g_APinDescription[PIN_PA21A_MCDA0].ulPinType,g_APinDescription[PIN_PA21A_MCDA0].ulPin,g_APinDescription[PIN_PA21A_MCDA0].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_PA22A_MCDA1].pPort,g_APinDescription[PIN_PA22A_MCDA1].ulPinType,g_APinDescription[PIN_PA22A_MCDA1].ulPin,g_APinDescription[PIN_PA22A_MCDA1].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_PA23A_MCDA2].pPort,g_APinDescription[PIN_PA23A_MCDA2].ulPinType,g_APinDescription[PIN_PA23A_MCDA2].ulPin,g_APinDescription[PIN_PA23A_MCDA2].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_PA24A_MCDA3].pPort,g_APinDescription[PIN_PA24A_MCDA3].ulPinType,g_APinDescription[PIN_PA24A_MCDA3].ulPin,g_APinDescription[PIN_PA24A_MCDA3].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_PA20A_MCCDA].pPort,g_APinDescription[PIN_PA20A_MCCDA].ulPinType,g_APinDescription[PIN_PA20A_MCCDA].ulPin,g_APinDescription[PIN_PA20A_MCCDA].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PIN_PA19A_MCCK].pPort,g_APinDescription[PIN_PA19A_MCCK].ulPinType,g_APinDescription[PIN_PA19A_MCCK].ulPin,g_APinDescription[PIN_PA19A_MCCK].ulPinConfiguration);
	
  // Initialize CAN pins 
  PIO_Configure(g_APinDescription[PINS_CAN0].pPort,g_APinDescription[PINS_CAN0].ulPinType,g_APinDescription[PINS_CAN0].ulPin,g_APinDescription[PINS_CAN0].ulPinConfiguration);
  PIO_Configure(g_APinDescription[PINS_CAN1].pPort,g_APinDescription[PINS_CAN1].ulPinType,g_APinDescription[PINS_CAN1].ulPin,g_APinDescription[PINS_CAN1].ulPinConfiguration);
		
  // Initialize Analog Controller
  pmc_enable_periph_clk(ID_ADC);
  adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST);
  adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
  adc_configure_trigger(ADC, ADC_TRIG_SW, 0); // Disable hardware trigger.
  adc_disable_interrupt(ADC, 0xFFFFFFFF); // Disable all ADC interrupts.
  adc_disable_all_channel(ADC);

  // Initialize analogOutput module
  analogOutputInit();
}

#ifdef __cplusplus
}
#endif
