/*
 * Copyright (C) 2009, Texas Instruments, Incorporated
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef _UBX_DEFS_H_
#define _UBX_DEFS_H_

#include <asm/arch/cpu.h>
#include <asm/arch/common_ud_defs.h>

/* defines gpio map */
#define B_REV_P1					GPIO_N(0, 1)
#define B_REV_P2					GPIO_N(0, 2)
#define B_REV_P3					GPIO_N(0, 3)
#define B_REV_P4					GPIO_N(0, 4)
#define REC_SWITCH					GPIO_N(0, 6)
/* #define BUZZER_EN					GPIO_N(0, 13) removed rev0.2 */
#define SERDES_4_LED				GPIO_N(0, 14)
#define SERDES_4_LED_FB				GPIO_N(0, 15)
#define ACCEL_IRQ					GPIO_N(0, 16)
#define SERDES_3_PWR				GPIO_N(0, 17)
#define SERDES_4_PWR				GPIO_N(0, 18)
#define USER_2_LED					GPIO_N(0, 19)
#define SERDES_3_LED_FB				GPIO_N(0, 22)
#define SERDES_1_LOCKN				GPIO_N(0, 23)
#define SERDES_2_LED				GPIO_N(0, 24) /* active_low */
#define SERDES_1_LED				GPIO_N(0, 25) /* active_low */
#define SERDES_2_PWR				GPIO_N(0, 28) /* active_high */

/* BANK1 */
#define MMC_CD						GPIO_N(1, 7)  /* input */
#define ETH_STATUS_LED				GPIO_N(1, 8)  /* active_high */
#define EXT_DETECT					GPIO_N(1, 13) /* input */
#define USB_GPS_DETECT				GPIO_N(1, 14) /* input, for rev0.2 */
#define NAND_WP						GPIO_N(1, 25) /* active high */
#define RSTOUT_EN					GPIO_N(1, 27) /* active_low */

/* BANK2 */
#define S_GPS_PWR					GPIO_N(2, 4)  /* output, for rev0.2 */
#define REC_LED						GPIO_N(2, 5)  /* active_low */
#define BACKUP_LED					GPIO_N(2, 6)  /* active_high */
#define MCU_BSL_RTS					GPIO_N(2, 21)
#define MICOM_HOLD					GPIO_N(2, 26)
#define SERDES_2_LOCKN				GPIO_N(2, 28)
#define SERDES_2_LED_FB				GPIO_N(2, 30)

/* BANK3 */
#define SERDES_3_PDN				GPIO_N(3, 7) /* active_low */
#define SERDES_4_PDN				GPIO_N(3, 8) /* active_low */
#define MCU_BSL_DTR					GPIO_N(3, 10)
#define SERDES_4_LOCKN				GPIO_N(3, 11)
#define GPIO_I2C_SCL				GPIO_N(3, 12) /* for rev0.5 */
#define GPIO_I2C_SDA				GPIO_N(3, 13) /* for rev0.5 */
#define SERDES_1_PWR				GPIO_N(3, 14) /* active_high */
#define VIN_B_D8					GPIO_N(3, 17)
#define SERDES_3_LOCKN				GPIO_N(3, 18)
#define ETH_IRQ						GPIO_N(3, 19)
#define SERDES_2_PDN				GPIO_N(3, 21) /* active_low */
#define SERDES_1_PDN				GPIO_N(3, 22) /* active_low */

#endif /* _UBX_DEFS_H_*/
