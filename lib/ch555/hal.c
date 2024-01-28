/********************************** (C) COPYRIGHT *******************************
* File Name          : Debug.C
* Author             : WCH
* Version            : V1.4
* Date               : 2021/12/15
* Description        : This file contains all the functions prototypes for UART
*                      Printf , Delay functions.									 
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
********************************************************************************/
#include "hal.h"

/*******************************************************************************
*Function Name: CfgFsys
*Description  : CH555 clock selection and configuration function, reset uses internal crystal oscillator by default, Fsys=12MHz,
*               FREQ_SYS can be obtained through CLOCK_CFG configuration, the formula is as follows:
*               Fsys = (Fosc *4/(CLOCK_CFG & MASK_SYS_CK_SEL); The specific clock needs to be configured by yourself
*Input        : None
*Return       : None
*******************************************************************************/ 
void CfgFsys(void)  
{
#if OSC_EN_XT	
	P7 = P7 & 0xF0 | 0x06;                            //Before starting the external crystal oscillator, p7.0 is at low level and p7.1 is in pull-up state.
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	CLOCK_CFG |= bOSC_EN_XT;                          //Enable external crystal oscillator
	mDelaymS(10);
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	CLOCK_CFG &= ~bOSC_EN_INT;                        //Turn off the internal crystal oscillator 
	SAFE_MOD = 0x00;
#endif	
	
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
#if FREQ_SYS == 48000000	
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x07;  // 48MHz	
	
#endif	
	
#if FREQ_SYS == 32000000	
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x06;  // 32MHz	
	
#endif
	
#if FREQ_SYS == 24000000	
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x05;  // 24MHz	
	
#endif	

#if FREQ_SYS == 16000000		
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x04;  // 16MHz
	
#endif

#if FREQ_SYS == 12000000		
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x03;  // 12MHz
	
#endif	

#if FREQ_SYS == 3000000	
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x02;  // 3MHz
	
#endif

#if FREQ_SYS == 750000	
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x01;  // 750KHz
	
#endif

#if FREQ_SYS == 187500	
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x00;  // 187.5KHz	
	
#endif

	SAFE_MOD = 0x00;
}

/*******************************************************************************
* Function Name  : mDelayus
* Description    : microsecond delay function, delay in us
* Input          : n
* Return         : None
*******************************************************************************/ 
void mDelayuS(UINT16 n)  
{
#ifdef	FREQ_SYS
#if		FREQ_SYS <= 6000000
		n >>= 2;
#endif
#if		FREQ_SYS <= 3000000
		n >>= 2;
#endif
#if		FREQ_SYS <= 750000
		n >>= 4;
#endif
#endif
	while(n){  // total = 12~13 Fsys cycles, 1uS @Fsys=12MHz
		++ SAFE_MOD;  // 2 Fsys cycles, for higher Fsys, add operation here
#ifdef	FREQ_SYS
#if		FREQ_SYS >= 14000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 16000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 18000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 20000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 22000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 24000000
		++ SAFE_MOD;
#endif
#endif
		-- n;
	}
}

/*******************************************************************************
* Function Name  : mDelayms
* Description    : Ms delay function, delay in ms
* Input          : n
* Return         : None
*******************************************************************************/
void mDelaymS(UINT16 n)                                                    
{
	while(n){
		mDelayuS(1000);
		-- n;
	}
}                                         

#ifdef UART0_PRINTF
/*******************************************************************************
* Function Name  : CH555UART0Alter
* Description    : CH555 serial port 0 pin mapping, mapped to p0.2 and p0.3
* Input          : None
* Return         : None
*******************************************************************************/
void CH555UART0Alter(void)
{
	PIN_FUNC |= bUART0_PIN_X;                                                 
}

/*******************************************************************************
* Function Name  : mInitSTDIO
* Description    : Ch555 serial port 0 is initialized, 
*                  t1 is used as the baud rate generator by default, t2 can also be used
*                  -T1 acts as baud rate generator for uart0：RCLK=0 and TCLK=0
*                  -T2 acts as baud rate generator for uart0：RCLK=1 or TCLK=1               
* Input          : None
* Return         : None
*******************************************************************************/
void mInitSTDIO(void)
{
	UINT32 x;
	UINT8 x2; 

/*Serial port 0 usage mode 1*/
	SM0 = 0;
	SM1 = 1;
	SM2 = 0;                                                                 
	
/*Use Timer1 as baud rate generator*/
	RCLK = 0;                                                                  //Uart0 receive clock
	TCLK = 0;                                                                  //Uart0 send clock
	
	PCON |= SMOD;
	x = 10 * FREQ_SYS / UART0_BAUD / 16;                                       //If you change the main frequency, be careful not to overflow the value of x                      
	x2 = x % 10;
	x /= 10;
	if(x2 >= 5) x++;                                                           //rounding

	TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT & ~ MASK_T1_MOD | bT1_M1;              //0X20，Timer1 as 8-bit auto-reload timer
	T2MOD = T2MOD | bTMR_CLK | bT1_CLK;                                        //Timer1 clock selection
	TH1 = 0-x;                                                                 //12mhz crystal oscillator, buad/12 is the actual baud rate that needs to be set
	TR1 = 1;                                                                   //Start timer 1
	TI = 1;
	REN = 1;                                                                   //Serial port 0 receive enable
}

/*******************************************************************************
* Function Name  : CH555UART0RcvByte
* Description    : Ch555 uart0 receives a byte
* Input          : None
* Return         : SBUF
*******************************************************************************/
int getchar(void) {
	while (!RI);//Query reception, interrupt mode is not required
	RI = 0;
	return (int)SBUF;
}

/*******************************************************************************
* Function Name  : CH555UART0SendByte
* Description    : Ch555 uart0 sends a byte
* Input          : SendDat；data to send
* Return         : None
*******************************************************************************/
int putchar(int c) {
	while (!TI);//For query sending, the following two statements are not needed in the interrupt mode, but ti=0 is required before sending.
	TI = 0;
	SBUF = c & 0xff;
	return c;
}



#elif defined(UART1_PRINTF)
void CH555UART1Alter(void) {
    P1_MOD_OC |= (3<<6);
    P1_DIR_PU |= (3<<6);
	PIN_FUNC |= bUART1_PIN_X;
}
void mInitSTDIO(void) {
  SCON1 &= ~bU1SM0;
  SCON1 |= bU1REN | bU1SMOD;

  //SBAUD1 = 256 - F_CPU / 16 / UART1_BAUD;
  //SBAUD1 = 0xf9;//for 115200 baud at 12MHz clk
	UINT32 x;
	UINT8 x2; 
	x = 10 * FREQ_SYS / UART1_BAUD / 16;                                       //If you change the main frequency, be careful not to overflow the value of x                      
	x2 = x % 10;
	x /= 10;
	if(x2 >= 5) x++;                                                           //rounding
    SBAUD1 = 0 - x;

  SIF1 = bU1TI | bU1RI; // clear interrupt flags
  //IE_UART1 = 1;
  //EA = 1;
  CH555UART1Alter(); 
}

int getchar(void) {
	while (!(SIF1&bU1RI));//Query reception, interrupt mode is not required
	SIF1  = bU1RI;
	return (int)SBUF1;
}

int putchar(int c) {
	SBUF1 = c & 0xff;
	while (!(SIF1&bU1TI));//For query sending, the following two statements are not needed in the interrupt mode, but ti=0 is required before sending.
	SIF1  = bU1TI;
	return c;
}
#endif

/*******************************************************************************
* Function Name  : CH555WDTModeSelect
* Description    : CH555 Watchdog mode selection
* Input          : mode：
*                   0：timer
*                   1：watchDog
* Return         : None
*******************************************************************************/
void CH555WDTModeSelect(UINT8 mode)
{
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xaa;                                                             //Enter safe mode
	if(mode){
	 GLOBAL_CFG |= bWDOG_EN;                                                     //Start watchdog reset
	}
	else GLOBAL_CFG &= ~bWDOG_EN;	                                             //Start the watchdog only as a timer
	SAFE_MOD = 0x00;                                                             //exit safe mode
	WDOG_COUNT = 0;                                                              //Watchdog assigned initial value
}

/*******************************************************************************
* Function Name  : CH555WDTFeed
* Description    : CH555 Watchdog Feeding 
*                  Watchdog reset time (s) = (256-tim)/(Fsys/131072)
*                  00H(Fsys=12MHz)=2.8s
*                  80H(Fsys=12MHz)=1.4s
* Input          : tim：watchdog count assignment
* Return         : None
*******************************************************************************/
void CH555WDTFeed(UINT8 tim)
{
	WDOG_COUNT = tim;                                                             //Watchdog counter assignment
}

/*******************************************************************************
* Function Name  : CH555SoftReset
* Description    : CH555 soft reset
* Input          : None
* Return         : None
*******************************************************************************/
void CH555SoftReset(void)
{
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	GLOBAL_CFG |= bSW_RESET;
}

