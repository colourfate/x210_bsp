#define GPA0CON		0xE0200800
#define UCON0 		0xE2900804
#define ULCON0 		0xE2900800
#define UMCON0 		0xE290080C
#define UFCON0 		0xE2900808
#define UBRDIV0 	0xE2900828
#define UDIVSLOT0	0xE290082C
#define UTRSTAT0	0xE2900810
#define UTXH0		0xE2900820	
#define URXH0		0xE2900824	

#define rGPA0CON	(*(volatile unsigned int *)GPA0CON)
#define rUCON0		(*(volatile unsigned int *)UCON0)
#define rULCON0		(*(volatile unsigned int *)ULCON0)
#define rUMCON0		(*(volatile unsigned int *)UMCON0)
#define rUFCON0		(*(volatile unsigned int *)UFCON0)
#define rUBRDIV0	(*(volatile unsigned int *)UBRDIV0)
#define rUDIVSLOT0	(*(volatile unsigned int *)UDIVSLOT0)
#define rUTRSTAT0		(*(volatile unsigned int *)UTRSTAT0)
#define rUTXH0		(*(volatile unsigned int *)UTXH0)
#define rURXH0		(*(volatile unsigned int *)URXH0)

// 串口初始化程序
void uart_init(void)
{
	// 初始化Tx Rx对应的GPIO引脚
	rGPA0CON &= ~(0xff<<0);			// 把寄存器的bit0～7全部清零
	rGPA0CON |= 0x00000022;			// 0b0010, Rx Tx
	
	// 几个关键寄存器的设置
	rULCON0 = 0x3;
	rUCON0 = 0x5;
	rUMCON0 = 0;
	rUFCON0 = 0;
	
	// 波特率设置	DIV_VAL = (PCLK / (bps x 16))-1
	// PCLK_PSYS用66MHz算		余数0.8
	//rUBRDIV0 = 34;	
	//rUDIVSLOT0 = 0xdfdd;
	
	// PCLK_PSYS用66.7MHz算		余数0.18
	// DIV_VAL = (66700000/(115200*16)-1) = 35.18
	rUBRDIV0 = 35;
	// (rUDIVSLOT中的1的个数)/16=上一步计算的余数=0.18
	// (rUDIVSLOT中的1的个数 = 16*0.18= 2.88 = 3
	rUDIVSLOT0 = 0x0888;		// 3个1，查官方推荐表得到这个数字
}


// 串口发送程序，发送一个字节
void uart_putc(char c)
{                  	
	// 串口发送一个字符，其实就是把一个字节丢到发送缓冲区中去
	// 因为串口控制器发送1个字节的速度远远低于CPU的速度，所以CPU发送1个字节前必须
	// 确认串口控制器当前缓冲区是空的（意思就是串口已经发完了上一个字节）
	// 如果缓冲区非空则位为0，此时应该循环，直到位为1
	while (!(rUTRSTAT0 & (1<<1)));
	rUTXH0 = c;
}

// 串口接收程序，轮询方式，接收一个字节
char uart_getc(void)
{
	while (!(rUTRSTAT0 & (1<<0)));
	return (rURXH0 & 0x0f);
}
