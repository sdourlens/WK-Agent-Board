/**
 * \file
 *
 * \brief SAM3X-EK Board Definition.
 *
 * Copyright (c) 2011 - 2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef _SAM3X_EK_H_
#define _SAM3X_EK_H_

#include "compiler.h"
//DCG#include "system_sam3x.h"
//DCG#include "exceptions.h"

/*#define BOARD_REV_A */
#define BOARD_REV_B

/* ------------------------------------------------------------------------ */

/**
 *  \page sam3x_ek_opfreq "SAM3X-EK - Operating frequencies"
 *  This page lists several definition related to the board operating frequency
 *
 *  \section Definitions
 *  - \ref BOARD_FREQ_*
 *  - \ref BOARD_MCK
 */

/*! Board oscillator settings */
#define BOARD_FREQ_SLCK_XTAL            (32768U)
#define BOARD_FREQ_SLCK_BYPASS          (32768U)
#define BOARD_FREQ_MAINCK_XTAL          (12000000U)
#define BOARD_FREQ_MAINCK_BYPASS        (12000000U)

/*! Master clock frequency */
#define BOARD_MCK                       CHIP_FREQ_CPU_MAX

/** board main clock xtal statup time */
#define BOARD_OSC_STARTUP_US   15625

/* ------------------------------------------------------------------------ */

/**
 * \page sam3x_ek_board_info "SAM3X-EK - Board informations"
 * This page lists several definition related to the board description.
 *
 * \section Definitions
 * - \ref BOARD_NAME
 */

/*! Name of the board */
//DCG#define BOARD_NAME "SAM3X-EK"
/*! Board definition */
//DCG#define sam3xek
/*! Family definition (already defined) */
#define sam3x
/*! Core definition */
#define cortexm3

/* ------------------------------------------------------------------------ */

/**
 * \page sam3x_ek_piodef "SAM3X-EK - PIO definitions"
 * This pages lists all the pio definitions. The constants
 * are named using the following convention: PIN_* for a constant which defines
 * a single Pin instance (but may include several PIOs sharing the same
 * controller), and PINS_* for a list of Pin instances.
 *
 */


/**
 * \file
 * HSMCI
 * - \ref PINS_HSMCI
 *
 */

/* ------------------------------------------------------------------------ */
/* HSMCI                                                                      */
/* ------------------------------------------------------------------------ */
/*! HSMCI pins definition. */
/*! Number of slot connected on HSMCI interface */
#define SD_MMC_HSMCI_MEM_CNT        1
#define SD_MMC_HSMCI_SLOT_0_SIZE    8
#define PINS_HSMCI\
	{ PIO_PA20A_MCCDA | PIO_PA19A_MCCK | PIO_PA21A_MCDA0 | PIO_PA22A_MCDA1\
	| PIO_PA23A_MCDA2 | PIO_PA24A_MCDA3,\
	PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP },\
	{ PIO_PD0B_MCDA4 | PIO_PD1B_MCDA5 | PIO_PD2B_MCDA6 | PIO_PD3B_MCDA7,\
	PIOD, ID_PIOD, PIO_PERIPH_B, PIO_PULLUP},\
	{ PIO_PE20B_MCCDB | PIO_PE22B_MCDB0 | PIO_PE24B_MCDB1 | PIO_PE26B_MCDB2\
	| PIO_PE27B_MCDB3,\
	PIOE, ID_PIOE, PIO_PERIPH_B, PIO_PULLUP }


/** HSMCI MCCDA pin definition. */
#define PIN_HSMCI_MCCDA_GPIO            (PIO_PA20_IDX)
#define PIN_HSMCI_MCCDA_FLAGS           (PIO_PERIPH_A | PIO_DEFAULT)

const int MCCDA_Pin = 109;	//Arduino pin number definition

/** HSMCI MCCK pin definition. */
#define PIN_HSMCI_MCCK_GPIO             (PIO_PA19_IDX)
#define PIN_HSMCI_MCCK_FLAGS            (PIO_PERIPH_A | PIO_DEFAULT)

const int MCCK_Pin = 110;	//Arduino pin number definition

/** HSMCI MCDA0 pin definition. */
#define PIN_HSMCI_MCDA0_GPIO            (PIO_PA21_IDX)
#define PIN_HSMCI_MCDA0_FLAGS           (PIO_PERIPH_A | PIO_DEFAULT)

const int MCDA0_Pin = 111;	//Arduino pin number definition

/** HSMCI MCDA1 pin definition. */
#define PIN_HSMCI_MCDA1_GPIO            (PIO_PA22_IDX)
#define PIN_HSMCI_MCDA1_FLAGS           (PIO_PERIPH_A | PIO_DEFAULT)

const int MCDA1_Pin = 112;	//Arduino pin number definition

/** HSMCI MCDA2 pin definition. */
#define PIN_HSMCI_MCDA2_GPIO            (PIO_PA23_IDX)
#define PIN_HSMCI_MCDA2_FLAGS           (PIO_PERIPH_A | PIO_DEFAULT)

const int MCDA2_Pin = 113;	//Arduino pin number definition

/** HSMCI MCDA3 pin definition. */
#define PIN_HSMCI_MCDA3_GPIO            (PIO_PA24_IDX)
#define PIN_HSMCI_MCDA3_FLAGS           (PIO_PERIPH_A | PIO_DEFAULT)

const int MCDA3_Pin = 114;	//Arduino pin number definition

/** SD/MMC card detect pin definition. 
#define PIN_HSMCI_CD {PIO_PC13, PIOC, ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define SD_MMC_0_CD_GPIO            (PIO_PC13_IDX)
#define SD_MMC_0_CD_PIO_ID          ID_PIOC
#define SD_MMC_0_CD_FLAGS           (PIO_INPUT | PIO_PULLUP)
#define SD_MMC_0_CD_DETECT_VALUE    0

const int SD_CD_Pin = 50;	//Arduino pin number definition
*/

extern void board_init(void);

#endif  /* _SAM3X_EK_H_ */
