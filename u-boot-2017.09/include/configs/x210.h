/*
 * Copyright (C) 2009 Samsung Electronics
 * Minkyu Kang <mk7.kang@samsung.com>
 * Kyungmin Park <kyungmin.park@samsung.com>
 *
 * Configuation settings for the SAMSUNG Universal (s5pc100) board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/* High Level Configuration Options */
#define CONFIG_SAMSUNG		1	/* in a SAMSUNG core */
#define CONFIG_S5P		1	/* which is in a S5P Family */
#define CONFIG_S5PC110		1	/* which is in a S5PC110 */
#define CONFIG_MACH_GONI	1	/* working with Goni */

#include <linux/sizes.h>
#include <asm/arch/cpu.h>		/* get chip and board defs */

#define CONFIG_ARCH_CPU_INIT

/* input clock of PLL: has 24MHz input clock at S5PC110 */
#define CONFIG_SYS_CLK_FREQ_C110	24000000

/* DRAM Base */
#define CONFIG_SYS_SDRAM_BASE		0x30000000

/* Text Base */
#define CONFIG_SYS_TEXT_BASE		0x33E00000	// 0x34800000

#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG
#define CONFIG_REVISION_TAG
#define CONFIG_INITRD_TAG
#define CONFIG_CMDLINE_EDITING

/* Size of malloc() pool before and after relocation */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + (80 << 20))

/*
 * select serial console configuration
 */
#define CONFIG_SERIAL2			1	/* use SERIAL2 */

/* MMC */
#define SDHCI_MAX_HOSTS		4

/* PWM */
#define CONFIG_PWM			1

/* USB Composite download gadget - g_dnl */
#define CONFIG_SYS_DFU_DATA_BUF_SIZE SZ_32M
#define DFU_DEFAULT_POLL_TIMEOUT 300

/* TIZEN THOR downloader support */
#define CONFIG_USB_FUNCTION_THOR

/* USB Samsung's IDs */

#define CONFIG_G_DNL_THOR_VENDOR_NUM 0x04E8
#define CONFIG_G_DNL_THOR_PRODUCT_NUM 0x685D
#define CONFIG_G_DNL_UMS_VENDOR_NUM 0x0525
#define CONFIG_G_DNL_UMS_PRODUCT_NUM 0xA4A5

/* Actual modem binary size is 16MiB. Add 2MiB for bad block handling */
#define MTDIDS_DEFAULT		"onenand0=samsung-onenand"
#define MTDPARTS_DEFAULT	"mtdparts=samsung-onenand:1m(bootloader)"\
				",256k(params)"\
				",2816k(config)"\
				",8m(csa)"\
				",7m(kernel)"\
				",1m(log)"\
				",12m(modem)"\
				",60m(qboot)\0"

/* partitions definitions */
#define PARTS_CSA			"csa-mmc"
#define PARTS_BOOTLOADER	"u-boot"
#define PARTS_BOOT			"boot"
#define PARTS_ROOT			"platform"
#define PARTS_DATA			"data"
#define PARTS_CSC			"csc"
#define PARTS_UMS			"ums"

#define CONFIG_DFU_ALT \
	"u-boot raw 0x80 0x400;" \
	"uImage ext4 0 2;" \
	"exynos3-goni.dtb ext4 0 2;" \
	""PARTS_ROOT" part 0 5\0"

#define PARTS_DEFAULT \
	"uuid_disk=${uuid_gpt_disk};" \
	"name="PARTS_CSA",size=8MiB,uuid=${uuid_gpt_"PARTS_CSA"};" \
	"name="PARTS_BOOTLOADER",size=60MiB," \
	"uuid=${uuid_gpt_"PARTS_BOOTLOADER"};" \
	"name="PARTS_BOOT",size=100MiB,uuid=${uuid_gpt_"PARTS_BOOT"};" \
	"name="PARTS_ROOT",size=1GiB,uuid=${uuid_gpt_"PARTS_ROOT"};" \
	"name="PARTS_DATA",size=3GiB,uuid=${uuid_gpt_"PARTS_DATA"};" \
	"name="PARTS_CSC",size=150MiB,uuid=${uuid_gpt_"PARTS_CSC"};" \
	"name="PARTS_UMS",size=-,uuid=${uuid_gpt_"PARTS_UMS"}\0" \

#define CONFIG_BOOTCOMMAND	"run mmcboot"

#define CONFIG_DEFAULT_CONSOLE	"console=ttySAC2,115200n8\0"

#define CONFIG_RAMDISK_BOOT	"root=/dev/ram0 rw rootfstype=ext4" \
		" ${console} ${meminfo}"

#define CONFIG_COMMON_BOOT	"${console} ${meminfo} ${mtdparts}"

#define CONFIG_UPDATEB	"updateb=onenand erase 0x0 0x100000;" \
			" onenand write 0x32008000 0x0 0x100000\0"

#define CONFIG_MISC_COMMON
#define CONFIG_MISC_INIT_R

#define CONFIG_ENV_OVERWRITE
#define CONFIG_ENV_VARS_UBOOT_CONFIG
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
#define CONFIG_EXTRA_ENV_SETTINGS					\
	CONFIG_UPDATEB \
	"updatek=" \
		"onenand erase 0xc00000 0x600000;" \
		"onenand write 0x31008000 0xc00000 0x600000\0" \
	"updateu=" \
		"onenand erase 0x01560000 0x1eaa0000;" \
		"onenand write 0x32000000 0x1260000 0x8C0000\0" \
	"bootk=" \
		"run loaduimage;" \
		"bootm 0x30007FC0\0" \
	"flashboot=" \
		"set bootargs root=/dev/mtdblock${bootblock} " \
		"rootfstype=${rootfstype} ${opts} " \
		"${lcdinfo} " CONFIG_COMMON_BOOT "; run bootk\0" \
	"ubifsboot=" \
		"set bootargs root=ubi0!rootfs rootfstype=ubifs " \
		"${opts} ${lcdinfo} " \
		CONFIG_COMMON_BOOT "; run bootk\0" \
	"tftpboot=" \
		"set bootargs root=ubi0!rootfs rootfstype=ubifs " \
		"${opts} ${lcdinfo} " CONFIG_COMMON_BOOT \
		"; tftp 0x30007FC0 uImage; bootm 0x30007FC0\0" \
	"ramboot=" \
		"set bootargs " CONFIG_RAMDISK_BOOT \
		"initrd=0x33000000,8M ramdisk=8192\0" \
	"mmcboot=" \
		"set bootargs root=/dev/mmcblk${mmcdev}p${mmcrootpart} " \
		"rootfstype=${rootfstype} ${opts} ${lcdinfo} " \
		CONFIG_COMMON_BOOT "; run bootk\0" \
	"boottrace=setenv opts initcall_debug; run bootcmd\0" \
	"bootchart=set opts init=/sbin/bootchartd; run bootcmd\0" \
	"verify=n\0" \
	"rootfstype=ext4\0" \
	"console=" CONFIG_DEFAULT_CONSOLE \
	"meminfo=mem=80M mem=256M@0x40000000 mem=128M@0x50000000\0" \
	"loaduimage=ext4load mmc ${mmcdev}:${mmcbootpart} 0x30007FC0 uImage\0" \
	"mmcdev=0\0" \
	"mmcbootpart=2\0" \
	"mmcrootpart=5\0" \
	"partitions=" PARTS_DEFAULT \
	"bootblock=9\0" \
	"ubiblock=8\0" \
	"ubi=enabled\0" \
	"opts=always_resume=1\0" \
	"dfu_alt_info=" CONFIG_DFU_ALT "\0"

#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_PBSIZE	384	/* Print Buffer Size */
/* memtest works on */
#if 0
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0x5000000)
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x4000000)
#endif
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE	/* memtest works on	*/
#define CONFIG_SYS_MEMTEST_END		CONFIG_SYS_SDRAM_BASE + 0x3E00000		/* 256 MB in DRAM	*/
#define CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_SDRAM_BASE	/* default load address	*/

/* Goni has 3 banks of DRAM, but swap the bank */
/* SDRAM1: 0x3000_0000 --- 0x3500_0000
 * SDRAM2: 0x4000_0000 --- 0x5000_0000
 * SDRAM3: 0x5000_0000 --- 0x5800_0000
 * */
#if 0
#define CONFIG_NR_DRAM_BANKS	3
#define PHYS_SDRAM_1		CONFIG_SYS_SDRAM_BASE	/* OneDRAM Bank #0 */
#define PHYS_SDRAM_1_SIZE	(80 << 20)		/* 80 MB in Bank #0 */
#define PHYS_SDRAM_2		0x40000000		/* mDDR DMC1 Bank #1 */
#define PHYS_SDRAM_2_SIZE	(256 << 20)		/* 256 MB in Bank #1 */
#define PHYS_SDRAM_3		0x50000000		/* mDDR DMC2 Bank #2 */
#define PHYS_SDRAM_3_SIZE	(128 << 20)		/* 128 MB in Bank #2 */
#endif
/* 该x210开发板有512MB内存
 * SDRAM1: 0x3000_0000 --- 0x4000_0000
 * SDRAM2: 0x4000_0000 --- 0x5000_0000
 * */
#define CONFIG_NR_DRAM_BANKS    2               /* we have 2 bank of DRAM */
#define SDRAM_BANK_SIZE         0x10000000    	/* 256 MB */
#define PHYS_SDRAM_1            CONFIG_SYS_SDRAM_BASE/* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE       SDRAM_BANK_SIZE
#define PHYS_SDRAM_2            0x40000000     /* SDRAM Bank #2 */
#define PHYS_SDRAM_2_SIZE       SDRAM_BANK_SIZE


#define CONFIG_SYS_MONITOR_BASE		0x00000000
#define CONFIG_SYS_MONITOR_LEN		(256 << 10)	/* 256 KiB */

/* FLASH and environment organization */
/* #define CONFIG_MMC_DEFAULT_DEV	0 */
/* 默认MMC改为1 */
#define CONFIG_MMC_DEFAULT_DEV	1
#define CONFIG_SYS_MMC_ENV_DEV		CONFIG_MMC_DEFAULT_DEV
#define CONFIG_ENV_SIZE			4096
/* #define CONFIG_ENV_OFFSET		((32 - 4) << 10) */ /* 32KiB - 4KiB */
/* 环境变量存在34～42扇区，共4KB
 * uboot bl1存在1～32扇区，共16KB
 * uboot bl2存在49扇区以后 */
#define CONFIG_ENV_OFFSET		(17 << 10)
#define CONFIG_ENV_OVERWRITE

#define CONFIG_USE_ONENAND_BOARD_INIT
#define CONFIG_SAMSUNG_ONENAND		1
#define CONFIG_SYS_ONENAND_BASE		0xB0000000

/* write support for filesystems */
/* 这个在menuconfig中定义了
 * 不知道为什么uboot-2017中的/fs/ext4/Kconfig是空的，这就导致了menuconfig中
 * 没有ext4相关选项，这里是复制了官方uboot的Kconfig，然后选中了ext4相关支持 */
#if 0
#define CONFIG_EXT4_WRITE
#endif

/* GPT */
#if 0
#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_LOAD_ADDR - 0x1000000)	/* 0x3300_0000 */
#endif
#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_TEXT_BASE - 12)

#define CONFIG_USB_GADGET_DWC2_OTG_PHY
#define CONFIG_USB_FUNCTION_MASS_STORAGE

/******************************* clock ******************************/
#define SET_CHECK_MPLL_LOCK	1
#define SET_MCP_SINGLE	1
#define SET_EVT1		1
/* #define SET_CLK_667_166_166_133 */
/* #define SET_CLK_533_133_100_100 */
/* #define SET_CLK_800_200_166_133 */
/* #define SET_CLK_800_100_166_133 */
#define SET_CLK_1000_200_166_133
/* #define SET_CLK_400_200_166_133 */
/* #define SET_CLK_400_100_166_133 */

#if defined(SET_CLK_667_166_166_133)
#define APLL_MDIV       0xfa
#define APLL_PDIV       0x6
#define APLL_SDIV       0x1
#elif defined(SET_CLK_533_133_100_100)
#define APLL_MDIV       0x215
#define APLL_PDIV       0x18
#define APLL_SDIV       0x1
#elif defined(SET_CLK_800_200_166_133) || \
	defined(SET_CLK_800_100_166_133) || \
	defined(SET_CLK_400_200_166_133) || \
	defined(SET_CLK_400_100_166_133)
#define APLL_MDIV       0x64
#define APLL_PDIV       0x3
#define APLL_SDIV       0x1
#elif defined(SET_CLK_1000_200_166_133)
#define APLL_MDIV       0x7d
#define APLL_PDIV       0x3
#define APLL_SDIV       0x1
#endif

/* #define APLL_LOCKTIME_VAL	0x2cf */
#define APLL_LOCKTIME_VAL	0xe10 //lxg changed.
#define MPLL_LOCKTIME_VAL	0xe10 //lxg added

#if defined(SET_EVT1)
/* Set AFC value */
#define AFC_ON		0x00000000
#define AFC_OFF		0x10000010
#endif

#if defined(SET_CLK_533_133_100_100)
#define MPLL_MDIV	0x190
#define MPLL_PDIV	0x6
#define MPLL_SDIV	0x2
#else
#define MPLL_MDIV	0x29b
#define MPLL_PDIV	0xc
#define MPLL_SDIV	0x1
#endif

#define EPLL_MDIV	0x60
#define EPLL_PDIV	0x6
#define EPLL_SDIV	0x2

#define VPLL_MDIV	0x6c
#define VPLL_PDIV	0x6
#define VPLL_SDIV	0x3

/* CLK_DIV0 */
#define APLL_RATIO	0
#define A2M_RATIO	4
#define HCLK_MSYS_RATIO	8
#define PCLK_MSYS_RATIO	12
#define HCLK_DSYS_RATIO	16
#define PCLK_DSYS_RATIO 20
#define HCLK_PSYS_RATIO	24
#define PCLK_PSYS_RATIO 28

#define CLK_DIV0_MASK	0x7fffffff
/*******lxg added**************/
#define CLK_DIV1_MASK	0xffffffff
#define CLK_DIV2_MASK	0x0fff
#define CLK_DIV3_MASK	0x7fffffff
#define CLK_DIV4_MASK	0xffffffff
#define CLK_DIV6_MASK	0xffffffff
/*******end**************/
#define set_pll(mdiv, pdiv, sdiv)	(1<<31 | mdiv<<16 | pdiv<<8 | sdiv)

#define APLL_VAL	set_pll(APLL_MDIV,APLL_PDIV,APLL_SDIV)
#define MPLL_VAL	set_pll(MPLL_MDIV,MPLL_PDIV,MPLL_SDIV)
#define EPLL_VAL	set_pll(EPLL_MDIV,EPLL_PDIV,EPLL_SDIV)
#define VPLL_VAL	set_pll(VPLL_MDIV,VPLL_PDIV,VPLL_SDIV)

#if defined(SET_CLK_667_166_166_133)
#define CLK_DIV0_VAL    ((0<<APLL_RATIO)|(3<<A2M_RATIO)|(3<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
			|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))
#elif defined(SET_CLK_533_133_100_100)
#define CLK_DIV0_VAL    ((0<<APLL_RATIO)|(3<<A2M_RATIO)|(3<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
			|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(3<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))
#elif defined(SET_CLK_800_200_166_133)
#define CLK_DIV0_VAL    ((0<<APLL_RATIO)|(3<<A2M_RATIO)|(3<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
			|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))
#elif defined(SET_CLK_800_100_166_133)
#define CLK_DIV0_VAL    ((0<<APLL_RATIO)|(7<<A2M_RATIO)|(7<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
			|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))
#elif defined(SET_CLK_400_200_166_133)
#define CLK_DIV0_VAL    ((1<<APLL_RATIO)|(3<<A2M_RATIO)|(1<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
			|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))
#elif defined(SET_CLK_400_100_166_133)
#define CLK_DIV0_VAL    ((1<<APLL_RATIO)|(7<<A2M_RATIO)|(3<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
			|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))			
#elif defined(SET_CLK_1000_200_166_133)
#define CLK_DIV0_VAL    ((0<<APLL_RATIO)|(4<<A2M_RATIO)|(4<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
			|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))
#endif

#define CLK_DIV1_VAL	((1<<16)|(1<<12)|(1<<8)|(1<<4))
#define CLK_DIV2_VAL	(1<<0)
#define CLK_DIV4_VAL	0x99990000
#define CLK_DIV6_VAL	0x71000

#if defined(SET_CLK_533_133_100_100)

#if defined(SET_MCP_SINGLE)

#define DMC0_TIMINGA_REF	0x40e
#define DMC0_TIMING_ROW		0x10233206
#define DMC0_TIMING_DATA	0x12130005
#define	DMC0_TIMING_PWR		0x0E100222

#define DMC1_TIMINGA_REF	0x40e
#define DMC1_TIMING_ROW		0x10233206
#define DMC1_TIMING_DATA	0x12130005
#define	DMC1_TIMING_PWR		0x0E100222

#else

#error "You should define memory type (AC type or H type or B type)"

#endif

#elif defined(SET_CLK_800_200_166_133) || \
	defined(SET_CLK_1000_200_166_133) || \
	defined(SET_CLK_800_100_166_133) || \
	defined(SET_CLK_400_200_166_133) || \
	defined(SET_CLK_400_100_166_133)

#if defined(SET_MCP_SINGLE)

#define DMC0_MEMCONTROL		0x00212400	// MemControl	BL=4, 1Chip, DDR2 Type, dynamic self refresh, force precharge, dynamic power down off
#define DMC0_MEMCONFIG_0	0x30F01313	// MemConfig0	256MB config, 8 banks,Mapping Method[12:15]0:linear, 1:linterleaved, 2:Mixed
#define DMC0_MEMCONFIG_1	0x40F01313	// MemConfig1
#define DMC0_TIMINGA_REF        0x00000618      // TimingAref   7.8us*133MHz=1038(0x40E), 100MHz=780(0x30C), 20MHz=156(0x9C), 10MHz=78(0x4E)
#define DMC0_TIMING_ROW         0x28233287      // TimingRow    for @200MHz
#define DMC0_TIMING_DATA        0x23240304      // TimingData   CL=3
#define DMC0_TIMING_PWR         0x09C80232      // TimingPower

#define	DMC1_MEMCONTROL		0x00202400	// MemControl	BL=4, 2 chip, DDR2 type, dynamic self refresh, force precharge, dynamic power down off
#define DMC1_MEMCONFIG_0	0x40F01313	// MemConfig0	512MB config, 8 banks,Mapping Method[12:15]0:linear, 1:linterleaved, 2:Mixed
#define DMC1_MEMCONFIG_1	0x00F01313	// MemConfig1
#define DMC1_TIMINGA_REF        0x00000618      // TimingAref   7.8us*133MHz=1038(0x40E), 100MHz=780(0x30C), 20MHz=156(0x9C), 10MHz=78(0x4E)
#define DMC1_TIMING_ROW         0x28233289      // TimingRow    for @200MHz
#define DMC1_TIMING_DATA        0x23240304      // TimingData   CL=3
#define DMC1_TIMING_PWR         0x08280232      // TimingPower
#if defined(SET_CLK_800_100_166_133) || defined(SET_CLK_400_100_166_133)
#define DMC0_MEMCONFIG_0	0x20E01323	// MemConfig0	256MB config, 8 banks,Mapping Method[12:15]0:linear, 1:linterleaved, 2:Mixed
#define DMC0_MEMCONFIG_1	0x40F01323	// MemConfig1
#define DMC0_TIMINGA_REF	0x0000030C	// TimingAref	7.8us*133MHz=1038(0x40E), 100MHz=780(0x30C), 20MHz=156(0x9C), 10MHz=78(0x4E)
#define DMC0_TIMING_ROW		0x28233287	// TimingRow	for @200MHz
#define DMC0_TIMING_DATA	0x23240304	// TimingData	CL=3
#define	DMC0_TIMING_PWR		0x09C80232	// TimingPower

#define	DMC1_MEMCONTROL		0x00202400	// MemControl	BL=4, 2 chip, DDR2 type, dynamic self refresh, force precharge, dynamic power down off
#define DMC1_MEMCONFIG_0	0x40C01323	// MemConfig0	512MB config, 8 banks,Mapping Method[12:15]0:linear, 1:linterleaved, 2:Mixed
#define DMC1_MEMCONFIG_1	0x00E01323	// MemConfig1
#define DMC1_TIMINGA_REF	0x0000030C	// TimingAref	7.8us*133MHz=1038(0x40E), 100MHz=780(0x30C), 20MHz=156(0x9C), 10MHz=78(0x4
#define DMC1_TIMING_ROW		0x28233289	// TimingRow	for @200MHz
#define DMC1_TIMING_DATA	0x23240304	// TimingData	CL=3
#define	DMC1_TIMING_PWR		0x08280232	// TimingPower
#endif

#else

#error "You should define memory type (AC type or H type)"

#endif

#else

#define DMC0_TIMINGA_REF	0x50e
#define DMC0_TIMING_ROW		0x14233287
#define DMC0_TIMING_DATA	0x12130005
#define	DMC0_TIMING_PWR		0x0E140222

#define DMC1_TIMINGA_REF	0x618
#define DMC1_TIMING_ROW		0x11344309
#define DMC1_TIMING_DATA	0x12130005
#define	DMC1_TIMING_PWR		0x0E190222

#endif


#if defined(SET_CLK_533_133_100_100)
#define UART_UBRDIV_VAL		26
#define UART_UDIVSLOT_VAL	0x0808
#else
#define UART_UBRDIV_VAL		34
#define UART_UDIVSLOT_VAL	0xDDDD
#endif

/* BL2开始扇区为49号扇区，大小为512KB，也就是1024个扇区 */
#define MOVI_BL2_POS 49
#define MOVI_BL2_BLKCNT ((512 * 1024) / 512)

/* #define DEBUG */

#endif	/* __CONFIG_H */
