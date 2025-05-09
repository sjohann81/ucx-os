/** @file HL_rti.h
*   @brief RTI Driver Header File
*   @date 11-Dec-2018
*   @version 04.07.01
*   
*   This file contains:
*   - Definitions
*   - Types
*   - Interface Prototypes
*   .
*   which are relevant for the RTI driver.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com  
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/



#ifndef __RTI_H__
#define __RTI_H__

#include <stdint.h>
#include <uapi/errno.h>

#define RTI_FREQUENCY 9375000

typedef volatile struct rtiBase
{
    uint32_t GCTRL;          /**< 0x0000: Global Control Register   */
    uint32_t TBCTRL;         /**< 0x0004: Timebase Control Register */
    uint32_t CAPCTRL;        /**< 0x0008: Capture Control Register  */
    uint32_t COMPCTRL;       /**< 0x000C: Compare Control Register  */
    struct
    {
        uint32_t FRCx;       /**< 0x0010,0x0030: Free Running Counter x Register         */
        uint32_t UCx;        /**< 0x0014,0x0034: Up Counter x Register                   */
        uint32_t CPUCx;      /**< 0x0018,0x0038: Compare Up Counter x Register           */
        uint32_t   rsvd1;    /**< 0x001C,0x003C: Reserved                                */
        uint32_t CAFRCx;     /**< 0x0020,0x0040: Capture Free Running Counter x Register */
        uint32_t CAUCx;      /**< 0x0024,0x0044: Capture Up Counter x Register           */
        uint32_t   rsvd2[2U]; /**< 0x0028,0x0048: Reserved                                */
    } CNT[2U];               /**< Counter x selection:
                                    - 0: Counter 0
                                    - 1: Counter 1                                       */
    struct
    {
        uint32_t COMPx;      /**< 0x0050,0x0058,0x0060,0x0068: Compare x Register        */
        uint32_t UDCPx;      /**< 0x0054,0x005C,0x0064,0x006C: Update Compare x Register */
    } CMP[4U];               /**< Compare x selection:
                                    - 0: Compare 0
                                    - 1: Compare 1
                                    - 2: Compare 2
                                    - 3: Compare 3                                       */
    uint32_t TBLCOMP;        /**< 0x0070: External Clock Timebase Low Compare Register   */
    uint32_t TBHCOMP;        /**< 0x0074: External Clock Timebase High Compare Register  */
    uint32_t   rsvd3[2U];    /**< 0x0078: Reserved                                       */
    uint32_t SETINTENA;      /**< 0x0080: Set/Status Interrupt Register                  */
    uint32_t CLEARINTENA;    /**< 0x0084: Clear/Status Interrupt Register                */
    uint32_t INTFLAG;        /**< 0x0088: Interrupt Flag Register                        */
    uint32_t   rsvd4;        /**< 0x008C: Reserved                                       */
    uint32_t DWDCTRL;        /**< 0x0090: Digital Watchdog Control Register   */
    uint32_t DWDPRLD;        /**< 0x0094: Digital Watchdog Preload Register */
    uint32_t WDSTATUS;       /**< 0x0098: Watchdog Status Register  */
    uint32_t WDKEY;          /**< 0x009C: Watchdog Key Register  */
    uint32_t DWDCNTR;        /**< 0x00A0: Digital Watchdog Down Counter   */
    uint32_t WWDRXNCTRL;     /**< 0x00A4: Digital Windowed Watchdog Reaction Control */
    uint32_t WWDSIZECTRL;    /**< 0x00A8: Digital Windowed Watchdog Window Size Control  */
    uint32_t INTCLRENABLE;   /**< 0x00AC: RTI Compare Interrupt Clear Enable Register  */
    uint32_t COMP0CLR;       /**< 0x00B0: RTI Compare 0 Clear Register   */
    uint32_t COMP1CLR;       /**< 0x00B4: RTI Compare 1 Clear Register */
    uint32_t COMP2CLR;       /**< 0x00B8: RTI Compare 2 Clear Register  */
    uint32_t COMP3CLR;       /**< 0x00BC: RTI Compare 3 Clear Register  */
} rtiBASE_t;

/** @def rtiREG1
*   @brief RTI1 Register Frame Pointer
*
*   This pointer is used by the RTI driver to access the RTI1 registers.
*/
#define rtiREG1 ((rtiBASE_t *)0xFFFFFC00U)

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN (0) */
/* USER CODE END */

/* RTI General Definitions */

/** @def rtiCOUNTER_BLOCK0
*   @brief Alias name for RTI counter block 0
*
*   This is an alias name for the RTI counter block 0.
*
*   @note This value should be used for API argument @a counter
*/
#define rtiCOUNTER_BLOCK0 0U

/** @def rtiCOUNTER_BLOCK1
*   @brief Alias name for RTI counter block 1
*
*   This is an alias name for the RTI counter block 1.
*
*   @note This value should be used for API argument @a counter
*/
#define rtiCOUNTER_BLOCK1 1U

/** @def rtiCOMPARE0
*   @brief Alias name for RTI compare 0
*
*   This is an alias name for the RTI compare 0.
*
*   @note This value should be used for API argument @a compare
*/
#define rtiCOMPARE0 0U

/** @def rtiCOMPARE1
*   @brief Alias name for RTI compare 1
*
*   This is an alias name for the RTI compare 1.
*
*   @note This value should be used for API argument @a compare
*/
#define rtiCOMPARE1 1U

/** @def rtiCOMPARE2
*   @brief Alias name for RTI compare 2
*
*   This is an alias name for the RTI compare 2.
*
*   @note This value should be used for API argument @a compare
*/
#define rtiCOMPARE2 2U

/** @def rtiCOMPARE3
*   @brief Alias name for RTI compare 3
*
*   This is an alias name for the RTI compare 3.
*
*   @note This value should be used for API argument @a compare
*/
#define rtiCOMPARE3 3U

/** @def rtiNOTIFICATION_COMPARE0
*   @brief Alias name for RTI compare 0 notification
*
*   This is an alias name for the RTI compare 0 notification.
*
*   @note This value should be used for API argument @a notification
*/
#define rtiNOTIFICATION_COMPARE0 1U

/** @def rtiNOTIFICATION_COMPARE1
*   @brief Alias name for RTI compare 1 notification
*
*   This is an alias name for the RTI compare 1 notification.
*
*   @note This value should be used for API argument @a notification
*/
#define rtiNOTIFICATION_COMPARE1 2U

/** @def rtiNOTIFICATION_COMPARE2
*   @brief Alias name for RTI compare 2 notification
*
*   This is an alias name for the RTI compare 2 notification.
*
*   @note This value should be used for API argument @a notification
*/
#define rtiNOTIFICATION_COMPARE2 4U

/** @def rtiNOTIFICATION_COMPARE3
*   @brief Alias name for RTI compare 3 notification
*
*   This is an alias name for the RTI compare 3 notification.
*
*   @note This value should be used for API argument @a notification
*/
#define rtiNOTIFICATION_COMPARE3 8U

/** @def rtiNOTIFICATION_TIMEBASE
*   @brief Alias name for RTI timebase notification
*
*   This is an alias name for the RTI timebase notification.
*
*   @note This value should be used for API argument @a notification
*/
#define rtiNOTIFICATION_TIMEBASE 0x10000U

/** @def rtiNOTIFICATION_COUNTER0
*   @brief Alias name for RTI counter block 0 overflow notification
*
*   This is an alias name for the RTI counter block 0 overflow notification.
*
*   @note This value should be used for API argument @a notification
*/
#define rtiNOTIFICATION_COUNTER0 0x20000U

/** @def rtiNOTIFICATION_COUNTER1
*   @brief Alias name for RTI counter block 1 overflow notification
*
*   This is an alias name for the RTI counter block 1 overflow notification.
*
*   @note This value should be used for API argument @a notification
*/
#define rtiNOTIFICATION_COUNTER1 0x40000U

/* USER CODE BEGIN (1) */
/* USER CODE END */

/** @enum dwdViolationTag
*   @brief DWD Violations
*/
typedef enum dwdViolationTag
{
	NoTime_Violation = 0U,
	Time_Window_Violation = 1U,
	EndTime_Window_Violation = 2U,
	StartTime_Window_Violation = 3U,
	Key_Seq_Violation = 4U
}dwdViolation_t;

/* USER CODE BEGIN (2) */
/* USER CODE END */

/** @enum dwdResetStatusTag
*   @brief DWD Reset status
*/
typedef enum dwdResetStatusTag
{
	No_Reset_Generated = 0U,
	Reset_Generated    = 1U
}dwdResetStatus_t;

/* USER CODE BEGIN (3) */
/* USER CODE END */

/** @enum dwwdReactionTag
*   @brief DWWD Reaction on vioaltion
*/
typedef enum dwwdReactionTag
{
	Generate_Reset = 0x00000005U,
	Generate_NMI   = 0x0000000AU
}dwwdReaction_t;

/* USER CODE BEGIN (4) */
/* USER CODE END */

/** @enum dwwdWindowSizeTag
*   @brief DWWD Window size
*/
typedef enum dwwdWindowSizeTag
{
	Size_100_Percent   = 0x00000005U,
	Size_50_Percent    = 0x00000050U,
	Size_25_Percent    = 0x00000500U,
	Size_12_5_Percent  = 0x00005000U,
	Size_6_25_Percent  = 0x00050000U,
	Size_3_125_Percent = 0x00500000U
}dwwdWindowSize_t;

/* USER CODE BEGIN (5) */
/* USER CODE END */

/* Configuration registers */
typedef struct rti_config_reg
{
    uint32_t CONFIG_GCTRL;
    uint32_t CONFIG_TBCTRL;
    uint32_t CONFIG_CAPCTRL;
    uint32_t CONFIG_COMPCTRL;
    uint32_t CONFIG_UDCP0;
    uint32_t CONFIG_UDCP1;
    uint32_t CONFIG_UDCP2;
    uint32_t CONFIG_UDCP3;
} rti_config_reg_t;


/* Configuration registers initial value */
#define RTI_GCTRL_CONFIGVALUE	 ((uint32_t)((uint32_t)0x5U << 16U) | 0x00000000U)
#define RTI_TBCTRL_CONFIGVALUE  0x00000000U
#define RTI_CAPCTRL_CONFIGVALUE  (0U | 0U)
#define RTI_COMPCTRL_CONFIGVALUE (0x00001000U | 0x00000100U | 0x00000000U | 0x00000000U)
#define RTI_UDCP0_CONFIGVALUE 9375U
#define RTI_UDCP1_CONFIGVALUE 46875U
#define RTI_UDCP2_CONFIGVALUE 75000U
#define RTI_UDCP3_CONFIGVALUE 93750U

#define RTI_COUNTER_1_RES_NS 13.33f

/** 
 *  @defgroup RTI RTI
 *  @brief Real Time Interrupt Module.
 *  
 *  The real-time interrupt (RTI) module provides timer functionality for operating systems and for
 *  benchmarking code. The RTI module can incorporate several counters that define the timebases needed
 *  for scheduling in the operating system.
 *
 *	Related Files
 *   - HL_reg_rti.h
 *   - HL_rti.h
 *   - HL_rti.c
 *  @addtogroup RTI
 *  @{
 */
 
/* RTI Interface Functions */

void init_rti(void);
void rtiStartCounter(rtiBASE_t *rtiREG,uint32_t counter);
void rtiStopCounter(rtiBASE_t *rtiREG,uint32_t counter);
uint32_t rtiResetCounter(rtiBASE_t *rtiREG,uint32_t counter);
void rtiSetPeriod(rtiBASE_t *rtiREG,uint32_t compare, uint32_t period);
uint32_t rtiGetPeriod(rtiBASE_t *rtiREG,uint32_t compare);
uint32_t rtiGetCurrentTick(rtiBASE_t *rtiREG,uint32_t compare);
void rtiEnableNotification(rtiBASE_t *rtiREG,uint32_t notification);
void rtiDisableNotification(rtiBASE_t *rtiREG,uint32_t notification);
void ja_bsp_process_timer(void);
pok_ret_t ja_get_hpet_ns(uint64_t *timer);

/**@}*/
#ifdef __cplusplus
}
#endif /*extern "C" */

#endif
