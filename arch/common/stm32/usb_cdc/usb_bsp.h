/**
  ******************************************************************************
  * @file    usb_bsp.h
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    22-July-2011
  * @brief   Specific api's relative to the used hardware platform
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

#ifndef __USB_BSP__H__
#define __USB_BSP__H__

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "usb_core.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"


void BSP_Init(void);

void USB_OTG_BSP_Init (USB_OTG_CORE_HANDLE *pdev);
void USB_OTG_BSP_uDelay (const uint32_t usec);
void USB_OTG_BSP_mDelay (const uint32_t msec);
void USB_OTG_BSP_EnableInterrupt (USB_OTG_CORE_HANDLE *pdev);

#endif



