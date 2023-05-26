/**
  ******************************************************************************
  * @file    usbd_usr.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   This file includes the user application layer
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "usbd_usr.h"
#include "usbd_ioreq.h"



USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,
};

USB_CDC_STATUS_t USB_CDC_STATUS;


//--------------------------------------------------------------
void USBD_USR_Init(void)
{  
  USB_CDC_STATUS=USB_CDC_DETACHED;
}


//--------------------------------------------------------------
void USBD_USR_DeviceReset(uint8_t speed )
{
  USB_CDC_STATUS=USB_CDC_DETACHED;
}


//--------------------------------------------------------------
void USBD_USR_DeviceConfigured (void)
{
  USB_CDC_STATUS=USB_CDC_CONNECTED;
}


//--------------------------------------------------------------
void USBD_USR_DeviceConnected (void)
{
  USB_CDC_STATUS=USB_CDC_DETACHED;
}


//--------------------------------------------------------------
void USBD_USR_DeviceDisconnected (void)
{
  USB_CDC_STATUS=USB_CDC_DETACHED;
}


//--------------------------------------------------------------
void USBD_USR_DeviceSuspended(void)
{
  USB_CDC_STATUS=USB_CDC_DETACHED;
}


//--------------------------------------------------------------
void USBD_USR_DeviceResumed(void)
{
  USB_CDC_STATUS=USB_CDC_DETACHED;
}




