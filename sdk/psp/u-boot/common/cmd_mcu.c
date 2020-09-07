/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    cmd_mcu.c
 * @brief	write binary to MSP430G2333
 * @section MODIFY history
 *     - 2016.06.01 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <common.h>
#include <command.h>
#include <ns16550.h>
#include <asm/arch/cpu.h>
#include <asm/gpio.h>
#include <asm/arch/common_ud_defs.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define MODE_X_DIV 		16
#define MCU_BAUDRATE	9600

#define BSL_RTS			GPIO_N(2, 21)
#define BSL_DTR			GPIO_N(3, 10)
#define PWR_HOLD		GPIO_N(2, 26)

//# bsl command
#define BSL_TXPWORD		0x10
#define BSL_TXBLK		0x12
#define BSL_RXBLK		0x14
#define BSL_ERASE		0x16
#define BSL_MERAS		0x18
#define BSL_LOADPC		0x1A
#define BSL_ECHECK		0x1C
#define BSL_RXID		0x1E
#define BSL_SPEED		0x20
#define BSL_MEMOFFSET	0x21
#define BSL_SYNC		0x80

//# header define
#define CMD_FAILED 		0x70
#define DATA_FRAME 		0x80
#define DATA_ACK 		0x90
#define DATA_NAK 		0xA0

#define MAX_FRAME_SIZE	256
#define MAX_DATA_BYTES	250

//# binary load
#define BIN_LOAD_ADDR	0x81000000
#define CH_SEPAR		"\n"

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
char ubx_passwd[0x20] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xF1, 0x80, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xF2,
	0xFF, 0xFF, 0x80, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x02, 0xF0
};

static char blkout[MAX_DATA_BYTES];	//# Transmit buffer
static char rx_frame[MAX_FRAME_SIZE];
static char infoA[0x40];

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
//# control pin
#define set_pwr_hold(val)	gpio_direction_output(PWR_HOLD, val);
#define set_rst_pin(val)	gpio_direction_output(BSL_DTR, !val);
#define set_test_pin(val)	gpio_direction_output(BSL_RTS, val);

#define mdelay(ms)			udelay(ms*1000)

#undef eprintf
#define eprintf(x, ...)	printf(" [ERR] \033[41m%s:\033[0m " x, __func__, ##__VA_ARGS__);

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
static int str2int(char *data)
{
	int i, addr, num;

	addr = 0;
	for(i=3; i>=0; i--)
	{
		if(*data >= 'a' && *data <= 'f') {
			num = *data - 'a' + 10;
		} else if(*data >= 'A' && *data <= 'F') {
			num = *data - 'A' + 10;
		} else {
			num = *data - 48;
		}
		//printf("%d(%x) 0x%x, ", num, num, (1<<(i*4)));

		addr += num * (1<<(i*4));
		data += 1;
	}

	//printf("addr 0x%x\n", addr);
	return addr;
}

static char str2nchar(char *data)
{
	int i, nchar, num;

	nchar = 0;
	for(i=1; i>=0; i--)
	{
		if(*data >= 'a' && *data <= 'f') {
			num = *data - 'a' + 10;
		} else if(*data >= 'A' && *data <= 'F') {
			num = *data - 'A' + 10;
		} else {
			num = *data - 48;
		}

		nchar += num * (1<<(i*4));
		data += 1;
	}

	//printf("%02X ", nchar);
	return nchar;
}

static short calc_checksum(char data[], char length)
{
	short *i_data;
	short checksum=0;
	char i= 0;

	i_data = (short *)data;
	for (i=0; i<(length/2); i++) {
		checksum ^= *i_data++;	//# xor-ing
	}

	return (checksum ^ 0xffff); //# inverting
}

/*----------------------------------------------------------------------------
 bsl reset
 [desc] invoke 1: complete sequence
        invoke 0: only RST/NMI pin accessed
-----------------------------------------------------------------------------*/
static void bsl_reset(int invoke)
{
	set_rst_pin(1);			//# default high
	set_test_pin(0);		//# default low
	mdelay(250);

	if(invoke) {
		set_rst_pin(0);
		set_test_pin(0);	mdelay(10);
		set_test_pin(1);	mdelay(10);
		set_test_pin(0);	mdelay(10);
		set_test_pin(1);	mdelay(10);
		set_rst_pin(1);		mdelay(10);
		set_test_pin(0);
	}
	else {
		set_rst_pin(0);
		mdelay(10);
		set_rst_pin(1);
	}

	mdelay(250);
}

/*----------------------------------------------------------------------------
 bsl communication
-----------------------------------------------------------------------------*/
static void bsl_write(char *data, int size)
{
	int i;

	for(i=0; i<size; i++) {
		mdelay(1);
		NS16550_putc((NS16550_t)CONFIG_SYS_NS16550_COM3, *data++);
	}
}

static int bsl_read(char *data, int size)
{
	int i;

	for(i=0; i<size; i++) {
		*data++ = NS16550_getc((NS16550_t)CONFIG_SYS_NS16550_COM3);
	}

	return 0;
}

static int bsl_sync(void)
{
	char data;

	data = BSL_SYNC;
	bsl_write(&data, 1);
	//mdelay(100);
	bsl_read(&data, 1);
	//printf("[bsl_sync] data 0x%x\n", data);
	if(data == DATA_ACK) {
		return 0;
	}

	return -1;
}

static int bsl_rx_frame(void)
{
	int rxLength, rxLengthCRC;
	short checksum;

	rx_frame[2] = 0;	//# set lengths of received data to 0!
	bsl_read(&rx_frame[0], 4);

	if ((rx_frame[0] == DATA_FRAME) && (rx_frame[2] != 0))
	{
		rxLength = rx_frame[2];
		rxLengthCRC = rxLength + 2;

		bsl_read(&rx_frame[4], rxLengthCRC);

		checksum = calc_checksum(rx_frame, (rxLength+4));
		if(rx_frame[rxLength+4] == (char)(checksum) &&
				rx_frame[rxLength+5] == (char)(checksum >> 8)) {
			return 0;
		}
	}

	return -1;
}

static int com_tx_rx(char cmd, char data_out[], char length)
{
	char tx_frame[MAX_FRAME_SIZE];
	char rx_data=0;
	short checksum=0;

	/* Transmitting part ----------------------------------------*/
	tx_frame[0] = DATA_FRAME;
	tx_frame[1] = cmd;
	tx_frame[2] = length;
	tx_frame[3] = length;

	memcpy(&tx_frame[4], data_out, length);

	checksum = calc_checksum(tx_frame, (length+4));
	tx_frame[length+4] = (char)(checksum);
	tx_frame[length+5] = (char)(checksum >> 8);

	#if 0
	{	int i;
		for(i=0; i<(length+6); i++) {
			printf("0x%x ", tx_frame[i]);
		}
		printf("\n");
	}
	#endif

	//# Transmit data
	bsl_write(tx_frame, (length+6));

	/* Receiving part -------------------------------------------*/
	if (cmd==BSL_RXBLK) {
		return bsl_rx_frame();
	}
	else
	{
		bsl_read(&rx_data, 1);
		//printf("[com_tx_rx] cmd 0x%x rx_data 0x%02x (i=%d)\n", cmd, rx_data, i);
		if(rx_data==DATA_ACK) {
			return 0;
		} else if(rx_data==DATA_NAK || rx_data==CMD_FAILED) {
			return -1;
		}
	}

	return -1;
}

static int bsl_tx_rx(char cmd, unsigned int addr, int len, char *dout, char *din)
{
	char data_out[MAX_FRAME_SIZE], length=4;
	int ret;

	if(bsl_sync() < 0) {
		eprintf("device sync error!\n");
		return -1;
	}

	if ((len % 2) != 0)	{
		len += 1;
	}

	if((cmd == BSL_TXBLK) || (cmd == BSL_TXPWORD))    {
		length = len + 4;
    }

    //# Add necessary information data to frame
    data_out[0] = (char)( addr       & 0x00ff);
    data_out[1] = (char)((addr >> 8) & 0x00ff);
    data_out[2] = (char)( len        & 0x00ff);
    data_out[3] = (char)((len  >> 8) & 0x00ff);

    if (cmd==BSL_TXBLK || cmd==BSL_TXPWORD)    {
      	memcpy(&data_out[4], dout, len);		//# copy data out of dout into frame frame
    }

    //# send frame
    ret = com_tx_rx(cmd, data_out, length);

    if (cmd==BSL_RXBLK) {
      	memcpy(din, &rx_frame[4], rx_frame[2]);	//# copy received data out of frame buffer into din
    }

	return ret;
}

/*----------------------------------------------------------------------------
 bsl functions
-----------------------------------------------------------------------------*/
static int bsl_mass_erase(void)
{
	int ret;

	ret = bsl_tx_rx(BSL_MERAS, 0xFF00, 0xA506, NULL, NULL);
	if(ret < 0) {
		eprintf("mass erase fail!\n");
		return ret;
	}

	printf(" mass erase ok\n");
	return 0;
}

static int bsl_write_infoA(void)
{
	int ret;

	ret = bsl_tx_rx(BSL_TXBLK, 0x10C0, 0x40, infoA, NULL);
	if(ret < 0) {
		eprintf("write infoA fail!\n");
		return ret;
	}

	//printf(" write infoA ok\n");
	return 0;
}

static int bsl_read_infoA(void)
{
	int ret;

	ret = bsl_tx_rx(BSL_RXBLK, 0x10C0, 0x40, NULL, infoA);
	if(ret < 0) {
		eprintf("read infoA fail!\n");
		return ret;
	}

	#if 0
	{
		int i;
		for(i=0; i<0x40; i++) {
			printf("0x%x ", infoA[i]);
		}
		printf("\n");
	}
	#endif

	//printf(" read infoA ok\n");
	return 0;
}

static int bsl_read_verison(void)
{
	int ret;
	char ver[2];

	ret = bsl_tx_rx(BSL_RXBLK, 0xf000, 2, NULL, ver);
	if(ret < 0) {
		eprintf("read version fail!\n");
		return ret;
	}

	printf(" mcu version %02X.%02X\n", ver[1], ver[0]);
	return 0;
}

static int bsl_tx_passwd(void)
{
	int ret;

	//# first : try default password
	memset((void *)blkout, 0xff, 0x20);
	ret = bsl_tx_rx(BSL_TXPWORD, 0xffe0, 0x20, blkout, NULL);
	if(ret==0) {
		printf(" password 1st ok\n");
		return 0;
	}

	//# second : try password
	ret = bsl_tx_rx(BSL_TXPWORD, 0xffe0, 0x20, ubx_passwd, NULL);
	if(ret==0) {
		printf(" password 2nd ok\n");
		return 0;
	}

	printf("password fail!\n");
	return -1;
}

static int bsl_program_write(void)
{
	char *bin, *strdata;
	int dataframelen = 0;
	unsigned int currentAddr=0;
	int ret, i, first=1;

	bin = (char *)BIN_LOAD_ADDR;

	printf("Writing:");
	while(1)
	{
		//# read data
		if(first) {
			strdata = strtok(bin, CH_SEPAR);	//# first read
			first = 0;
		} else {
			strdata = strtok(NULL, CH_SEPAR);
		}

		//# pasing data
		if(strdata[0] == 'q') {		//# quit
			break;
		}
		if(strdata[0] == '!') {		//# version
			continue;
		}
		if(strdata[0] == '@') {
			//sscanf(&strdata[1], "%x\n", &currentAddr);
			currentAddr = str2int(&strdata[1]);
			continue;
		}

		dataframelen = 0;
		for(i=0; i<strlen(strdata)-3; i+=3, dataframelen++) {
			blkout[dataframelen] = str2nchar(&strdata[i]);
		}

		#if 0
		for(i=0; i<dataframelen; i++) {
			printf("%02X ", blkout[i]);
		}
		printf("\n");
		#endif

		ret = bsl_tx_rx(BSL_TXBLK, currentAddr, dataframelen, blkout, NULL);
		if(ret < 0) {
			eprintf("program write error\n");
			return 0;
		}
		printf("#");

		currentAddr += dataframelen;
	}

	printf("\n program writ ok\n");
	return 0;
}

/*----------------------------------------------------------------------------
 system functions
-----------------------------------------------------------------------------*/
static void uart_init(void)
{
	int clock_divisor;

	//# power hold
	//set_pwr_hold(1);		//# set in gpio_init()

	//# change pin mux to bsl
	*(volatile unsigned int *)(CTRL_BASE+0x0910) = 0x60080;		//# uart2_rxd -> gpio input
	*(volatile unsigned int *)(CTRL_BASE+0x090C) = 0x60080;		//# uart2_txd -> gpio input

	*(volatile unsigned int *)(CTRL_BASE+0x09C0) = 0xE0020;		//# uart2_rxd_bsl
	*(volatile unsigned int *)(CTRL_BASE+0x09C4) = 0xE0020;		//# uart2_txd_bsl

	//# init uart2 clock
	clock_divisor = (CONFIG_SYS_NS16550_CLK + (MCU_BAUDRATE * (MODE_X_DIV / 2))) / (MODE_X_DIV * MCU_BAUDRATE);
	NS16550_init((NS16550_t)CONFIG_SYS_NS16550_COM3, clock_divisor);
}

static void uart_exit(void)
{
	//# change pin mux to uart - maybe not need 'coz will restart
	*(volatile unsigned int *)(CTRL_BASE+0x09C0) = 0x60080;		//# uart2_rxd_bsl -> gpio input
	*(volatile unsigned int *)(CTRL_BASE+0x09C4) = 0x60080;		//# uart2_txd_bsl -> gpio input

	*(volatile unsigned int *)(CTRL_BASE+0x0910) = 0xE0002;		//# uart2_rxd
	*(volatile unsigned int *)(CTRL_BASE+0x090C) = 0xE0002;		//# uart2_txd

	//# power free
	//set_pwr_hold(0);
}

static int chk_binary(void)
{
	char *bin;

	bin = (char *)BIN_LOAD_ADDR;
	if(bin[0] != '!' && bin[0] != '@') {
		eprintf("do not exist binary!\n");
		printf("- load binary to 0x%x\n", BIN_LOAD_ADDR);
		return -1;
	}

	return 0;
}

static int chk_hw_ver(void)
{
	int ver;

	/* cwx
	   0x0 -> ver0.1
	   0x1 -> ver0.2
	   0x2 -> ver0.3 : mcu support
	   ...
	*/

	ver = ((*(volatile unsigned int *)(GPIO0_BASE + GPIO_DATAIN))>>1) & 0xF;
	printf("hw ver 0.%d\n", (ver+1));
	if(ver < 0x2) {
		eprintf("mcu supports from ver0.3\n");
		return -1;
	}

	return 0;
}

/*----------------------------------------------------------------------------
 mcu functions
-----------------------------------------------------------------------------*/
static int mcu_reset(void)
{
	//set_pwr_hold(1);		//# set in gpio_init()

	bsl_reset(1);

	*(volatile unsigned int *)(CTRL_BASE+0x0910) = 0x60080;
	*(volatile unsigned int *)(CTRL_BASE+0x090C) = 0x60080;

	printf("mcu_reset ...done\n");

	return 0;
}

static int mcu_update(void)
{
	int ret;

	//# check mcu binary
	if(chk_binary() < 0) {
		return -1;
	}
	printf(" start mcu update\n");

	uart_init();

	bsl_reset(1);

	//# send password
	ret = bsl_tx_passwd();
	if(ret < 0) {
		return -1;
	}

	//# read infoA
	ret = bsl_read_infoA();
	if(ret < 0) {
		return -1;
	}

	//# mass erase
	ret = bsl_mass_erase();
	if(ret < 0) {
		return -1;
	}

	//# write infoA
	ret = bsl_write_infoA();
	if(ret < 0) {
		return -1;
	}

	//# program write
	ret = bsl_program_write();
	if(ret < 0) {
		return -1;
	}

	bsl_reset(0);

	uart_exit();

	return 0;
}

static int mcu_version(void)
{
	int ret;

	uart_init();

	bsl_reset(1);

	//# send password
	ret = bsl_tx_passwd();
	if(ret < 0) {
		return -1;
	}

	//# read version
	ret = bsl_read_verison();
	if(ret < 0) {
		return -1;
	}

	bsl_reset(0);

	uart_exit();

	return 0;
}

int do_mcu (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *cmd;

	if (argc < 2) {
		goto usage;
	}
	cmd = argv[1];

	if (strcmp(cmd, "ver") == 0) {
		mcu_version();
		return 0;
	}

	if (strcmp(cmd, "update") == 0) {
		mcu_update();
		return 0;
	}

	if (strcmp(cmd, "reset") == 0) {	//# for bsl program of PC
		mcu_reset();
		return 0;
	}

usage:
	cmd_usage(cmdtp);

	return 1;
}

/***************************************************/

U_BOOT_CMD(
	mcu,	2,	1,	do_mcu,
	"mcu util",
	"ver - view version\n"
	"mcu update - update binary of [BIN_LOAD_ADDR]\n"
	"mcu reset - ready for bsl program of PC\n"
);
