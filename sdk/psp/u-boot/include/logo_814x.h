#include <command.h>
#include <common.h>
#include <ti81xx_vpdma_firmware.h>
#include <asm/arch/sys_proto.h>

#define COLORBAR_MODE 1
#define NORMAL_MODE 0

#define CPU_TI816X 0
#define CPU_TI814X 1
#define CPU_TI810X 2
#define CPU_TYPE	CPU_TI810X

#define LCD_DVO2
#define EN_TV_LOGO		0
#define DVO2_VENC_REG_BASE					(0x4810A000)

/* We will program the BOOT_LOGO_DISPLAY_TIME_IN_SECS*BOOT_LOGO_DISPLAY_FPS
    VPDMA descriptors

   Roughly on a 60FPS display like 1080p60 or 720p60
   this will result in boot logo being display for
    BOOT_LOGO_DISPLAY_TIME_IN_SECS seconds

   BOOT_LOGO_DISPLAY_TIME_IN_SECS*BOOT_LOGO_DISPLAY_FPS*32

    MUST BE <= VPDMA_DESC_BUFFER_SIZE
*/

#define DEFAULT_IMAGE_DEST_ADDR			0xb0000000
#define DEFAULT_VPDMA_DESC_ADDR			0xb0200000
#define DEFAULT_BOOT_LOGO_DISPLAY_TIME_IN_SECS  (60)
#define DEFAULT_BOOT_LOGO_DISPLAY_FPS           (60)

#define HDMI_REG_BASE 0x46C00000
#define HDMI_REG_SIZE (4*1024)

#define hdmi_read32(off)            *(volatile int*)(HDMI_REG_BASE+(off))
#define hdmi_write32(off, value)    *(volatile int*)(HDMI_REG_BASE+(off)) = (value)

#define vpdma_read32(off)            *(volatile int*)(VPDMA_REG_BASE+(off))
#define vpdma_write32(off, value)    (*(volatile int*)(VPDMA_REG_BASE+(off)) =(value))

#define VPDMA_REG_BASE 0x4810D000

#define DATA_TYPE  					0x6
#define DATA_TYPE_LCD				0x0		//# 0:RGB16-565, 6:RGB24-888
#define VPDMA_DESC_FIRMWARE_OFFSET  (0)
#define VPDMA_DESC_LIST_OFFSET      (0)
#define VPDMA_DESC_PAYLOAD_OFFSET   (0x1000)
#define VPDMA_DESC_BUFFER_SIZE      (256*1024)

#define VPDMA_DATA_DESC_SIZE        (32) /* in bytes */

#define VPDMA_PID                       0x000
#define VPDMA_LIST_ADDR                 0x004
#define VPDMA_LIST_ATTR                 0x008
#define VPDMA_LIST_STAT_SYNC            0x00C
#define VPDMA_BG_RGB                    0x018
#define VPDMA_GRPX0_DATA_CSTAT          0x37c
#define VPDMA_GRPX1_DATA_CSTAT          0x380
#define VPDMA_GRPX2_DATA_CSTAT          0x384
#define VPDMA_LIST_NUMBER_DVO2			0x3
#define VPDMA_LIST_NUMBER_HD            0x4
#define VPDMA_LIST_NUMBER_SD            0x5

#define vps_read32(off)            *(volatile int*)(VPS_REG_BASE+(off))
#define vps_write32(off, value)    *(volatile int*)(VPS_REG_BASE+(off)) = (value)

#define VPS_REG_BASE			0x48100000
#define VPS_REG_SIZE			(128*1024)

#define vps_read32(off)            *(volatile int*)(VPS_REG_BASE+(off))
#define vps_write32(off, value)    *(volatile int*)(VPS_REG_BASE+(off)) = (value)

#define VPS_CLKC_ENABLE                 0x0100
#define VPS_CLKC_VENC_CLK_SELECT        0x0114
#define VPS_CLKC_VENC_ENABLE            0x0118
#define VPS_COMP_STATUS                 0x5200
#define VPS_COMP_BGCOLOR                0x5214
#define LOGO_BGCOLOR			        0x00	/*Black*/
#define VPS_CLKC_RESET                  0x0104
#define VPS_COMP_HDMI_VOUT1             0x5204
#define VPS_COMP_HDCOMP			0x5208
#ifdef CPU_TI810X
	#define VPS_COMP_DVO2		0x5208
#else
	#define VPS_COMP_DVO2		0x520C
#endif
#define VPS_COMP_SD             0x5210
#define PRCM_REG_BASE 			0x48180000
#define PRCM_REG_SIZE 			(12*1024)
#define prcm_read32(off)            *(volatile int*)(PRCM_REG_BASE+(off))
#define prcm_write32(off, value)    (*(volatile int*)(PRCM_REG_BASE+(off)) =(value))

#define SD_write32(off, value)    (*(volatile int*)(off) =(value))
#define SD_read32(off)            *(volatile int*)(off)

#define CM_REG_BASE                     0x48140000
#define cm_write32(off, value)          (*(volatile int *)(CM_REG_BASE + (off)) = (value))

#define HD_DAC_CTRL_REG_ADDR	        0x1200
#define HD_CLK_CTRL_REG_ADDR			0x131C

#define CM_HDVPSS_CLKSTCTRL         	0x0800
#define CM_HDVPSS_HDVPSS_CLK_CTRL   	0x0820
#define CM_HDVPSS_HDMI_CLKCTRL      	0x0824

#define PM_HDVPSS_PWRSTCTRL         	0x0E00
#define PM_HDVPSS_PWRSTST           	0x0E04
#define RM_HDVPSS_RSTCTRL           	0x0E10
#define RM_HDVPSS_RSTST             	0x0E14

#define PLL_REG_BASE 			0x481C5000
#define PLL_REG_SIZE 			(4*1024)

#define pll_read32(off)            *(volatile int*)(PLL_REG_BASE+(off))
#define pll_write32(off, value)    (*(volatile int*)(PLL_REG_BASE+(off))= (unsigned int)(value))

#define CLKCTRL        			0x04
#define TENABLE        			0x08
#define TENABLEDIV     			0x0C
#define M2NDIV         			0x10
#define MN2DIV         			0x14
#define STATUS         			0x24
#define OSC_FREQ       			20

#define PLL_CONTROL_REVISION      	0x0000
#define PLL_CONTROL_SYSCONFIG     	0x0010

#define PLL_HDVPSS_BASE           	PLL_HDVPSS_PWRCTRL
#define PLL_HDVPSS_PWRCTRL        	0x0170
#define PLL_HDVPSS_CLKCTRL        	0x0174
#define PLL_HDVPSS_TENABLE        	0x0178
#define PLL_HDVPSS_TENABLEDIV     	0x017C
#define PLL_HDVPSS_M2NDIV         	0x0180
#define PLL_HDVPSS_MN2DIV         	0x0184
#define PLL_HDVPSS_FRACDIV        	0x0188
#define PLL_HDVPSS_BWCTRL         	0x018C
#define PLL_HDVPSS_FRACCTRL       	0x0190
#define PLL_HDVPSS_STATUS         	0x0194

#define PLL_VIDEO0_BASE           	PLL_VIDEO0_PWRCTRL
#define PLL_VIDEO0_PWRCTRL        	0x01A0
#define PLL_VIDEO0_CLKCTRL        	0x01A4
#define PLL_VIDEO0_TENABLE        	0x01A8
#define PLL_VIDEO0_TENABLEDIV     	0x01AC
#define PLL_VIDEO0_M2NDIV         	0x01B0
#define PLL_VIDEO0_MN2DIV         	0x01B4
#define PLL_VIDEO0_FRACDIV        	0x01B8
#define PLL_VIDEO0_BWCTRL         	0x01BC
#define PLL_VIDEO0_FRACCTRL       	0x01C0
#define PLL_VIDEO0_STATUS         	0x01C4

#define PLL_VIDEO1_BASE           	PLL_VIDEO1_PWRCTRL
#define PLL_VIDEO1_PWRCTRL       	0x01D0
#define PLL_VIDEO1_CLKCTRL        	0x01D4
#define PLL_VIDEO1_TENABLE        	0x01D8
#define PLL_VIDEO1_TENABLEDIV     	0x01DC
#define PLL_VIDEO1_M2NDIV         	0x01E0
#define PLL_VIDEO1_MN2DIV         	0x01E4
#define PLL_VIDEO1_FRACDIV        	0x01E8
#define PLL_VIDEO1_BWCTRL         	0x01EC
#define PLL_VIDEO1_FRACCTRL       	0x01F0
#define PLL_VIDEO1_STATUS         	0x01F4

#define PLL_VIDEO2_BASE           	PLL_VIDEO2_PWRCTRL
#define PLL_VIDEO2_PWRCTRL        	0x0200
#define PLL_VIDEO2_CLKCTRL        	0x0204
#define PLL_VIDEO2_TENABLE        	0x0208
#define PLL_VIDEO2_TENABLEDIV     	0x020C
#define PLL_VIDEO2_M2NDIV         	0x0210
#define PLL_VIDEO2_MN2DIV        	0x0214
#define PLL_VIDEO2_FRACDIV        	0x0218
#define PLL_VIDEO2_BWCTRL         	0x021C
#define PLL_VIDEO2_FRACCTRL       	0x0220
#define PLL_VIDEO2_STATUS         	0x0224
#define PLL_VIDEO2_PINMUX         	0x02C8

#define PLL_OSC_SRC_CTRL          	0x02C0

#define bmp_read64(off)		*(long long*)(off)
#define bmp_read16(off)		*(short int*)(off)

#define NAND_LOGO_FLASH_ADDR "0x0D500000"

#define BMP_SIGNATURE 0x4d42
#define BMP_WIDTH_OFFSET 0x12
#define BMP_HEIGHT_OFFSET 0x16
#define BMP_DATA_OFFSET 0xA
#define BMP_BITS_PER_PIXEL_OFFSET 0x1C
#define BMP_IMAGE_BOTTOMUP 0
#define BMP_IMAGE_TOPDOWN 1

typedef struct
 {
         uint32_t n;
         uint32_t m;
         uint32_t m2;
         uint32_t clk_ctrl;
 } pll_config_t;

typedef enum
{
       VPS_MODULE_VPDMA = 0
} vps_module_t;

static int ti810x_pll_get_dividers(uint32_t, int, pll_config_t*);
static void ti810x_pll_configure(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
static int ti810x_pll_config_hdvpss(void);
static int ti810x_vpdma_load_firmware(uint32_t*);
static int ti810x_prcm_enable_vps_power_and_clock(void);
static void ti810x_vps_configure_venc(uint32_t, int, int, int, int, int, int, int, int, int, int, int,char*);
int ti810x_pll_config_hdmi(uint32_t);
int ti810x_prcm_init(void);
int ti810x_pll_init(void);
int ti810x_vps_init(void);
int ti810x_set_mode(char []);
int ti810x_vpdma_send_list(uint32_t*, int, int);
int ti810x_dispmgr_wait_for_list_complete(int);
int ti810x_read_bmp_image(uint32_t lcd_flag,uint32_t bmp_source, uint32_t bmp_dest);
uint32_t ti810x_dispmgr_create_grpx_conf_descriptor(int, uint32_t*, uint32_t*, uint32_t, uint32_t);
uint32_t ti810x_dispmgr_setup_layers(uint32_t*, int , int, int,uint32_t, uint32_t, uint32_t);
uint32_t ti810x_dispmgr_setup_layers_SD(uint32_t*, int , int, int,uint32_t, uint32_t, uint32_t);
static void ti810x_pll_configure_SD(void);
void ti810x_pll_hdmi_setwrapper_clk(void);
int ti810x_pll_config_dvo2(uint32_t freq);
void ti810x_hdmi_enable(int);
void ti810x_vps_reset_module(vps_module_t, int);

