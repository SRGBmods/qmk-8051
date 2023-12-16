/********************************** (C) COPYRIGHT *******************************
* File Name          : USB_DEVICE.C
* Author             : MJX
* Version            : V1.00
* Date               : 2021/04/29
* Description        : Usb device underlying operation related functions
*******************************************************************************/



/******************************************************************************/
#include <string.h>
#include "hal.h"
#include "debug.h"
#include "usb_device.h"
//#include "usb_descriptor.h"



/******************************************************************************/
/* constant variable definition */
volatile UINT8	D0SetupReqCode = 0xFF;											/* USB Setup package request code */
volatile UINT16	D0SetupLen = 0x00;												/* USB Setup packet length */
volatile PUINT8	pD0Descr;

volatile UINT8  D0UsbConfig = 0x00;												/* USB configuration flags - the Configuation Id selected */
volatile UINT8  Report_Value = 0x00;                                            /* hid interface related */
volatile UINT8  Idle_Value = 0x00;                                              /* host request hid interface go idle */
volatile UINT8  KB_USB_SetReport = 0x00;										/* */

volatile UINT8  USB_EnumStatus = 0x00;											/* USB enumeration status */	
volatile UINT8  USB_SleepStatus = 0x00;											/* USB sleep state */		
volatile UINT8  MCU_Sleep_Operate = 0x00;										/* Mcu sleep operation flag */
   

volatile UINT8  KB_USB_UpStatus = 0x00;											/* endpoint 1 busy flag*/
volatile UINT8  ep3_data_wait   = 0x00;											/* endpoint 3 data waiting flag*/

//UINT8D  RGB_Mode = 0x00;
void USB_EP_init( void )  
{
    D0_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
    D0_EP_MOD = bUX_DEV_EN;  
#ifdef USE_D0_EP1_IN
    D0_EP1RES = UEP_X_RES_NAK;
#endif
#ifdef USE_D0_EP2_IN
    D0_EP2RES = UEP_X_RES_NAK;
#endif
#ifdef USE_D0_EP3_IN
    D0_EP3RES = UEP_X_RES_NAK; 
#endif
#ifdef USE_D0_EP5_IN
	D0_EP_MOD |= bUX_EP5I_EN;
    D0_EP5RES = UEP_X_RES_NAK; 
#endif
#ifdef USE_D0_EP6_IN
	D0_EP_MOD |= bUX_EP6I_EN;
    D0_EP6RES = UEP_X_RES_NAK; 
#endif

#ifdef USE_D0_EP1_OUT
    D0_EP_MOD |= bUX_EP1O_EN;
    D0_EP1RES = UEP_X_RES_ACK; 
#endif
#ifdef USE_D0_EP2_OUT
    D0_EP_MOD |= bUX_EP2O_EN;
    D0_EP2RES = UEP_X_RES_ACK; 
#endif
#ifdef USE_D0_EP3_OUT
    D0_EP_MOD |= bUX_EP3O_EN;
    D0_EP3RES = UEP_X_RES_ACK; 
#endif
#ifdef USE_D0_EP4_OUT
    D0_EP_MOD |= bUX_EP4O_EN;
    D0_EP4RES = UEP_X_RES_ACK; 
#endif
}
/*******************************************************************************
* Function Name  : USB_Device_Init
* Description    : CH55X USB device initialization
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void USB_Device_Init( void )  
{
	/* Initialize related variables */
	D0SetupReqCode = 0xFF;														/* USB Setup package request code */
	D0SetupLen = 0x00;															/* USB Setup packet length */
	D0UsbConfig = 0x00;															/* USB configuration flags */
	USB_EnumStatus = 0x00;														/* USB enumeration status */	
	USB_SleepStatus = 0x00;														/* USB sleep state */		
	MCU_Sleep_Operate = 0x00;													/* Mcu sleep operation flag */
	KB_USB_UpStatus = 0x00;														/* Usb keyboard and mouse upload status */
	ep3_data_wait   = 0x00;														/* Usb keyboard and mouse upload status */
	KB_USB_SetReport = 0x00;													/* Usb keyboard set report value */  

	/* Initialize usb related registers */
	IE_USB = 0;                              // usb interrupt dissable, avoid acidental interrupt during init
	IP_EX &= ~bIP_USB;                       // set usb interrupt priority to high
    
	//SAFE_MOD = 0x55;
	//SAFE_MOD = 0xAA;
	//GLOBAL_CFG |= bXIR_XSFR;                 // use __pdata to access xSFR instead of pRAM        

	USB_CTRL = 0;                            // usb physical config

    USB_EP_init();                           // setup endpoints
//	D0_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
//	D0_EP1RES = bUEP_X_AUTO_TOG | UEP_X_RES_NAK;
//	D0_EP2RES = bUEP_X_AUTO_TOG | UEP_X_RES_ACK;
//	D0_EP3RES = bUEP_X_AUTO_TOG | UEP_X_RES_NAK;
	//D0_EP_MOD = bUX_EP3O_EN | bUX_DEV_EN; 
	//D0_EP_MOD |= bUX_EP5I_EN;

	D0_ADDR = 0;
	HB_ADDR = 0x7F;             												/* Set this address to forward the received data directly to d0 */
	USB_IF = 0xFF;
	USB_IE = bUX_IE_SUSPEND | bUX_IE_TRANSFER | bUX_IE_BUS_RST;
   	USB_CTRL = bUX_DP_PU_EN;                 // usb physical config
	IE_USB = 1;                              // usb interrupt enable
}

/*******************************************************************************
* Function Name  : USB_DeviceInterrupt
* Description    : CH55X Usb device interrupt handler function
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
#pragma save
#pragma nooverlay
void USB_DeviceInterrupt( void ) __interrupt(INT_NO_USB) __using(1)
{
    __data UINT8  us,i;
	__data UINT16 len;

USB_DevIntNext:
	us = USB_IF; 
	if( us & bUX_IF_D0_TRANS )            
	{
		//printf("%x\n",SP);
		//dprint("d0tr");
		/* D0 data transfer completed */
		switch( D0_STATUS & ( bUXS_SETUP_ACT | MASK_UXS_TOKEN | MASK_UXS_ENDP ) )     
		{
			/* Analyze action tokens and endpoint numbers */
			case UXS_TOKEN_IN | 0:                       
			case bUXS_SETUP_ACT | UXS_TOKEN_IN | 0:
            	/* endpoint 0# IN */
				switch( D0SetupReqCode ) 
				{
					case USB_GET_DESCRIPTOR:
						len = D0SetupLen >= DEF_ENDP0_SIZE ? DEF_ENDP0_SIZE : D0SetupLen;  /* The length of this transmission */
						memcpy( pD0_EP0_BUF, pD0Descr, len );  					/* Load upload data */
						D0SetupLen -= len;
						pD0Descr += len;
						D0_EP0T_L = len;
						D0_EP0RES ^= bUEP_T_TOG;  								/* flip */
						break;
						
					case USB_SET_ADDRESS:
						D0_ADDR = D0SetupLen;
						D0_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
						break;

					case USB_SET_FEATURE:
						break;

					default:
						/* The status phase is completed and interrupted or the 0-length data packet is forced to be uploaded to end the control transmission. */
						D0_EP0T_L = 0;  
						D0_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
						break;
				}
                break;

#ifdef USE_D0_EP4_OUT
			case UXS_TOKEN_OUT | 4:  
			case bUXS_SETUP_ACT | UXS_TOKEN_OUT | 4:
				/*  endpoint 4# download */
				if( D0_STATUS & bUXS_TOG_OK ) 
				D0_EP4RES = D0_EP4RES & ~MASK_UEP_X_RES | UEP_X_RES_NAK; 		/* Pause download */
				ep4_data_wait = 0x01;
                break; 
#endif
#ifdef USE_D0_EP3_OUT
			case UXS_TOKEN_OUT | 3:  
			case bUXS_SETUP_ACT | UXS_TOKEN_OUT | 3:
				/*  endpoint 3# download */
				if( D0_STATUS & bUXS_TOG_OK ) 
				D0_EP3RES = D0_EP3RES & ~MASK_UEP_X_RES | UEP_X_RES_NAK; 		/* Pause download */
				ep3_data_wait = 0x01;
                break; 
#endif
#ifdef USE_D0_EP2_OUT
			case UXS_TOKEN_OUT | 2:  
			case bUXS_SETUP_ACT | UXS_TOKEN_OUT | 2:
				/*  endpoint 2# download */
				if( D0_STATUS & bUXS_TOG_OK ) 
				D0_EP2RES = D0_EP2RES & ~MASK_UEP_X_RES | UEP_X_RES_NAK; 		/* Pause download */
				ep2_data_wait = 0x01;
                break; 
#endif
#ifdef USE_D0_EP1_OUT
			case UXS_TOKEN_OUT | 1:  
			case bUXS_SETUP_ACT | UXS_TOKEN_OUT | 1:
				/*  endpoint 1# download */
				if( D0_STATUS & bUXS_TOG_OK ) 
				D0_EP1RES = D0_EP1RES & ~MASK_UEP_X_RES | UEP_X_RES_NAK; 		/* Pause download */
				ep1_data_wait = 0x01;
                break; 
#endif
                          
                          
                          
                          
#ifdef USE_D0_EP6_IN
            case UXS_TOKEN_IN | 6:  
            case bUXS_SETUP_ACT | UXS_TOKEN_IN | 6:
				/* endpoint 6# upload completed */            	
				D0_EP6RES = D0_EP6RES & ~MASK_UEP_X_RES | UEP_X_RES_NAK; 		/* Pause upload */
                break;
#endif 

#ifdef USE_D0_EP5_IN
            case UXS_TOKEN_IN | 5:  
            case bUXS_SETUP_ACT | UXS_TOKEN_IN | 5:
				/* endpoint 5# upload completed */            	
				D0_EP5RES = D0_EP5RES & ~MASK_UEP_X_RES | UEP_X_RES_NAK; 		/* Pause upload */
                break;
#endif 

#ifdef USE_D0_EP3_IN
            case UXS_TOKEN_IN | 3:  
            case bUXS_SETUP_ACT | UXS_TOKEN_IN | 3:
				/* endpoint 3# upload completed */            	
				D0_EP3RES = D0_EP3RES & ~MASK_UEP_X_RES | UEP_X_RES_NAK; 		/* Pause upload */
                break;
#endif
                
#ifdef USE_D0_EP2_IN
            case UXS_TOKEN_IN | 2:  
            case bUXS_SETUP_ACT | UXS_TOKEN_IN | 2:
				/* endpoint 2# upload completed */            	
				D0_EP2RES = D0_EP2RES & ~MASK_UEP_X_RES | UEP_X_RES_NAK; 		/* Pause upload */
                break;
#endif
                
#ifdef USE_D0_EP1_IN
            case UXS_TOKEN_IN | 1:  
            case bUXS_SETUP_ACT | UXS_TOKEN_IN | 1:
            	/* endpoint 1# upload completed */
                D0_EP1RES = D0_EP1RES & ~MASK_UEP_X_RES | UEP_X_RES_NAK; 		/* Pause upload */
				KB_USB_UpStatus = 0x01;
                break;
#endif
               
			case UXS_TOKEN_OUT | 0:  
            case bUXS_SETUP_ACT | UXS_TOKEN_OUT | 0:
 				/* endpoint 0# OUT */
				switch( D0SetupReqCode ) 
				{
					case HID_SET_REPORT: 
						KB_USB_SetReport = pD0_EP0_BUF[ 0 ];
						D0_EP0RES ^= bUEP_R_TOG;                      
                        break;
                        
					case USB_GET_DESCRIPTOR:
					
					default:
						D0_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;  
						break;
				}
                break;
			
			default:
//	        case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 0:    
//          case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 1:    
//          case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 2:    
//          case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 3:    
//          case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 4:    
//          case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 5:    
//          case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 6:    
//          case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 7:  
				if(( D0_STATUS & ( bUXS_SETUP_ACT | MASK_UXS_TOKEN ) ) == ( bUXS_SETUP_ACT | UXS_TOKEN_FREE ))
				{
					/* endpoint 0# SETUP */
					D0_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;

					len = 0;  														/* Defaults to success and uploading 0 length */               
					D0SetupLen = pD0_SETUP_REQ->wLengthL + ( (UINT16)pD0_SETUP_REQ->wLengthH << 8 );
					D0SetupReqCode = pD0_SETUP_REQ->bRequest;
					
					/* Analyze the current setup package */				                
					if( ( pD0_SETUP_REQ->bRequestType & USB_REQ_TYP_MASK ) == USB_REQ_TYP_STANDARD )
					{ 
						/* Standard request */                                   
						switch( D0SetupReqCode ) 
						{  
							case USB_GET_DESCRIPTOR:            
								// Get usb descriptor 
								
								switch( pD0_SETUP_REQ->wValueH )
								{
									case USB_DESCR_TYP_DEVICE:  
										// USB device descriptor 
										pD0Descr = (PUINT8)( &DeviceDescriptor );
										len = sizeof( USB_Descriptor_Device_t );
										break;
										
									case USB_DESCR_TYP_CONFIG:  
										// USB configuration descriptor 
										pD0Descr = (PUINT8)( &ConfigurationDescriptor );
										len = sizeof( USB_Descriptor_Configuration_t );
										break;
										
									case USB_DESCR_TYP_STRING:  
										// USB string descriptor 
										switch( pD0_SETUP_REQ->wValueL ) 
										{
											case 1:
												pD0Descr = (PUINT8)( &ManufacturerString );
												len = ManufacturerString.Header.Size;
												break;
																			
											case 2:
												pD0Descr = (PUINT8)( &ProductString );
												len = ProductString.Header.Size;
												break;
												
											case 0:
												pD0Descr = (PUINT8)( &LanguageString );
												len = LanguageString.Header.Size;
												break;
											#if defined(SERIAL_NUMBER)
											case 3:
												pD0Descr = (PUINT8)( &SerialNumberString );
												len = SerialNumberString.Header.Size;
												break;
											#endif
												
											default:
												// Unsupported string descriptor 
												len = 0xFFFF;  
											break;
										}
										break;

									case USB_DESCR_TYP_HID:
										switch( pD0_SETUP_REQ->wIndexL )
										{       
											// Select interface 
											//case 0:
#ifndef KEYBOARD_SHARED_EP
											case KEYBOARD_INTERFACE:
												pD0Descr = (PUINT8)( &ConfigurationDescriptor.Keyboard_HID );        
												len = 9;
												break;										
#endif
											
#if defined(MOUSE_ENABLE) && !defined(MOUSE_SHARED_EP)
                							case MOUSE_INTERFACE:
                    							pD0Descr = &ConfigurationDescriptor.Mouse_HID;
                    							len    = sizeof(USB_HID_Descriptor_HID_t);
                    							break;
#endif

											//case 2:
#ifdef SHARED_EP_ENABLE
											case SHARED_INTERFACE:
												pD0Descr = (PUINT8)( &ConfigurationDescriptor.Shared_HID );        
												len = 9;
												break;			
#endif
												
											//case 1:
#ifdef RAW_ENABLE
											case RAW_INTERFACE:
												pD0Descr = (PUINT8)( &ConfigurationDescriptor.Raw_HID );        
												len = 9;
												break;			
#endif
												
#ifdef CONSOLE_ENABLE
                case CONSOLE_INTERFACE:
                    pD0Descr = &ConfigurationDescriptor.Console_HID;
                    len    = sizeof(USB_HID_Descriptor_HID_t);
                    break;
#endif
#if defined(JOYSTICK_ENABLE) && !defined(JOYSTICK_SHARED_EP)
                case JOYSTICK_INTERFACE:
                    pD0Descr = &ConfigurationDescriptor.Joystick_HID;
                    len    = sizeof(USB_HID_Descriptor_HID_t);
                    break;
#endif
#if defined(DIGITIZER_ENABLE) && !defined(DIGITIZER_SHARED_EP)
                case DIGITIZER_INTERFACE:
                    pD0Descr = &ConfigurationDescriptor.Digitizer_HID;
                    len    = sizeof(USB_HID_Descriptor_HID_t);
                    break;
#endif
											default:
												// Unsupported string descriptor 
												len = 0xFFFF;  
												break;
										}
										break;

									case USB_DESCR_TYP_REPORT:  
										// Hid report descriptor 
										switch( pD0_SETUP_REQ->wIndexL )
										{       
											// Select interface 
											//case 0:
#ifndef KEYBOARD_SHARED_EP
											case KEYBOARD_INTERFACE:
												pD0Descr = (PUINT8)( KeyboardReport );        
												len = KeyboardReport_size;
												break;										
#endif
											
#if defined(MOUSE_ENABLE) && !defined(MOUSE_SHARED_EP)
                                            case MOUSE_INTERFACE:
                                                pD0Descr = &MouseReport;
                                                len    = MouseReport_size;
                                                break;
#endif

											//case 2:
#ifdef SHARED_EP_ENABLE
											case SHARED_INTERFACE:
												pD0Descr = (PUINT8)( SharedReport );        
												len = SharedReport_size;
												break;			
#endif
												
											//case 1:
#ifdef RAW_ENABLE
											case RAW_INTERFACE:
												pD0Descr = (PUINT8)( RawReport );        
												len = RawReport_size;
												break;			
#endif
												
#ifdef CONSOLE_ENABLE
                                            case CONSOLE_INTERFACE:
                                                pD0Descr = &ConsoleReport;
                                                len    = ConsoleReport_size;
                                                break;
#endif
#if defined(JOYSTICK_ENABLE) && !defined(JOYSTICK_SHARED_EP)
                                            case JOYSTICK_INTERFACE:
                                                pD0Descr = &JoystickReport;
                                                len    = JoystickReport_size;
                                                break;
#endif
#if defined(DIGITIZER_ENABLE) && !defined(DIGITIZER_SHARED_EP)
                                            case DIGITIZER_INTERFACE:
                                                pD0Descr = &DigitizerReport;
                                                len    = DigitizerReport_size;
                                                break;
#endif
											default:
												// Unsupported string descriptor 
												len = 0xFFFF;  
												break;
										}
										break;
										
									//case 0x06:
									//	pD0Descr = (PUINT8)( &My_QueDescr[ 0 ] );    								   
									//	len = sizeof( My_QueDescr );
									//	break;
									//		
									//case 0x07:	
									//	memcpy( &USB_FS_OSC_DESC[ 2 ], &MyDevice_CfgDesc[ 2 ], sizeof( MyDevice_CfgDesc ) - 2 );
									//	pD0Descr = (PUINT8)( &USB_FS_OSC_DESC[ 0 ] );    
									//	len = sizeof( USB_FS_OSC_DESC );
									//	break;
									//	
									default:
										// Unsupported string descriptor 
										len = 0xFFFF; 
										break;
								} 
								//USB_Request_Header_t *pSetupReq;
								//pSetupReq = (USB_Request_Header_t *)pD0_EP0_BUF;
								//#define pSetupReq ((USB_Request_Header_t __xdata *)XSFR_USBD0_BASE)
								//#define pSetupReq ((PXUSB_SETUP_REQ2)XSFR_USBD0_BASE)
								//PXUSB_SETUP_REQ2 pSetupReq = XSFR_USB_BASE;
								//USB_Request_Header_t *pSetupReq = XSFR_USB_BASE;
								//len = get_usb_descriptor(pSetupReq->wValue,pSetupReq->wIndex,pSetupReq->wLength,(void *)(&pD0Descr));
								//if (len==0) len = 0xffff;

								// Determine whether it can be processed normally						
								if( len != 0xFFFF )
								{
									if( D0SetupLen > len ) 
									{	
										D0SetupLen = len;  							/* Limit total length */
									}
									len = D0SetupLen >= DEF_ENDP0_SIZE ? DEF_ENDP0_SIZE : D0SetupLen;  /* The length of this transmission */
									D0SetupLen -= len;
									memcpy( pD0_EP0_BUF, pD0Descr, len );  			/* Load upload data */
									pD0Descr += len;
								}
								break;
								
							case USB_SET_ADDRESS:               
								/* USB device address setting */
								D0SetupLen = pD0_SETUP_REQ->wValueL;  				/* Temporarily store USB device address */
								break;
								
							case USB_GET_CONFIGURATION:
								pD0_EP0_BUF[ 0 ] = D0UsbConfig;
								if( D0SetupLen >= 1 ) 
								{
									len = 1;
								}
								break; 
														
							case USB_SET_CONFIGURATION:
								D0UsbConfig = pD0_SETUP_REQ->wValueL;
								USB_EnumStatus = 0x01;
								KB_USB_UpStatus = 0x01;
								break;			
																
							case USB_SET_FEATURE:
								if( ( pD0_SETUP_REQ->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP ) 
								{  
									/* endpoint */
									switch( pD0_SETUP_REQ->wIndexL ) 
									{
#ifdef USE_D0_EP6_IN
										case 0x86:
										//case 0x06:
											D0_EP6RES = D0_EP6RES & ~( MASK_UEP_X_RES ) | UEP_X_RES_STALL;
											break;
#endif
											
#ifdef USE_D0_EP5_IN
										case 0x85:
										//case 0x05:
											D0_EP5RES = D0_EP5RES & ~( MASK_UEP_X_RES ) | UEP_X_RES_STALL;
											break;
#endif
											
										//case 0x84:
#ifdef USE_D0_EP4_OUT
										case 0x04:
											D0_EP4RES = D0_EP4RES & ~( MASK_UEP_X_RES ) | UEP_X_RES_STALL;
											break;
#endif
											
#ifdef USE_D0_EP3_IN
										case 0x83:
#endif
#ifdef USE_D0_EP3_OUT
										case 0x03:
#endif
#if defined(USE_D0_EP3_IN) || defined(USE_D0_EP3_OUT)
											D0_EP3RES = D0_EP3RES & ~( MASK_UEP_X_RES ) | UEP_X_RES_STALL;
											break;
#endif
											
#ifdef USE_D0_EP2_IN
										case 0x82:
#endif
#ifdef USE_D0_EP2_OUT
										case 0x02:
#endif
#if defined(USE_D0_EP2_IN) || defined(USE_D0_EP2_OUT)
											D0_EP2RES = D0_EP2RES & ~( MASK_UEP_X_RES ) | UEP_X_RES_STALL;
											break;
#endif
											
#ifdef USE_D0_EP1_IN
										case 0x81:
#endif
#ifdef USE_D0_EP1_OUT
										case 0x01:
#endif
#if defined(USE_D0_EP1_IN) || defined(USE_D0_EP1_OUT)
											D0_EP1RES = D0_EP1RES & ~( MASK_UEP_X_RES ) | UEP_X_RES_STALL;
											break;
#endif
											
										default:
											/* Unsupported endpoint */
											len = 0xFFFF;  
											break;
									}
								}
								else if( ( pD0_SETUP_REQ->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_DEVICE )
								{
									if( pD0_SETUP_REQ->wValueL == 1 )
									{ 
										/* Set sleep */
										USB_SleepStatus |= 0x01; 
									}		
								}
								else
								{
									len = 0xFFFF;
								}															
								break;
													
							case USB_SET_INTERFACE:
								break;
								
							case USB_CLEAR_FEATURE:
								if( ( pD0_SETUP_REQ->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP ) 
								{  
									/* endpoint */
									switch( pD0_SETUP_REQ->wIndexL ) 
									{
#ifdef USE_D0_EP6_IN
										case 0x86:
											D0_EP6RES = D0_EP6RES & ~(  MASK_UEP_X_RES ) | UEP_X_RES_NAK;
											break;
#endif
											
#ifdef USE_D0_EP5_IN
										case 0x85:
											D0_EP5RES = D0_EP5RES & ~(  MASK_UEP_X_RES ) | UEP_X_RES_NAK;
											break;
#endif
											
#ifdef USE_D0_EP4_OUT
										case 0x04:
											D0_EP4RES = D0_EP4RES & ~(  MASK_UEP_X_RES ) | UEP_X_RES_ACK;
											break; 
#endif
																  
#ifdef USE_D0_EP3_IN
										case 0x83:
											D0_EP3RES = D0_EP3RES & ~(  MASK_UEP_X_RES ) | UEP_X_RES_NAK;
											break;
#endif
											
#ifdef USE_D0_EP3_OUT
										case 0x03:
											D0_EP3RES = D0_EP3RES & ~(  MASK_UEP_X_RES ) | UEP_X_RES_ACK;
											break; 
#endif
																  
#ifdef USE_D0_EP2_IN
										case 0x82:                       
											D0_EP2RES = D0_EP2RES & ~(  MASK_UEP_X_RES ) | UEP_X_RES_NAK;
											break;   
#endif
																
#ifdef USE_D0_EP2_OUT
										case 0x02:                       
											D0_EP2RES = D0_EP2RES & ~(  MASK_UEP_X_RES ) | UEP_X_RES_ACK;
											break; 
#endif
																  
#ifdef USE_D0_EP1_IN
										case 0x81:                       
											D0_EP1RES = D0_EP1RES & ~(  MASK_UEP_X_RES ) | UEP_X_RES_NAK;
											break;
#endif
																   
#ifdef USE_D0_EP1_OUT
										case 0x01:                       
											D0_EP1RES = D0_EP1RES & ~(  MASK_UEP_X_RES ) | UEP_X_RES_ACK;
											break;
#endif
											
										default:
											len = 0xFFFF;  
											break;
									}
								}
								else if( ( pD0_SETUP_REQ->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_DEVICE )
								{
									if( pD0_SETUP_REQ->wValueL == 1 )
									{ 
										USB_SleepStatus &= ~0x01;                      
									}
								}
								else
								{
									len = 0xFFFF;  
								}											
								break;
												
							case USB_GET_INTERFACE:
								pD0_EP0_BUF[ 0 ] = 0x00;
								if( D0SetupLen >= 1 ) 
								{
									len = 1;
								}
								break;
								
							case USB_GET_STATUS: 
								if( ( pD0_SETUP_REQ->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP ) 
								{  
									/* endpoint */
									pD0_EP0_BUF[ 0 ] = 0x00;
									switch( pD0_SETUP_REQ->wIndexL ) 
									{
#ifdef USE_D0_EP6_IN
										case 0x86:                                       
											if( ( D0_EP6RES & MASK_UEP_X_RES ) == UEP_X_RES_STALL )
											{
												pD0_EP0_BUF[ 0 ] = 0x01;
											}                                      
											break;
#endif
											
#ifdef USE_D0_EP5_IN
										case 0x85:                                       
										//case 0x05:
											if( ( D0_EP5RES & MASK_UEP_X_RES ) == UEP_X_RES_STALL )
											{
												pD0_EP0_BUF[ 0 ] = 0x01;
											}                                      
											break;
#endif
											
#ifdef USE_D0_EP4_OUT
										case 0x04:
											if( ( D0_EP4RES & MASK_UEP_X_RES ) == UEP_X_RES_STALL )
											{
												pD0_EP0_BUF[ 0 ] = 0x01;
											}                                      
											break;
#endif

#ifdef USE_D0_EP3_IN
										case 0x83:                                       
#endif
#ifdef USE_D0_EP3_OUT
										case 0x03:
#endif
#if defined(USE_D0_EP3_IN) || defined(USE_D0_EP3_OUT)
											if( ( D0_EP3RES & MASK_UEP_X_RES ) == UEP_X_RES_STALL )
											{
												pD0_EP0_BUF[ 0 ] = 0x01;
											}                                      
											break;
#endif
											
#ifdef USE_D0_EP2_IN
										case 0x82:                                     
#endif
#ifdef USE_D0_EP2_OUT
										case 0x02:
#endif
#if defined(USE_D0_EP2_IN) || defined(USE_D0_EP2_OUT)
											if( ( D0_EP2RES & MASK_UEP_X_RES ) == UEP_X_RES_STALL )
											{
												pD0_EP0_BUF[ 0 ] = 0x01;																					
											}
											break;
#endif
											
#ifdef USE_D0_EP1_IN
										case 0x81:                                       
#endif
#ifdef USE_D0_EP1_OUT
										case 0x01:
#endif
#if defined(USE_D0_EP1_IN) || defined(USE_D0_EP1_OUT)
											if( ( D0_EP1RES &MASK_UEP_X_RES ) == UEP_X_RES_STALL )
											{
												pD0_EP0_BUF[0] = 0x01;
											}
											break;                                
#endif
										}
								}
								else if( ( pD0_SETUP_REQ->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_DEVICE )
								{
									pD0_EP0_BUF[ 0 ] = 0x00;
									if( USB_SleepStatus )
									{
										pD0_EP0_BUF[ 0 ] = 0x02;
									}
									else
									{
										pD0_EP0_BUF[0] = 0x00;
									}
								}																	
								pD0_EP0_BUF[ 1 ] = 0;
								if( D0SetupLen >= 2 ) 
								{
									len = 2;
								}
								else 
								{
									len = D0SetupLen;
								}
								break;
								
							default:
								/* operation failed */
								len = 0xFFFF;  
								break;
						}
					}
					else if( ( pD0_SETUP_REQ->bRequestType & USB_REQ_TYP_MASK ) != USB_REQ_TYP_STANDARD ) 
					{  
						/* non-standard request */
						
						/*Other requests, such as class requests, manufacturer requests, etc.*/	
						if( pD0_SETUP_REQ->bRequestType & 0x40 )		 		
						{    
							/* Vendor request */
						}		
						else if( pD0_SETUP_REQ->bRequestType & 0x20 )	 		
						{
							/* class request */
							switch( D0SetupReqCode )
							{
								case DEF_USB_SET_REPORT:			 				/* 0x09: SET_REPORT */
									break;
								
								case DEF_USB_SET_IDLE:								/* 0x0A: SET_IDLE */
									Idle_Value = pD0_EP0_BUF[ 3 ];
									break;
									
								case DEF_USB_SET_PROTOCOL:							/* 0x0B: SET_PROTOCOL */
									Report_Value = pD0_EP0_BUF[ 2 ];
									break;
									
								case DEF_USB_GET_IDLE:								/* 0x02: GET_IDLE */
									pD0_EP0_BUF[ 0 ] = Idle_Value;
									len = 1;
									break;

								case DEF_USB_GET_PROTOCOL:							/* 0x03: GET_PROTOCOL */
									pD0_EP0_BUF[ 0 ] = Report_Value;
									len = 1;
									break;
									
								default:
									len = 0xFFFF;  							
									break;
							}
						}
					}
					else
					{  
						/* Only supports standard requests/class requests */
						len = 0xFFFF;  
					}
					
					if( len == 0xFFFF ) 
					{  
						/* operation failed */
						D0SetupReqCode = 0xFF;
						D0_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;  
						
						/* Delay undoing of stall to prevent it from being too late to set in the next setup */					
						i = 255;
						while( i-- );
						D0_EP0RES = D0_EP0RES & ~(MASK_UEP_R_RES|MASK_UEP_T_RES) | UEP_R_RES_ACK | UEP_T_RES_NAK;	
					}
					else if( len <= DEF_ENDP0_SIZE ) 
					{  
						/* Upload data or status phase returns 0 length packet */               
						D0_EP0T_L = len;
						D0_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  /* The default packet is data1 */  
					}
					else 
					{  
						/* Download data or other */
						/* Although it has not yet reached the status stage, it is preset to upload 0-length data packets in advance to prevent the host from entering the status stage early. */
						D0_EP0T_L = 0;  
						D0_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  /* The default packet is data1 */
					}
				}
				break;
		}     
        D0_STATUS = 0;    
	}
	else if( us & bUX_IF_SUSPEND )
    {
    	/* Suspend or wake up events */	
		if( us & bUX_SUSPEND )
		{       
			/* pending events */
        //print("zzz...\n");
			USB_SleepStatus |= 0x02;			
			if( USB_SleepStatus != 0x03 )
			{				
				USB_EnumStatus = 0x00;									
			}
			else
			{
				MCU_Sleep_Operate = 0x01;
			}
		}
		else
		{                      
			/* Wake event */
			USB_SleepStatus &= ~0x02;
			USB_EnumStatus = 0x01;										
        }
		USB_IF = bUX_IF_SUSPEND;
		D0_STATUS = 0;
    }
	else if(us & bUX_IF_BUS_RST) 
    {
    	/*bus reset event*/
        //print("reset...\n");
        USB_EP_init();                           // setup endpoints
        //D0_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
        //D0_EP1RES = UEP_X_RES_NAK;
       	//D0_EP2RES = UEP_X_RES_NAK;
        //D0_EP3RES = UEP_X_RES_ACK; 
       	//D0_EP5RES = UEP_X_RES_NAK;
        //D0_EP_MOD = bUX_EP3O_EN | bUX_DEV_EN;  
		//D0_EP_MOD |= bUX_EP5I_EN;
        D0_ADDR = 0;
        HB_ADDR = 0x7F;             
        USB_IF = 0xFF;
        
        USB_IE = bUX_IE_SUSPEND | bUX_IE_TRANSFER | bUX_IE_BUS_RST;

        KB_USB_UpStatus = 0x00;
		USB_EnumStatus = 0x00;										

        USB_IF = bUX_IF_BUS_RST;
	}
	else
    {
    	/*It is an interrupt that does not need to be processed. Clear it directly.*/
        //print("@\n");
		//printf("%x\n",us);
		//printf("%x\n",D0_STATUS);
		//printf("%x\n",SP);
        D0_STATUS = 0;
	}
  	
  	/* Determine again whether there are still USB interrupts that need to be processed */
	if( USB_IF & ( bUX_IF_D0_TRANS | bUX_IF_SUSPEND | bUX_IF_BUS_RST ) ) 
	{
		goto USB_DevIntNext;
	}    
}
#pragma restore

/*******************************************************************************
* Function Name  : USB_WakeUp_PC
* Description    : Usb device wakes up computer
* Input          : None
* Output         : None
* Return         : None									   
*******************************************************************************/
void USB_WakeUp_PC( void )
{
	UINT8  save;
	
	save = USB_CTRL;
    USB_CTRL ^= bUX_DP_PU_EN | bUX_DM_PU_EN;
    mDelaymS( 10 );  
    USB_CTRL = save;
}

/*******************************************************************************
* Function Name  : CH55X_Sleep_Deal
* Description    : Ch55x microcontroller sleep processing
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void CH55X_Sleep_Deal( void )	
{
	UINT8	savePortCfg;

	P4_LED_KEY = 0xFF;															/* enable key mode */
    P4_MOD_OC = 0xFF;															/* disable push-pull output */
    //LED_CTRL &= ~bLED_EN;
																				/* P4_DIR_PU = 0xFF; */																						
	P0 = P3 = 0;																/* COMMON all low to support any key wakeup */
	P7 &= ~0x03;																/* P7 &= ~ 0x03, if P7 used for COMMON  */ 
	savePortCfg = PORT_CFG;
	PORT_CFG |= bP4_IE_LEVEL;		/* enable P4 level change to wakeup, P4 input@pullup vs P4 output@0xFF, so low action */
																				/* USB_CTRL = 0; */	
	/* Before sleeping, switch the USB port pull-up resistor to 7 k5 */
	P6_OUT_PU = ( 1 << 1 );
	USB_CTRL &= ~bUX_DP_PU_EN;
	P6_DIR = 0;
	
	dprint("sleepin...");
	PCON |= PD;																	/* sleep */
		
	mDelaymS( 1 ); 
	USB_CTRL |= bUX_DP_PU_EN;
	P6_OUT_PU = 0;

	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	WAKE_CTRL = 0x00;
	SAFE_MOD = 0x00;
    
	PORT_CFG = savePortCfg;
	if ( GPIO_IE & bIE_IO_EDGE ) 
	{	
		/* edge mode interrupt */
		GPIO_IE &= ~ bIE_IO_EDGE;												/* clear P4 level change interrupt flag */
		GPIO_IE |= bIE_IO_EDGE;	
	}
	mDelaymS( 5 );

	P0 = P3 = 0xFF;  																							
	//LED_CTRL = bLED_IE_INHIB | bLED_BLUE_EN | bLED_GREEN_EN | bLED_RED_EN | bLED_COM_AHEAD | bLED_PWM_INHIB | bLED_EN;
	//LED_STATUS |= bLED_IF_SET;  												/* force bLED_IF to enter interrupt service */
	//IE_PWM_I2C = 1;   
}

