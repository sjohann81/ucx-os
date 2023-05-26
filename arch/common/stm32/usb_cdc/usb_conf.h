/**
  ******************************************************************************
  * @file    usb_conf.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   General low level driver configuration
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
  
#ifndef __USB_CONF__H__
#define __USB_CONF__H__

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"

/****************** USB OTG FS CONFIGURATION **********************************/
 #define RX_FIFO_FS_SIZE                          128
 #define TX0_FIFO_FS_SIZE                          32
 #define TX1_FIFO_FS_SIZE                         128
 #define TX2_FIFO_FS_SIZE                          32 
 #define TX3_FIFO_FS_SIZE                           0

/****************** USB OTG MODE CONFIGURATION ********************************/

#define __ALIGN_BEGIN
#define __ALIGN_END

#define __packed    __attribute__ ((__packed__))

#endif




