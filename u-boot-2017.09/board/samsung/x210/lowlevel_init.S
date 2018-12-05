/*
 * Memory Setup stuff - taken from blob memsetup.S
 *
 * Copyright (C) 2009 Samsung Electronics
 * Kyungmin Park <kyungmin.park@samsung.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <asm/arch/cpu.h>
#include <asm/arch/clock.h>
#include <asm/arch/power.h>
#include "s5pc110.h"

/*
 * Register usages:
 *
 * r5 has zero always
 * r7 has S5PC100 GPIO base, 0xE0300000
 * r8 has real GPIO base, 0xE0300000, 0xE0200000 at S5PC100, S5PC110 repectively
 * r9 has Mobile DDR size, 1 means 1GiB, 2 means 2GiB and so on
 */

	.globl lowlevel_init
lowlevel_init:
	mov	r11, lr
	
	// 检查是否需要重定位
	ldr	r0, =0x0000ffff
	bic	r1, pc, r0					// 实际载入地址（SRAM）的高16位存到r1
	ldr	r2, =CONFIG_SYS_TEXT_BASE
	bic	r2, r2, r0					// 链接地址的高16位存到r2
	cmp     r1, r2
	beq     after_copy
	
	/* r5 has always zero */
	mov	r5, #0

	ldr	r7, =S5PC100_GPIO_BASE
	ldr	r8, =S5PC100_GPIO_BASE
	/* Read CPU ID */
	ldr	r2, =S5PC110_PRO_ID
	ldr	r0, [r2]
	mov	r1, #0x00010000
	and	r0, r0, r1
	cmp	r0, r5
	beq	100f
	ldr	r8, =S5PC110_GPIO_BASE
100:
	/* Turn on KEY_LED_ON [GPJ4(1)] XMSMWEN */
	cmp	r7, r8
	beq	skip_check_didle			@ Support C110 only

	ldr	r0, =S5PC110_RST_STAT
	ldr	r1, [r0]
	and	r1, r1, #0x000D0000
	cmp	r1, #(0x1 << 19)			@ DEEPIDLE_WAKEUP
	beq	didle_wakeup
	cmp	r7, r8

skip_check_didle:	
	/* 关闭所有LED */
	addne	r0, r8, #0x240				@ S5PC110_GPIO_J0
	ldr	r1, [r0, #0x0]				@ GPIO_CON_OFFSET
	bic	r1, r1, #(0xf << 12)		@ 1 * 4-bit
	bic r1, r1, #(0xf << 16)
	bic r1, r1, #(0xf << 20)
	orr	r1, r1, #(0x1 << 12)		@ J0_3/4/5设置为输出
	orr r1, r1, #(0x1 << 16)
	orr r1, r1, #(0x1 << 20)
	str	r1, [r0, #0x0]				@ GPIO_CON_OFFSET
	
	/* GPJ0_3/4/5熄灭，表示0*/
	ldr	r1, [r0, #0x4]				@ GPIO_DAT_OFFSET
	orr	r1, r1, #(7 << 3)
	str	r1, [r0, #0x4]				@ GPIO_DAT_OFFSET

	/* 开发板供电锁存 */
	ldr r0, =(S5PC110_CLOCK_BASE + 0xE81C)
	ldr	r1, [r0]
	orr	r1, r1, #0x300	
	orr	r1, r1, #0x1	
	str	r1, [r0]

	/* Don't setup at s5pc100 */
	beq	100f

	// 001
	add r0, r8, #0x240
	ldr	r1, [r0, #0x4]				@ GPIO_DAT_OFFSET
	bic	r1, r1, #(1 << 5)
	str	r1, [r0, #0x4]				@ GPIO_DAT_OFFSET

	/*
	 * Initialize Async Register Setting for EVT1
	 * Because we are setting EVT1 as the default value of EVT0,
	 * setting EVT0 as well does not make things worse.
	 * Thus, for the simplicity, we set for EVT0, too
	 *
	 * The "Async Registers" are:
	 *	0xE0F0_0000
	 *	0xE1F0_0000
	 *	0xF180_0000
	 *	0xF190_0000
	 *	0xF1A0_0000
	 *	0xF1B0_0000
	 *	0xF1C0_0000
	 *	0xF1D0_0000
	 *	0xF1E0_0000
	 *	0xF1F0_0000
	 *	0xFAF0_0000
	 */
	ldr     r0, =0xe0f00000
	ldr     r1, [r0]
	bic     r1, r1, #0x1
	str     r1, [r0]

	ldr     r0, =0xe1f00000
	ldr     r1, [r0]
	bic     r1, r1, #0x1
	str     r1, [r0]

	ldr     r0, =0xf1800000
	ldr     r1, [r0]
	bic     r1, r1, #0x1
	str     r1, [r0]

	ldr     r0, =0xf1900000
	ldr     r1, [r0]
	bic     r1, r1, #0x1
	str     r1, [r0]

	ldr     r0, =0xf1a00000
	ldr     r1, [r0]
	bic     r1, r1, #0x1
	str     r1, [r0]

	ldr     r0, =0xf1b00000
	ldr     r1, [r0]
	bic     r1, r1, #0x1
	str     r1, [r0]

	ldr     r0, =0xf1c00000
	ldr     r1, [r0]
	bic     r1, r1, #0x1
	str     r1, [r0]

	ldr     r0, =0xf1d00000
	ldr     r1, [r0]
	bic     r1, r1, #0x1
	str     r1, [r0]

	ldr     r0, =0xf1e00000
	ldr     r1, [r0]
	bic     r1, r1, #0x1
	str     r1, [r0]

	ldr     r0, =0xf1f00000
	ldr     r1, [r0]
	bic     r1, r1, #0x1
	str     r1, [r0]

	ldr     r0, =0xfaf00000
	ldr     r1, [r0]
	bic     r1, r1, #0x1
	str     r1, [r0]

	/*
	 * Diable ABB block to reduce sleep current at low temperature
	 * Note that it's hidden register setup don't modify it
	 */
	ldr	r0, =0xE010C300
	ldr	r1, =0x00800000
	str	r1, [r0]

100:
		
	/* IO retension release */
	ldreq	r0, =S5PC100_OTHERS			@ 0xE0108200
	ldrne	r0, =S5PC110_OTHERS			@ 0xE010E000
	ldr	r1, [r0]
	ldreq	r2, =(1 << 31)				@ IO_RET_REL
	ldrne	r2, =((1 << 31) | (1 << 30) | (1 << 29) | (1 << 28))
	orr	r1, r1, r2
	/* Do not release retention here for S5PC110 */
	streq	r1, [r0]

	/* Disable Watchdog */
	ldreq	r0, =S5PC100_WATCHDOG_BASE		@ 0xEA200000
	ldrne	r0, =S5PC110_WATCHDOG_BASE		@ 0xE2700000
	str	r5, [r0]

	/* setting SRAM */
	ldreq	r0, =S5PC100_SROMC_BASE
	ldrne	r0, =S5PC110_SROMC_BASE
	ldr	r1, =0x9
	str	r1, [r0]

	/* S5PC100 has 3 groups of interrupt sources */
	ldreq	r0, =S5PC100_VIC0_BASE			@ 0xE4000000
	ldrne	r0, =S5PC110_VIC0_BASE			@ 0xF2000000
	add	r1, r0, #0x00100000
	add	r2, r0, #0x00200000

	/* Disable all interrupts (VIC0, VIC1 and VIC2) */
	mvn	r3, #0x0
	str	r3, [r0, #0x14]				@ INTENCLEAR
	str	r3, [r1, #0x14]				@ INTENCLEAR
	str	r3, [r2, #0x14]				@ INTENCLEAR

	/* Set all interrupts as IRQ */
	str	r5, [r0, #0xc]				@ INTSELECT
	str	r5, [r1, #0xc]				@ INTSELECT
	str	r5, [r2, #0xc]				@ INTSELECT

	/* Pending Interrupt Clear */
	str	r5, [r0, #0xf00]			@ INTADDRESS
	str	r5, [r1, #0xf00]			@ INTADDRESS
	str	r5, [r2, #0xf00]			@ INTADDRESS
	
	// 010
	add r0, r8, #0x240
	ldr	r1, [r0, #0x4]				@ GPIO_DAT_OFFSET
	bic	r1, r1, #(1 << 4)
	orr r1, r1, #(1 << 5)
	str	r1, [r0, #0x4]				@ GPIO_DAT_OFFSET

	bl system_clock_init

	// 011
	add r0, r8, #0x240
	ldr	r1, [r0, #0x4]				@ GPIO_DAT_OFFSET
	bic	r1, r1, #(1 << 5)
	str	r1, [r0, #0x4]				@ GPIO_DAT_OFFSET
	
	/* for UART */
	bl	uart_asm_init

	// 100
	add r0, r8, #0x240
	ldr	r1, [r0, #0x4]				@ GPIO_DAT_OFFSET
	orr	r1, r1, #(3 << 4)
	bic r1, r1, #(1 << 3)
	str	r1, [r0, #0x4]				@ GPIO_DAT_OFFSET
	
	bl	internal_ram_init

	// 101
	add r0, r8, #0x240
	ldr	r1, [r0, #0x4]				@ GPIO_DAT_OFFSET
	bic	r1, r1, #(1 << 5)
	str	r1, [r0, #0x4]				@ GPIO_DAT_OFFSET

	cmp	r7, r8
	/* Clear wakeup status register */
	ldreq	r0, =S5PC100_WAKEUP_STAT
	ldrne	r0, =S5PC110_WAKEUP_STAT
	ldr	r1, [r0]
	str	r1, [r0]

	/* IO retension release */
	ldreq	r0, =S5PC100_OTHERS			@ 0xE0108200
	ldrne	r0, =S5PC110_OTHERS			@ 0xE010E000
	ldr	r1, [r0]
	ldreq	r2, =(1 << 31)				@ IO_RET_REL
	ldrne	r2, =((1 << 31) | (1 << 30) | (1 << 29) | (1 << 28))
	orr	r1, r1, r2
	str	r1, [r0]

	// 110
	add r0, r8, #0x240
	ldr	r1, [r0, #0x4]				@ GPIO_DAT_OFFSET
	bic r1, r1, #(1 << 4)
	orr	r1, r1, #(1 << 5)
	str	r1, [r0, #0x4]				@ GPIO_DAT_OFFSET

	/* dram初始化 */
	bl	mem_ctrl_asm_init

	/* 设置栈在DRAM上，地址为：0x33E00000-12 */
	//ldr	sp, =0xD0036000 	/* setup temp stack pointer */
	ldr	sp, =0x33E00000
	sub	sp, sp, #12
	mov	fp, #0			/* no previous frame, so fp=0 */

	// 1，测试栈
	ldr	r0, =ELFIN_UART_CONSOLE_BASE
	ldr	r1, =0x4A4A4A4A
	push {r1}
	ldr r1, =0x4B4B4B4B
	pop {r1}
	str	r1, [r0, #UTXH_OFFSET]

	/* 从SD卡拷贝BL2到：0x33E00000 */
	bl movi_bl2_copy

	// 'B'
	ldr	r0, =ELFIN_UART_CONSOLE_BASE
	ldr	r1, =0x42424242
	str	r1, [r0, #UTXH_OFFSET]

	/* 完成重定位 */
	ldr pc, =_start

after_copy:

	/* 串口打印'K' */
	ldr	r0, =ELFIN_UART_CONSOLE_BASE
	ldr	r1, =0x4b4b4b4b
	str	r1, [r0, #UTXH_OFFSET]

	b	1f

didle_wakeup:
	/* Wait when APLL is locked */
	ldr	r0, =0xE0100100			@ S5PC110_APLL_CON
lockloop:
	ldr	r1, [r0]
	and	r1, r1, #(1 << 29)
	cmp	r1, #(1 << 29)
	bne	lockloop

	ldr	r0, =S5PC110_INFORM0
	ldr	r1, [r0]
	mov	pc, r1
	nop
	nop
	nop
	nop
	nop

1:
	mov	lr, r11
	mov	pc, lr

/*
	// 实际的载入地址
	ldr r0, =0xD0020000
	// 计算偏移量
	sub r0, r11, r0
	// 计算重定位后的返回地址
	ldr r1, =CONFIG_SYS_TEXT_BASE
	add lr, r1, r0
	// 重定位
	mov pc, lr
*/

/*
 * system_clock_init: Initialize core clock and bus clock.
 * void system_clock_init(void)
 */
system_clock_init:

	ldr	r0, =S5PC110_CLOCK_BASE	@0xE0100000

	/* Set Mux to FIN */
	ldr	r1, =0x0
	str	r1, [r0, #CLK_SRC0_OFFSET]	@0x200

	ldr	r1,	=APLL_LOCKTIME_VAL
	str	r1,	[r0, #APLL_LOCK_OFFSET]	@0x0

	/********lxg added*********************/
	ldr	r0, =S5PC110_CLOCK_BASE	@0xE0100000

	ldr	r1,	=MPLL_LOCKTIME_VAL @0xE10
	str	r1,	[r0, #MPLL_LOCK_OFFSET]
	/********end*********************/

	/* Disable PLL */
#if defined(SET_CHECK_MPLL_LOCK)
retryloop:
#endif
	ldr	r1, =0x0
	str	r1, [r0, #APLL_CON0_OFFSET]	@0x100
	ldr	r1, =0x0
	str	r1, [r0, #MPLL_CON_OFFSET]	@0x108

	ldr	r1, =0x0
	str	r1, [r0, #MPLL_CON_OFFSET]

	ldr   	r1, [r0, #CLK_DIV0_OFFSET]	@0x300
	ldr	r2, =CLK_DIV0_MASK @0x7FFFFFFF
	bic	r1, r1, r2

	ldr	r2, =CLK_DIV0_VAL
	orr	r1, r1, r2
	str	r1, [r0, #CLK_DIV0_OFFSET]

	ldr	r1, =APLL_VAL
	str	r1, [r0, #APLL_CON0_OFFSET]

	ldr	r1, =MPLL_VAL
	str	r1, [r0, #MPLL_CON_OFFSET]

	ldr	r1, =VPLL_VAL
	str	r1, [r0, #VPLL_CON_OFFSET]	@0x120

	/*******lxg added***********************/
	ldr	r1, =EPLL_VAL
	str	r1, [r0, #EPLL_CON_OFFSET]	@0x110

	/*******lxg added***********************/
	ldr   	r1, [r0, #CLK_DIV1_OFFSET]	@0x304
	ldr	r2, =CLK_DIV1_MASK
	bic	r1, r1, r2

	ldr	r2, =CLK_DIV1_VAL
	orr	r1, r1, r2
	str	r1, [r0, #CLK_DIV1_OFFSET]

	ldr   	r1, [r0, #CLK_DIV2_OFFSET]	@0x308
	ldr	r2, =CLK_DIV2_MASK
	bic	r1, r1, r2

	ldr	r2, =CLK_DIV2_VAL
	orr	r1, r1, r2
	str	r1, [r0, #CLK_DIV2_OFFSET]

	ldr   	r1, [r0, #CLK_DIV4_OFFSET]	@0x310
	ldr	r2, =CLK_DIV4_MASK
	bic	r1, r1, r2

	ldr	r2, =CLK_DIV4_VAL
	orr	r1, r1, r2
	str	r1, [r0, #CLK_DIV4_OFFSET]

	ldr   	r1, [r0, #CLK_DIV6_OFFSET]	@0x318
	ldr	r2, =CLK_DIV6_MASK
	bic	r1, r1, r2

	ldr	r2, =CLK_DIV6_VAL
	orr	r1, r1, r2
	str	r1, [r0, #CLK_DIV6_OFFSET]
	/*******end*****************/
	/*******end*****************/
#if defined(SET_EVT1)
	ldr	r1, =AFC_ON
	str	r1, [r0, #APLL_CON1_OFFSET]
#endif
	mov	r1, #0x10000
1:	subs	r1, r1, #1
	bne	1b

#if defined(SET_CHECK_MPLL_LOCK)
	/* MPLL software workaround */
	ldr	r1, [r0, #MPLL_CON_OFFSET]
	orr     r1, r1, #(1<<28)
	str	r1, [r0, #MPLL_CON_OFFSET]

	mov	r1, #0x100
1:	subs	r1, r1, #1
	bne	1b

	ldr	r1, [r0, #MPLL_CON_OFFSET]
	and	r1, r1, #(1<<29)
	cmp	r1, #(1<<29)
	bne 	retryloop

	/* H/W lock detect disable */
	ldr	r1, [r0, #MPLL_CON_OFFSET]
	bic     r1, r1, #(1<<28)
	str	r1, [r0, #MPLL_CON_OFFSET]
#endif

	ldr	r1, [r0, #CLK_SRC0_OFFSET] 	@0x200
	//ldr	r2, =0x10001111 //lxg changed.
   	ldr	r2, =0x00000111
	orr	r1, r1, r2
	str	r1, [r0, #CLK_SRC0_OFFSET]

	// added by terry 2012.12.4 for camera 
	ldr r1, [r0, #CLK_SRC1_OFFSET]	@0x204
	bic r1, r1, #(0xf<<12)
	orr r1, r1, #(0x1<<12) //0001 XusbXTI
	str r1, [r0, #CLK_SRC1_OFFSET]

#if defined(CONFIG_MCP_AC)

	/* CLK_SRC6[25:24] -> OneDRAM clock sel = MPLL */
	ldr	r1, [r0, #CLK_SRC6_OFFSET]
	bic	r1, r1, #(0x3<<24)
	orr	r1, r1, #0x01000000
	str	r1, [r0, #CLK_SRC6_OFFSET]

	/* CLK_DIV6[31:28] -> 4=1/5, 3=1/4(166MHZ@667MHz), 2=1/3 */
	ldr	r1, [r0, #CLK_DIV6_OFFSET]
	bic	r1, r1, #(0xF<<28)
	bic	r1, r1, #(0x7<<12)	@; ONENAND_RATIO: 0
	orr	r1, r1, #0x30000000
	str	r1, [r0, #CLK_DIV6_OFFSET]

#elif defined (CONFIG_MCP_H)

	/* CLK_SRC6[25:24] -> OneDRAM clock sel = 00:SCLKA2M, 01:SCLKMPLL */
	ldr	r1, [r0, #CLK_SRC6_OFFSET]
	bic	r1, r1, #(0x3<<24)
	orr	r1, r1, #0x00000000
	str	r1, [r0, #CLK_SRC6_OFFSET]

	/* CLK_DIV6[31:28] -> 4=1/5, 3=1/4(166MHZ@667MHz), 2=1/3 */
	ldr	r1, [r0, #CLK_DIV6_OFFSET]
	bic	r1, r1, #(0xF<<28)
	bic	r1, r1, #(0x7<<12)	@; ONENAND_RATIO: 0
	orr	r1, r1, #0x00000000
	str	r1, [r0, #CLK_DIV6_OFFSET]	

#elif defined (CONFIG_MCP_B) || defined (CONFIG_MCP_D)

	/* CLK_SRC6[25:24] -> OneDRAM clock sel = 00:SCLKA2M, 01:SCLKMPLL */
	ldr	r1, [r0, #CLK_SRC6_OFFSET]
	bic	r1, r1, #(0x3<<24)
	orr	r1, r1, #0x01000000
	str	r1, [r0, #CLK_SRC6_OFFSET]

	/* CLK_DIV6[31:28] -> 4=1/5, 3=1/4(166MHZ@667MHz), 2=1/3 */
	ldr	r1, [r0, #CLK_DIV6_OFFSET]
	bic	r1, r1, #(0xF<<28)
	bic	r1, r1, #(0x7<<12)	@; ONENAND_RATIO: 0
	orr	r1, r1, #0x30000000
	str	r1, [r0, #CLK_DIV6_OFFSET]

#elif defined (SET_MCP_SINGLE)

	/* CLK_DIV6 */
	/*ldr	r1, [r0, #CLK_DIV6_OFFSET]
	bic	r1, r1, #(0x7<<12)	@; ONENAND_RATIO: 0
	str	r1, [r0, #CLK_DIV6_OFFSET]*/ //lxg mask

#endif	

	mov	pc, lr


internal_ram_init:
	/* 增加这一句，以防前面修改了标志位 */
	cmp	r7, r8
	
	ldreq	r0, =0xE3800000
	ldrne	r0, =0xF1500000
	ldr	r1, =0x0
	str	r1, [r0]

	mov	pc, lr

/*
 * uart_asm_init: Initialize UART's pins
 */
uart_asm_init:
	/* set GPIO to enable UART0-UART4 */
	mov	r0, r8
	ldr	r1, =0x22222222
	str	r1, [r0, #0x0]			@ S5PC100_GPIO_A0_OFFSET
	ldr	r1, =0x00002222
	str	r1, [r0, #0x20]			@ S5PC100_GPIO_A1_OFFSET

	/* Check S5PC100 */
	cmp	r7, r8
	bne	110f

	/* UART_SEL GPK0[5] at S5PC100 */
	add	r0, r8, #0x2A0			@ S5PC100_GPIO_K0_OFFSET
	ldr	r1, [r0, #0x0]			@ S5PC1XX_GPIO_CON_OFFSET
	bic	r1, r1, #(0xf << 20)		@ 20 = 5 * 4-bit
	orr	r1, r1, #(0x1 << 20)		@ Output
	str	r1, [r0, #0x0]			@ S5PC1XX_GPIO_CON_OFFSET

	ldr	r1, [r0, #0x8]			@ S5PC1XX_GPIO_PULL_OFFSET
	bic	r1, r1, #(0x3 << 10)		@ 10 = 5 * 2-bit
	orr	r1, r1, #(0x2 << 10)		@ Pull-up enabled
	str	r1, [r0, #0x8]			@ S5PC1XX_GPIO_PULL_OFFSET

	ldr	r1, [r0, #0x4]			@ S5PC1XX_GPIO_DAT_OFFSET
	orr	r1, r1, #(1 << 5)		@ 5 = 5 * 1-bit
	str	r1, [r0, #0x4]			@ S5PC1XX_GPIO_DAT_OFFSET

	b	200f
110:
	/* 配置UART2，波特率115200，完成后输出'O' */	
	ldr	r0, =ELFIN_UART_CONSOLE_BASE		@0xE2900800
	mov	r1, #0x0
	str	r1, [r0, #UFCON_OFFSET]
	str	r1, [r0, #UMCON_OFFSET]

	mov	r1, #0x3
	str	r1, [r0, #ULCON_OFFSET]

	ldr	r1, =0x3c5				@0b0011_1100_0101
	str	r1, [r0, #UCON_OFFSET]

	ldr	r1, =UART_UBRDIV_VAL
	str	r1, [r0, #UBRDIV_OFFSET]

	ldr	r1, =UART_UDIVSLOT_VAL
	str	r1, [r0, #UDIVSLOT_OFFSET]

	ldr	r1, =0x4f4f4f4f
	str	r1, [r0, #UTXH_OFFSET]		@'O'
200:
	mov	pc, lr
