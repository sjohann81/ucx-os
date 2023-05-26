/**
  ******************************************************************************
  * @file    usbd_req.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2011
  * @brief   header file for the usbd_req.c file
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
  

#ifndef __USB_REQUEST_H_
#define __USB_REQUEST_H_

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include  "usbd_def.h"
#include  "usbd_core.h"
#include  "usbd_conf.h"


USBD_Status  USBD_StdDevReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req);
USBD_Status  USBD_StdItfReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req);
USBD_Status  USBD_StdEPReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req);
void USBD_ParseSetupRequest( USB_OTG_CORE_HANDLE  *pdev,
                                    USB_SETUP_REQ *req);

void USBD_CtlError( USB_OTG_CORE_HANDLE  *pdev,
                            USB_SETUP_REQ *req);

void USBD_GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len);

#endif
