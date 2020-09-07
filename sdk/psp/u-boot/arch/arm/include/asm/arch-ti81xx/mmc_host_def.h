/*
 * (C) Copyright 2008
 * Texas Instruments, <www.ti.com>
 * Syed Mohammed Khasim <khasim@ti.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation's version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef MMC_HOST_DEF_H
#define MMC_HOST_DEF_H

#include <asm/omap_mmc.h>

/*
 * OMAP HSMMC register definitions
 */
#if defined(CONFIG_TI816X)
# define OMAP_HSMMC_BASE		0x48060100
#elif defined(CONFIG_TI814X)
# define OMAP_HSMMC_BASE		0x481D8100
#endif

#if defined(CONFIG_TI814X)
#undef MMC_CLOCK_REFERENCE
#define MMC_CLOCK_REFERENCE	192 /* MHz */
#elif defined(CONFIG_TI816X)
#undef MMC_CLOCK_REFERENCE
#define MMC_CLOCK_REFERENCE	48 /* MHz */
#endif

#endif /* MMC_HOST_DEF_H */
