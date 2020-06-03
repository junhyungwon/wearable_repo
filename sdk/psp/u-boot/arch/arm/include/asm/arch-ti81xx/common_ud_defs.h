#ifndef __COMMON_UD_DEFS_H__
#define __COMMON_UD_DEFS_H__

#define BOARD_REV_00	0x0
#define BOARD_REV_01	0x1
#define BOARD_REV_02	0x2
#define BOARD_REV_03	0x3
#define BOARD_REV_04	0x4
#define BOARD_REV_05	0x5
#define BOARD_REV_06	0x6
#define BOARD_REV_07	0x7

#define SLW	(1 << 19)	//# SLEW_SLOW
#define INE	(1 << 18)	//# input enable

#define IPU	(1 << 17)	//# pull up
#define IPD	(0 << 17)	//# pull down

#define ENA	(0 << 16)	//# pull enable
#define DIS	(1 << 16)	//# pull disable

#define MODE(x)		(0x1 << (x - 1))
#define M0			0x01
#define M1			0x02
#define M2			0x04
#define M3			0x08
#define M4			0x10
#define M5			0x20
#define M6			0x40
#define M7			0x80	//# gpio

#define GPIO_N(bank, gpio)		((32*bank) + gpio)
#define MUX_VAL(offset, value)    __raw_writel(value, (CTRL_BASE + PINCtrl_##offset))
#define EX_MUX_VAL(offset, value) __raw_writel(value, (CTRL_BASE+offset))

#define PINCNTL(pin)	(0x0800+((pin-1)*4))

#define PINCtrl_MMC1_CLK		0x0800
#define PINCtrl_MMC1_CMD		0x0804
#define PINCtrl_MMC1_DAT0		0x0808
#define PINCtrl_MMC1_DAT1		0x080C
#define PINCtrl_MMC1_DAT2		0x0810
#define PINCtrl_MMC1_DAT3		0x0814
#define PINCtrl_OSC_WAKE		0x0818
#define PINCtrl_MMC0_CLK		0x081C
#define PINCtrl_MMC0_CMD		0x0820
#define PINCtrl_MMC0_DAT0		0x0824
#define PINCtrl_MMC0_DAT1		0x0828
#define PINCtrl_MMC0_DAT2		0x082C
#define PINCtrl_MMC0_DAT3		0x0830
#define PINCtrl_AUD_CLK0		0x0834
#define PINCtrl_AUD_CLK1		0x0838
#define PINCtrl_AUD_CLK2		0x083C
#define PINCtrl_MCASP0_ACLKX	0x0840
#define PINCtrl_MCASP0_FSX		0x0844
#define PINCtrl_MCASP0_CLKR		0x0848
#define PINCtrl_MCASP0_FSR		0x084C
#define PINCtrl_MCASP0_AXR0		0x0850
#define PINCtrl_MCASP0_AXR1		0x0854
#define PINCtrl_MCASP0_AXR2		0x0858
#define PINCtrl_MCASP0_AXR3		0x085C
#define PINCtrl_MCASP0_AXR4		0x0860
#define PINCtrl_MCASP0_AXR5		0x0864
#define PINCtrl_MCASP0_AXR6		0x0868
#define PINCtrl_MCASP0_AXR7		0x086C
#define PINCtrl_MCASP0_AXR8		0x0870
#define PINCtrl_MCASP0_AXR9		0x0874
#define PINCtrl_MCASP1_ACLKX	0x0878
#define PINCtrl_MCASP1_FSX		0x087C
#define PINCtrl_MCASP1_CLKR		0x0880
#define PINCtrl_MCASP1_FSR		0x0884
#define PINCtrl_MCASP1_AXR0		0x0888
#define PINCtrl_MCASP1_AXR1		0x088C
#define PINCtrl_MCASP1_AXR2		0x0890
#define PINCtrl_MCASP1_AXR3		0x0894
#define PINCtrl_MCASP2_CLKX		0x0898
#define PINCtrl_MCASP2_FSX		0x089C
#define PINCtrl_MCASP2_AXR0		0x08A0
#define PINCtrl_MCASP2_AXR1		0x08A4
#define PINCtrl_MCASP2_AXR2		0x08A8
#define PINCtrl_MCASP2_AXR3		0x08AC
#define PINCtrl_MCASP3_CLKX		0x08B0
#define PINCtrl_MCASP3_FSX		0x08B4
#define PINCtrl_MCASP3_AXR0		0x08B8
#define PINCtrl_MCASP3_AXR1		0x08BC
#define PINCtrl_MCASP3_AXR2		0x08C0
#define PINCtrl_MCASP3_AXR3		0x08C4
#define PINCtrl_MCASP4_CLKX		0x08C8
#define PINCtrl_MCASP4_FSX		0x08CC
#define PINCtrl_MCASP4_AXR0		0x08D0
#define PINCtrl_MCASP4_AXR1		0x08D4
#define PINCtrl_MCASP5_CLKX		0x08D8
#define PINCtrl_MCASP5_FSX		0x08DC
#define PINCtrl_MCASP5_AXR0		0x08E0
#define PINCtrl_MCASP5_AXR1		0x08E4
#define PINCtrl_MLB_SIG			0x08E8
#define PINCtrl_MLB_DAT			0x08EC
#define PINCtrl_MLB_CLK			0x08F0
#define PINCtrl_MLBP_SIG_P		0x08F4
#define PINCtrl_MLBP_SIG_N		0x08F8
#define PINCtrl_MLBP_DAT_P		0x08FC
#define PINCtrl_MLBP_DAT_N		0x0900
#define PINCtrl_MLBP_CLK_P		0x0904
#define PINCtrl_MLBP_CLK_N		0x0908
#define PINCtrl_DCAN0_TX		0x090C
#define PINCtrl_DCAN0_RX		0x0910
#define PINCtrl_UART0_RXD		0x0914
#define PINCtrl_UART0_TXD		0x0918
#define PINCtrl_UART0_CTSN		0x091C
#define PINCtrl_UART0_RTSN		0x0920
#define PINCtrl_UART0_DCDN		0x0924
#define PINCtrl_UART0_DSRN		0x0928
#define PINCtrl_UART0_DTRN		0x092C
#define PINCtrl_UART0_RIN		0x0930
#define PINCtrl_I2C1_SCL		0x0934
#define PINCtrl_I2C1_SDA		0x0938
#define PINCtrl_SPI0_CS1		0x093C
#define PINCtrl_SPI0_CS0		0x0940
#define PINCtrl_SPI0_SCLK		0x0944
#define PINCtrl_SPI0_D1			0x0948
#define PINCtrl_SPI0_D0			0x094C
#define PINCtrl_SPI1_CS0		0x0950
#define PINCtrl_SPI1_SCLK		0x0954
#define PINCtrl_SPI1_D1			0x0958
#define PINCtrl_SPI1_D0			0x095C
#define PINCtrl_GPMC_AD00		0x0960
#define PINCtrl_GPMC_AD01		0x0964
#define PINCtrl_GPMC_AD02		0x0968
#define PINCtrl_GPMC_AD03		0x096C
#define PINCtrl_GPMC_AD04		0x0970
#define PINCtrl_GPMC_AD05		0x0974
#define PINCtrl_GPMC_AD06		0x0978
#define PINCtrl_GPMC_AD07		0x097C
#define PINCtrl_GPMC_AD08		0x0980
#define PINCtrl_GPMC_AD09		0x0984
#define PINCtrl_GPMC_AD10		0x0988
#define PINCtrl_GPMC_AD11		0x098C
#define PINCtrl_GPMC_AD12		0x0990
#define PINCtrl_GPMC_AD13		0x0994
#define PINCtrl_GPMC_AD14		0x0998
#define PINCtrl_GPMC_AD15		0x099C
#define PINCtrl_GPMC_A16		0x09A0
#define PINCtrl_GPMC_A17		0x09A4
#define PINCtrl_GPMC_A18		0x09A8
#define PINCtrl_GPMC_A19		0x09AC
#define PINCtrl_GPMC_A20		0x09B0
#define PINCtrl_GPMC_A21		0x09B4
#define PINCtrl_GPMC_A22		0x09B8
#define PINCtrl_GPMC_A23		0x09BC
#define PINCtrl_MMC2_DAT7		0x09C0
#define PINCtrl_MMC2_DAT6		0x09C4
#define PINCtrl_MMC2_DAT5		0x09C8
#define PINCtrl_MMC2_DAT4		0x09CC
#define PINCtrl_MMC2_DAT3		0x09D0
#define PINCtrl_MMC2_DAT2		0x09D4
#define PINCtrl_MMC2_DAT1		0x09D8
#define PINCtrl_MMC2_DAT0		0x09DC
#define PINCtrl_MMC2_CLK		0x09E0
#define PINCtrl_GPMC_CS0		0x09E4
#define PINCtrl_GPMC_CS1		0x09E8
#define PINCtrl_GPMC_CS2		0x09EC
#define PINCtrl_GPMC_CS3		0x09F0
#define PINCtrl_GPMC_CS4		0x09F4
#define PINCtrl_GPMC_CLK		0x09F8
#define PINCtrl_GPMC_ADVN_ALE	0x09FC
#define PINCtrl_GPMC_OEN_REN	0x0A00
#define PINCtrl_GPMC_WEN		0x0A04
#define PINCtrl_GPMC_BEN0		0x0A08
#define PINCtrl_GPMC_BEN1		0x0A0C
#define PINCtrl_GPMC_WAIT0		0x0A10
#define PINCtrl_VIN0_CLK1		0x0A14
#define PINCtrl_VIN0_DE0_MUX0	0x0A18
#define PINCtrl_VIN0_FLD0_MUX0	0x0A1C
#define PINCtrl_VIN0_CLK0		0x0A20
#define PINCtrl_VIN0_HSYNC0		0x0A24
#define PINCtrl_VIN0_VSYNC0		0x0A28
#define PINCtrl_VIN0_D0			0x0A2C
#define PINCtrl_VIN0_D1			0x0A30
#define PINCtrl_VIN0_D2			0x0A34
#define PINCtrl_VIN0_D3			0x0A38
#define PINCtrl_VIN0_D4			0x0A3C
#define PINCtrl_VIN0_D5			0x0A40
#define PINCtrl_VIN0_D6			0x0A44
#define PINCtrl_VIN0_D7			0x0A48
#define PINCtrl_VIN0_D8			0x0A4C
#define PINCtrl_VIN0_D9			0x0A50
#define PINCtrl_VIN0_D10		0x0A54
#define PINCtrl_VIN0_D11		0x0A58
#define PINCtrl_VIN0_D12		0x0A5C
#define PINCtrl_VIN0_D13		0x0A60
#define PINCtrl_VIN0_D14		0x0A64
#define PINCtrl_VIN0_D15		0x0A68
#define PINCtrl_VIN0_D16		0x0A6C
#define PINCtrl_VIN0_D17		0x0A70
#define PINCtrl_VIN0_D18		0x0A74
#define PINCtrl_VIN0_D19		0x0A78
#define PINCtrl_VIN0_D20		0x0A7C
#define PINCtrl_VIN0_D21		0x0A80
#define PINCtrl_VIN0_D22		0x0A84
#define PINCtrl_VIN0_D23		0x0A88
#define PINCtrl_VIN0_DE0_MUX1	0x0A8C
#define PINCtrl_VIN0_DE1		0x0A90
#define PINCtrl_VIN0_FLD0_MUX1	0x0A94
#define PINCtrl_VIN0_FLD1		0x0A98
#define PINCtrl_VOUT1_G_Y_YC1	0x0A9C
#define PINCtrl_VOUT1_G_Y_YC0	0x0AA0
#define PINCtrl_VOUT1_R_CR1		0x0AA4
#define PINCtrl_VOUT1_R_CR0		0x0AA8
#define PINCtrl_VOUT1_B_CB_C1	0x0AAC
#define PINCtrl_VOUT1_B_CB_C0	0x0AB0
#define PINCtrl_VOUT1_FID		0x0AB4
#define PINCtrl_VOUT0_FID		0x0AB8
#define PINCtrl_VOUT0_CLK		0x0ABC
#define PINCtrl_VOUT0_HSYNC		0x0AC0
#define PINCtrl_VOUT0_VSYNC		0x0AC4
#define PINCtrl_VOUT0_AVID		0x0AC8
#define PINCtrl_VOUT0_B_CB_C2	0x0ACC
#define PINCtrl_VOUT0_B_CB_C3	0x0AD0
#define PINCtrl_VOUT0_B_CB_C4	0x0AD4
#define PINCtrl_VOUT0_B_CB_C5	0x0AD8
#define PINCtrl_VOUT0_B_CB_C6	0x0ADC
#define PINCtrl_VOUT0_B_CB_C7	0x0AE0
#define PINCtrl_VOUT0_B_CB_C8	0x0AE4
#define PINCtrl_VOUT0_B_CB_C9	0x0AE8
#define PINCtrl_VOUT0_G_Y_YC2	0x0AEC
#define PINCtrl_VOUT0_G_Y_YC3	0x0AF0
#define PINCtrl_VOUT0_G_Y_YC4	0x0AF4
#define PINCtrl_VOUT0_G_Y_YC5	0x0AF8
#define PINCtrl_VOUT0_G_Y_YC6	0x0AFC
#define PINCtrl_VOUT0_G_Y_YC7	0x0B00
#define PINCtrl_VOUT0_G_Y_YC8	0x0B04
#define PINCtrl_VOUT0_G_Y_YC9	0x0B08
#define PINCtrl_VOUT0_R_CR2		0x0B0C
#define PINCtrl_VOUT0_R_CR3		0x0B10
#define PINCtrl_VOUT0_R_CR4		0x0B14
#define PINCtrl_VOUT0_R_CR5		0x0B18
#define PINCtrl_VOUT0_R_CR6		0x0B1C
#define PINCtrl_VOUT0_R_CR7		0x0B20
#define PINCtrl_VOUT0_R_CR8		0x0B24
#define PINCtrl_VOUT0_R_CR9		0x0B28
#define PINCtrl_VOUT1_CLK		0x0B2C
#define PINCtrl_VOUT1_HSYNC		0x0B30
#define PINCtrl_VOUT1_VSYNC		0x0B34
#define PINCtrl_VOUT1_AVID		0x0B38
#define PINCtrl_VOUT1_B_CB_C3	0x0B3C
#define PINCtrl_VOUT1_B_CB_C4	0x0B40
#define PINCtrl_VOUT1_B_CB_C5	0x0B44
#define PINCtrl_VOUT1_B_CB_C6	0x0B48
#define PINCtrl_VOUT1_B_CB_C7	0x0B4C
#define PINCtrl_VOUT1_B_CB_C8	0x0B50
#define PINCtrl_VOUT1_B_CB_C9	0x0B54
#define PINCtrl_VOUT1_G_Y_YC3	0x0B58
#define PINCtrl_VOUT1_G_Y_YC4	0x0B5C
#define PINCtrl_VOUT1_G_Y_YC5	0x0B60
#define PINCtrl_VOUT1_G_Y_YC6	0x0B64
#define PINCtrl_VOUT1_G_Y_YC7	0x0B68
#define PINCtrl_VOUT1_G_Y_YC8	0x0B6C
#define PINCtrl_VOUT1_G_Y_YC9	0x0B70
#define PINCtrl_VOUT1_R_CR4		0x0B74
#define PINCtrl_VOUT1_R_CR5		0x0B78
#define PINCtrl_VOUT1_R_CR6		0x0B7C
#define PINCtrl_VOUT1_R_CR7		0x0B80
#define PINCtrl_VOUT1_R_CR8		0x0B84
#define PINCtrl_VOUT1_R_CR9		0x0B88
#define PINCtrl_VOUT1_G_Y_YC2	0x0B8C
#define PINCtrl_VOUT1_R_CR3		0x0B90
#define PINCtrl_VOUT1_R_CR2		0x0B94
#define PINCtrl_VOUT1_B_CB_C2	0x0B98
#define PINCtrl_RMII_REFCLK		0x0B9C
#define PINCtrl_MDIO_MCLK		0x0BA0
#define PINCtrl_MDIO_D			0x0BA4
#define PINCtrl_GMII0_TXCLK		0x0BA8
#define PINCtrl_GMII0_COL		0x0BAC
#define PINCtrl_GMII0_CRS		0x0BB0
#define PINCtrl_GMII0_RXER		0x0BB4
#define PINCtrl_GMII0_RXCLK		0x0BB8
#define PINCtrl_GMII0_RXD0		0x0BBC
#define PINCtrl_GMII0_RXD1		0x0BC0
#define PINCtrl_GMII0_RXD2		0x0BC4
#define PINCtrl_GMII0_RXD3		0x0BC8
#define PINCtrl_GMII0_RXD4		0x0BCC
#define PINCtrl_GMII0_RXD5		0x0BD0
#define PINCtrl_GMII0_RXD6		0x0BD4
#define PINCtrl_GMII0_RXD7		0x0BD8
#define PINCtrl_GMII0_RXDV		0x0BDC
#define PINCtrl_GMII0_GTXCLK	0x0BE0
#define PINCtrl_GMII0_TXD0		0x0BE4
#define PINCtrl_GMII0_TXD1		0x0BE8
#define PINCtrl_GMII0_TXD2		0x0BEC
#define PINCtrl_GMII0_TXD3		0x0BF0
#define PINCtrl_GMII0_TXD4		0x0BF4
#define PINCtrl_GMII0_TXD5		0x0BF8
#define PINCtrl_GMII0_TXD6		0x0BFC
#define PINCtrl_GMII0_TXD7		0x0C00
#define PINCtrl_GMII0_TXEN		0x0C04
#define PINCtrl_CLKIN32			0x0C08
#define PINCtrl_RESETN			0x0C0C
#define PINCtrl_NMIN			0x0C10
#define PINCtrl_RSTOUTN			0x0C14
#define PINCtrl_I2C0_SCL		0x0C18
#define PINCtrl_I2C0_SDA		0x0C1C
#define PINCtrl_USB_DRVVBUS		0x0C34

/* HDVPSS */
#define CM_DSS_CLKSTCTRL         (PRCM_BASE + 0x0800)
#define CM_DSS_CLKCTRL   		 (PRCM_BASE + 0x0820)
#define CM_DSS_HDMI_CLKCTRL      (PRCM_BASE + 0x0824)

#define PM_DSS_PWRSTCTRL         (PRCM_BASE + 0x0E00)
#define PM_DSS_PWRSTST           (PRCM_BASE + 0x0E04)
#define RM_DSS_RSTCTRL           (PRCM_BASE + 0x0E10)
#define RM_DSS_RSTST             (PRCM_BASE + 0x0E14)

#define HDVPSS_CLKC_CLKEN           (HDVPSS_REG_BASE + 0x0100)
#define HDVPSS_CLKC_RST             (HDVPSS_REG_BASE + 0x0104)
#define HDVPSS_CLKC_VENC_CLKSEL     (HDVPSS_REG_BASE + 0x0114)
#define HDVPSS_CLKC_VENC_ENA     	(HDVPSS_REG_BASE + 0x0118)

#define HDVPSS_COMP_STATUS          (HDVPSS_COMP_REG_BASE + 0x0ul)
#define HDVPSS_COMP_HDMI_DVO1       (HDVPSS_COMP_REG_BASE + 0x04ul)
#define HDVPSS_COMP_DVO2			(HDVPSS_COMP_REG_BASE + 0x08ul)
#define HDVPSS_COMP_HDCOMP			(HDVPSS_COMP_REG_BASE + 0x0Cul)
#define HDVPSS_COMP_SD             	(HDVPSS_COMP_REG_BASE + 0x10ul)
#define HDVPSS_COMP_BACK_COLOR      (HDVPSS_COMP_REG_BASE + 0x14ul)

/* HDVPSS_SD_VENC */
#define HDVPSS_SD_VENC_VMOD	        (HDVPSS_SD_VENC_REG_BASE + 0x04ul)
#define HDVPSS_SD_VENC_SLAVE        (HDVPSS_SD_VENC_REG_BASE + 0x08ul)
#define HDVPSS_SD_VENC_SIZE			(HDVPSS_SD_VENC_REG_BASE + 0x0Cul)
#define HDVPSS_SD_VENC_POL			(HDVPSS_SD_VENC_REG_BASE + 0x10ul)
#define HDVPSS_SD_VENC_DTVS0       	(HDVPSS_SD_VENC_REG_BASE + 0x14ul)
#define HDVPSS_SD_VENC_DTVS1      	(HDVPSS_SD_VENC_REG_BASE + 0x18ul)
#define HDVPSS_SD_VENC_DTVS2       	(HDVPSS_SD_VENC_REG_BASE + 0x1Cul)
#define HDVPSS_SD_VENC_DTVS3      	(HDVPSS_SD_VENC_REG_BASE + 0x20ul)
#define HDVPSS_SD_VENC_DTVS4       	(HDVPSS_SD_VENC_REG_BASE + 0x24ul)
#define HDVPSS_SD_VENC_DTVS5      	(HDVPSS_SD_VENC_REG_BASE + 0x28ul)
#define HDVPSS_SD_VENC_DTVS6       	(HDVPSS_SD_VENC_REG_BASE + 0x2Cul)
#define HDVPSS_SD_VENC_DTVS7      	(HDVPSS_SD_VENC_REG_BASE + 0x30ul)
#define HDVPSS_SD_VENC_TVDETGP1    	(HDVPSS_SD_VENC_REG_BASE + 0x58ul)
#define HDVPSS_SD_VENC_IRQ0	    	(HDVPSS_SD_VENC_REG_BASE + 0x5cul)
#define HDVPSS_SD_VENC_ESTAT	    (HDVPSS_SD_VENC_REG_BASE + 0x80ul)
#define HDVPSS_SD_VENC_ECTRL	    (HDVPSS_SD_VENC_REG_BASE + 0x84ul)
#define HDVPSS_SD_VENC_ETMGR0	    (HDVPSS_SD_VENC_REG_BASE + 0x88ul)
#define HDVPSS_SD_VENC_ETMGR1	    (HDVPSS_SD_VENC_REG_BASE + 0x8Cul)
#define HDVPSS_SD_VENC_ETMGR2	    (HDVPSS_SD_VENC_REG_BASE + 0x90ul)
#define HDVPSS_SD_VENC_ETMGR3	    (HDVPSS_SD_VENC_REG_BASE + 0x94ul)
#define HDVPSS_SD_VENC_ETMGR4	    (HDVPSS_SD_VENC_REG_BASE + 0x98ul)
#define HDVPSS_SD_VENC_CVBS0	    (HDVPSS_SD_VENC_REG_BASE + 0x9cul)
#define HDVPSS_SD_VENC_CVBS1	    (HDVPSS_SD_VENC_REG_BASE + 0xA0ul)
#define HDVPSS_SD_VENC_CCSC0	   	(HDVPSS_SD_VENC_REG_BASE + 0xACul)
#define HDVPSS_SD_VENC_CCSC1	   	(HDVPSS_SD_VENC_REG_BASE + 0xB0ul)
#define HDVPSS_SD_VENC_CCSC2	   	(HDVPSS_SD_VENC_REG_BASE + 0xB4ul)
#define HDVPSS_SD_VENC_CCSC3	   	(HDVPSS_SD_VENC_REG_BASE + 0xB8ul)
#define HDVPSS_SD_VENC_CCSC4	   	(HDVPSS_SD_VENC_REG_BASE + 0xBCul)
#define HDVPSS_SD_VENC_CCSC5	   	(HDVPSS_SD_VENC_REG_BASE + 0xC0ul)
#define HDVPSS_SD_VENC_CCSC6	   	(HDVPSS_SD_VENC_REG_BASE + 0xC4ul)
#define HDVPSS_SD_VENC_CCSC7	   	(HDVPSS_SD_VENC_REG_BASE + 0xC8ul)
#define HDVPSS_SD_VENC_CCSC8	   	(HDVPSS_SD_VENC_REG_BASE + 0xCCul)
#define HDVPSS_SD_VENC_CYGCLP	   	(HDVPSS_SD_VENC_REG_BASE + 0xf4ul)
#define HDVPSS_SD_VENC_CUBCLP	   	(HDVPSS_SD_VENC_REG_BASE + 0xf8ul)
#define HDVPSS_SD_VENC_CVRCLP	   	(HDVPSS_SD_VENC_REG_BASE + 0xfcul)

/* HDMI */
#define HDMI_WP_SYSCONFIG      		(HDMI_WP_REG_BASE + 0x0010UL)
#define HDMI_WP_PWR_CTRL			(HDMI_WP_REG_BASE + 0x0040UL)
#define HDMI_WP_DEBOUNCE			(HDMI_WP_REG_BASE + 0x0044UL)
#define HDMI_WP_VIDEO_CFG			(HDMI_WP_REG_BASE + 0x0050ul)
#define HDMI_WP_CLK					(HDMI_WP_REG_BASE + 0x0070UL)
#define HDMI_WP_AUDIO_CFG			(HDMI_WP_REG_BASE + 0x0080UL)

#define HDMI_CORE_SYS__SRST			(HDMI_CORE_REG_BASE + 0x14ul)
#define HDMI_CORE_SYS__CTRL1		(HDMI_CORE_REG_BASE + 0x20ul)
#define HDMI_CORE_SYS__SYS_STAT		(HDMI_CORE_REG_BASE + 0x24ul)
#define HDMI_CORE_SYS__DE_CTRL		(HDMI_CORE_REG_BASE + 0xCCul)
#define HDMI_CORE_SYS__IADJUST		(HDMI_CORE_REG_BASE + 0xF8ul)

#define HDMI_CORE_SYS__VID_CTRL		(HDMI_CORE_REG_BASE + 0x120ul)
#define HDMI_CORE_SYS__VID_ACEN		(HDMI_CORE_REG_BASE + 0x124ul)
#define HDMI_CORE_SYS__VID_MODE		(HDMI_CORE_REG_BASE + 0x128ul)
#define HDMI_CORE_SYS__DC_HEADER	(HDMI_CORE_REG_BASE + 0x138ul)

#define HDMI_CORE_AV_AVI_DBYTE__ELSIZE	(HDMI_IP_CORE_AV_REG_BASE + 0x4ul)
#define HDMI_CORE_AV_AUD_MODE			(HDMI_IP_CORE_AV_REG_BASE + 0x50ul)
#define HDMI_CORE_AV_HDMI_CTRL			(HDMI_IP_CORE_AV_REG_BASE + 0xBCul)
#define HDMI_CORE_AV_AUDO_TXSTAT		(HDMI_IP_CORE_AV_REG_BASE + 0xC0ul)
#define HDMI_CORE_AV_PB_CTRL1			(HDMI_IP_CORE_AV_REG_BASE + 0xF8ul)
#define HDMI_CORE_AV_PB_CTRL2			(HDMI_IP_CORE_AV_REG_BASE + 0xFCul)
#define HDMI_CORE_AV_AUD_TYPE			(HDMI_IP_CORE_AV_REG_BASE + 0x0200ul)
#define HDMI_CORE_AV_AUD_VERS			(HDMI_IP_CORE_AV_REG_BASE + 0x0204ul)
#define HDMI_CORE_AV_AUD_LEN			(HDMI_IP_CORE_AV_REG_BASE + 0x0208ul)
#define HDMI_CORE_AV_AUD_DBYTE_0		(HDMI_IP_CORE_AV_REG_BASE + 0x0210ul)
#define HDMI_CORE_AV_AUD_DBYTE_1		(HDMI_IP_CORE_AV_REG_BASE + 0x0214ul)
#define HDMI_CORE_AV_AUD_DBYTE_2		(HDMI_IP_CORE_AV_REG_BASE + 0x0218ul)
#define HDMI_CORE_AV_AUD_DBYTE_3		(HDMI_IP_CORE_AV_REG_BASE + 0x021Cul)

/* VPDMA_REG_BASE */
#define VPDMA_PID                       (VPDMA_REG_BASE + 0x0ul)
#define VPDMA_LIST_ADDR                 (VPDMA_REG_BASE + 0x4ul)
#define VPDMA_LIST_ATTR                 (VPDMA_REG_BASE + 0x8ul)
#define VPDMA_LIST_STAT_SYNC            (VPDMA_REG_BASE + 0xCul)
#define VPDMA_BG_RGB                    (VPDMA_REG_BASE + 0x18ul)
#define VPDMA_GRPX0_DATA_CSTAT          (VPDMA_REG_BASE + 0x37cul)
#define VPDMA_GRPX1_DATA_CSTAT          (VPDMA_REG_BASE + 0x380ul)
#define VPDMA_GRPX2_DATA_CSTAT          (VPDMA_REG_BASE + 0x384ul)

#endif /* __COMMON_UD_DEFS_H__ */
