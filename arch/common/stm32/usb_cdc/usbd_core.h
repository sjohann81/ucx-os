/**
  ******************************************************************************
  * @file    usbd_core.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2011
  * @brief   Header file for usbd_core.c
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


#ifndef __USBD_CORE_H
#define __USBD_CORE_H

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "usb_dcd.h"
#include "usbd_def.h"
#include "usbd_conf.h"



typedef enum {
  USBD_OK   = 0,
  USBD_BUSY,
  USBD_FAIL,
}USBD_Status;



void USBD_Init(USB_OTG_CORE_HANDLE *pdev,
               USB_OTG_CORE_ID_TypeDef coreID, 
               USBD_DEVICE *pDevice,                  
               USBD_Class_cb_TypeDef *class_cb, 
               USBD_Usr_cb_TypeDef *usr_cb);

USBD_Status USBD_DeInit(USB_OTG_CORE_HANDLE *pdev);

USBD_Status USBD_ClrCfg(USB_OTG_CORE_HANDLE  *pdev, uint8_t cfgidx);

USBD_Status USBD_SetCfg(USB_OTG_CORE_HANDLE  *pdev, uint8_t cfgidx);


#endif





