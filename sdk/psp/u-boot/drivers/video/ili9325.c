/*
 * ILI9320 lcd driver
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
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
#include <common.h>
#include <spi.h>

#include "ili9325.h"

#if 1
#define LCD_DBG(fmt, arg...) \
	printf("%s (line:%d) :" fmt "\n", __func__, __LINE__, ## arg)
#else
#define LCD_DBG(fmt, arg...) do {} while (0)
#endif

/*
 * Each transfer is performed as:
 * 1. chip-select active
 * 2. send 8-bit start code
 * 3. send 16-bit data
 * 4. chip-select inactive
 */
static int send_word(struct ili9325 *data, u8 rs, u16 word)
{
	/*
	 * The start byte looks like (binary):
	 * 01110<ID><RS><R/W>
	 * RS is 0 for index or 1 for data, and R/W is 0 for write.
	 */
	u32 buf8 = 0x70 | data->id | (rs & 2);
	u32 buf16 = cpu_to_le16(word);
	int err;

	err = spi_xfer(data->spi, 8, &buf8, NULL, SPI_XFER_BEGIN);
	if (err)
		return err;

	return spi_xfer(data->spi, 16, &buf16, NULL, SPI_XFER_END);
}

static int ili9325_spi_write(struct spi_slave *spi, u16 addr, u16 val)
{
	int ret = 0;
	
	spi_claim_bus(spi);
	
	/*
	 * The start byte looks like (binary):
	 * 01110<ID><RS><R/W>
	 * RS is 0 for index or 1 for data, and R/W is 0 for write.
	 */
	u32 buf8 = 0x70 | data->id | (rs & 2);
	u32 buf16 = cpu_to_le16(word);
	int err;

	err = spi_xfer(data->spi, 8, &buf8, NULL, SPI_XFER_BEGIN);
	if (err)
		return err;

	return spi_xfer(data->spi, 16, &buf16, NULL, SPI_XFER_END);
	
	ret = send_word(data, ILI9325_SPI_INDEX, idx);
	if (ret) {
		LCD_DBG("Error with the spi index transmit.\n");
		return ret;
	}
	
	ret = send_word(data, ILI9325_SPI_DATA, param);
	if (ret) {
		LCD_DBG("Error with the spi param transmit.\n");
		return ret;
	}
	
	spi_release_bus(data->spi);
	
	return 0;
}

static int ili9325_spi_read(struct spi_slave *spi, u16 addr, u16 val)
{
	int ret = 0;
	
	spi_claim_bus(spi);
	
	/*
	 * The start byte looks like (binary):
	 * 01110<ID><RS><R/W>
	 * RS is 0 for index or 1 for data, and R/W is 0 for write.
	 */
	u32 buf8 = 0x70 | data->id | (rs & 2);
	u32 buf16 = cpu_to_le16(word);
	int err;

	err = spi_xfer(data->spi, 8, &buf8, NULL, SPI_XFER_BEGIN);
	if (err)
		return err;

	return spi_xfer(data->spi, 16, &buf16, NULL, SPI_XFER_END);
	
	ret = send_word(data, ILI9325_SPI_INDEX, idx);
	if (ret) {
		LCD_DBG("Error with the spi index transmit.\n");
		return ret;
	}
	
	ret = send_word(data, ILI9325_SPI_DATA, param);
	if (ret) {
		LCD_DBG("Error with the spi param transmit.\n");
		return ret;
	}
	
	spi_release_bus(data->spi);
	
	return 0;
}

int ili9325_lcd_init(void)
{
	struct spi_slave *spi;
	
	if (data->id != 0 && data->id != 4) {
		printf("ili9325d: invalid ID %u\n", data->id);
		return 1;
	}
	
	data->spi = spi_setup_slave(data->bus, data->cs, 1000000, SPI_MODE_3);
	if (!data->spi) {
		printf("Invalid device giving up.\n");
		return 1;
	}
	
	return 0;
}
