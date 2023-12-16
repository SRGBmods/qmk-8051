/********************************** (C) COPYRIGHT *******************************
* File Name          : DEVICE.H
* Author             : MJX
* Version            : V1.00
* Date               : 2021/04/29
* Description        : CH55X Usb device operation related definitions
*******************************************************************************/



//#ifndef __DEVICE_H__
//#define __DEVICE_H__
#pragma once

//#ifdef __cplusplus
//extern "C" {
//#endif

/******************************************************************************/
/* The header file contains */
//#include "MAIN.H"	 															/* Main program definition related header files */
#include "ch555.h"
#include "usb.h"
#include "usb_descriptor.h"
//#include "ch555_usb_stack.h"
//#include "usb_desc.h"

/***********************************************************************************************************************/
/* Usb device related macro definitions */
#define bUEP_R_TOG        0x80      // expected data toggle flag of USB endpoint X receiving (OUT): 0=DATA0, 1=DATA1
#define bUEP_T_TOG        0x40      // prepared data toggle flag of USB endpoint X transmittal (IN): 0=DATA0, 1=DATA1
#define bUEP_R_RES1       0x08      // handshake response type high bit for USB endpoint X receiving (OUT)
#define bUEP_R_RES0       0x04      // handshake response type low bit for USB endpoint X receiving (OUT)
#define MASK_UEP_R_RES    0x0C      // bit mask of handshake response type for USB endpoint X receiving (OUT)
#define UEP_R_RES_ACK     0x00
#define UEP_R_RES_TOUT    0x04
#define UEP_R_RES_NAK     0x08
#define UEP_R_RES_STALL   0x0C
// bUEP_R_RES1 & bUEP_R_RES0: handshake response type for USB endpoint X receiving (OUT)
//   00: ACK (ready)
//   01: no response, time out to host, for non-zero endpoint isochronous transactions
//   10: NAK (busy)
//   11: STALL (error)

#define bUEP_T_RES1       0x02      // handshake response type high bit for USB endpoint X transmittal (IN)
#define bUEP_T_RES0       0x01      // handshake response type low bit for USB endpoint X transmittal (IN)
#define MASK_UEP_T_RES    0x03      // bit mask of handshake response type for USB endpoint X transmittal (IN)
#define UEP_T_RES_ACK     0x00
#define UEP_T_RES_TOUT    0x01
#define UEP_T_RES_NAK     0x02
#define UEP_T_RES_STALL   0x03
// bUEP_T_RES1 & bUEP_T_RES0: handshake response type for USB endpoint X transmittal (IN)
//   00: DATA0 or DATA1 then expecting ACK (ready)
//   01: DATA0 or DATA1 then expecting no response, time out from host, for non-zero endpoint isochronous transactions
//   10: NAK (busy)
//   11: STALL (error)


/************************************************/ 
/* Endpoint size and other definitions */
#define DEF_ENDP0_SIZE             FIXED_CONTROL_ENDPOINT_SIZE
#if KEYBOARD_IN_EPNUM==1 || RAW_IN_EPNUM==1 || SHARED_IN_EPNUM==1 || CONSOLE_IN_EPNUM==1 || MIDI_STREAM_IN_EPNUM==1 || CDC_IN_EPNUM==1 || JOYSTICK_IN_EPNUM==1 || DIGITIZER_IN_EPNUM==1
#define USE_D0_EP1_IN
//#define DEF_EP1_SIZE               DEFAULT_ENDP0_SIZE                           /* Keyboard endpoint packet size */
#endif
#if KEYBOARD_IN_EPNUM==2 || RAW_IN_EPNUM==2 || SHARED_IN_EPNUM==2 || CONSOLE_IN_EPNUM==2 || MIDI_STREAM_IN_EPNUM==2 || CDC_IN_EPNUM==2 || JOYSTICK_IN_EPNUM==2 || DIGITIZER_IN_EPNUM==2
#define USE_D0_EP2_IN
#endif
#if KEYBOARD_IN_EPNUM==3 || RAW_IN_EPNUM==3 || SHARED_IN_EPNUM==3 || CONSOLE_IN_EPNUM==3 || MIDI_STREAM_IN_EPNUM==3 || CDC_IN_EPNUM==3 || JOYSTICK_IN_EPNUM==3 || DIGITIZER_IN_EPNUM==3
#define USE_D0_EP3_IN
#endif
#if KEYBOARD_IN_EPNUM==5 || RAW_IN_EPNUM==5 || SHARED_IN_EPNUM==5 || CONSOLE_IN_EPNUM==5 || MIDI_STREAM_IN_EPNUM==5 || CDC_IN_EPNUM==5 || JOYSTICK_IN_EPNUM==5 || DIGITIZER_IN_EPNUM==5
#define USE_D0_EP5_IN
#endif
#if KEYBOARD_IN_EPNUM==6 || RAW_IN_EPNUM==6 || SHARED_IN_EPNUM==6 || CONSOLE_IN_EPNUM==6 || MIDI_STREAM_IN_EPNUM==6 || CDC_IN_EPNUM==6 || JOYSTICK_IN_EPNUM==6 || DIGITIZER_IN_EPNUM==6
#define USE_D0_EP6_IN
#endif
#if RAW_OUT_EPNUM==1 || CONSOLE_OUT_EPNUM==1 || MIDI_STREAM_OUT_EPNUM==1 || CDC_OUT_EPNUM==1 
#define USE_D0_EP1_OUT
#endif
#if RAW_OUT_EPNUM==2 || CONSOLE_OUT_EPNUM==2 || MIDI_STREAM_OUT_EPNUM==2 || CDC_OUT_EPNUM==2 
#define USE_D0_EP2_OUT
#endif
#if RAW_OUT_EPNUM==3 || CONSOLE_OUT_EPNUM==3 || MIDI_STREAM_OUT_EPNUM==3 || CDC_OUT_EPNUM==3 
#define USE_D0_EP3_OUT
#endif
#if RAW_OUT_EPNUM==4 || CONSOLE_OUT_EPNUM==4 || MIDI_STREAM_OUT_EPNUM==4 || CDC_OUT_EPNUM==4 
#define USE_D0_EP4_OUT
#endif

/************************************************/
/* Hid type request */ 
#define DEF_USB_GET_IDLE           0x02           								/* get idle for key or mouse */
#define DEF_USB_GET_PROTOCOL       0x03          								/* get protocol for bios type */
#define DEF_USB_SET_REPORT         0x09           								/* set report for key */
#define DEF_USB_SET_IDLE           0x0A           								/* set idle for key or mouse */
#define DEF_USB_SET_PROTOCOL       0x0B           								/* set protocol for bios type */

/*******************************************************************************/
/* Function enable related macro definitions */
//#define DEF_PACK_HEAD1		   	   0x57		  									/* Header byte 1 */
//#define DEF_PACK_HEAD2		       0xAB 										/* Header byte 2 */
//
//#define DEF_CMD_GET_RGBMODEL       0x01
//#define DEF_CMD_SET_RGBMODEL       0x02  
//#define DEF_CMD_GET_RGBDATA        0x03
//#define DEF_CMD_SET_RGBDATA        0x04     
//
//typedef struct				//Data bit alignment
//{
//	UINT8 first;		
//	UINT8 second;	    
//	UINT8 cmd1;		    
//	UINT8 cmd2;		    
//    UINT8 cou1;		
//	UINT8 cou2;		
//	UINT8 len1;		    
//	UINT8 len2;		
//    UINT8 dat[56];
//}HID_VALUE;  
//
//#define pHID_VALUE     ((HID_VALUE *)pUSB_BUF_DEV0+UX_EP2_ADDR)    
//void	InitUSB_Device( void );

/***********************************************************************************************************************/
/* variable expansion */
//extern UINT8X MyDevice_DevDesc[ ];												/* USB device descriptor */
//extern UINT8C MyDevice_CfgDesc[ ];												/* USB configuration descriptor */
//extern UINT8C MyDevice_KB_RP_Desc[ ];											/* USB keyboard report descriptor */
//extern UINT8C MyDevice_HID_RP_Desc[ ];											/* Usb custom hid report descriptor */
//extern UINT8C MyDevice_Media1_RP_Desc[ ];										/* Keyboard Multimedia 1 Report Descriptor */					
//extern UINT8C MyDevice_StringLangID[ ];											/* Language string descriptor */
//extern UINT8C MyDevice_StringProduct[ ];										/* product string descriptor */
//extern UINT8C MyDevice_StringSerial[ ];											/* Serial number string descriptor */

extern volatile UINT8  D0SetupReqCode;											/* USB Setup package request code */
extern volatile UINT16 D0SetupLen;												/* USB Setup packet length */
extern volatile PUINT8 pD0Descr;
extern volatile UINT8  D0UsbConfig;												/* USB configuration flags */

extern volatile UINT8  USB_EnumStatus;											/* USB enumeration status */	
extern volatile UINT8  USB_SleepStatus;											/* USB sleep state */	
extern volatile UINT8  MCU_Sleep_Operate;										/* Mcu sleep operation flag */	

extern volatile UINT8  KB_USB_UpStatus;											/*  */
extern volatile UINT8  KB_USB_SetReport;
extern volatile UINT8  ep3_data_wait;											/* endpoint 3 data waiting flag*/
/***********************************************************************************************************************/
/* Function expansion */
extern void USB_Device_Init( void );											/* CH55X USB device initialization */
extern void USB_WakeUp_PC( void );												/* CH55X Usb device wakes up computer */	
extern void CH55X_Sleep_Deal( void );											/* Ch55x microcontroller sleep processing */

extern void USB_DeviceInterrupt( void ) __interrupt(INT_NO_USB) __using(1);
//#ifdef __cplusplus
//}
//#endif

//#endif

/***********************************************************************************************************************/


