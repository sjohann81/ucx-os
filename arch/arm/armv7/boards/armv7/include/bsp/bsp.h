/*
 * Polytechnique Montreal
 * Copyright (C) 2019 Alexy Torres Aurora Dugo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, Version 3.
 *
 * This program is distributed in the hope # that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License version 3 for more details.
 *
 * This file also incorporates work covered by CHPOK License
 * Institute for System Programming of the Russian Academy of Sciences
 * Copyright (C) 2016 ISPRAS
 *
 * This file also incorporates work covered by POK License.
 * Copyright (C) 2007-2009 POK team
 */

#ifndef __BSP_H__
#define __BSP_H__

#include <stdint.h>

/**
 * @brief System register type 1.
 * @details System register type 1 structure as defined by documentation.
 */
typedef struct sys_reg_type1
{
    uint32_t SYSPC1;
    uint32_t SYSPC2;
    uint32_t SYSPC3;
    uint32_t SYSPC4;
    uint32_t SYSPC5;
    uint32_t SYSPC6;
    uint32_t SYSPC7;
    uint32_t SYSPC8;
    uint32_t SYSPC9;
    uint32_t rsvd1;
    uint32_t rsvd2;
    uint32_t rsvd3;
    uint32_t CSDIS;
    uint32_t CSDISSET;
    uint32_t CSDISCLR;
    uint32_t CDDIS;
    uint32_t CDDISSET;
    uint32_t CDDISCLR;
    uint32_t GHVSRC;
    uint32_t VCLKASRC;
    uint32_t RCLKSRC;
    uint32_t CSVSTAT;
    uint32_t MSTGCR;
    uint32_t MINITGCR;
    uint32_t MSINENA;
    uint32_t MSTFAIL;
    uint32_t MSTCGSTAT;
    uint32_t MINISTAT;
    uint32_t PLLCTL1;
    uint32_t PLLCTL2;
    uint32_t SYSPC10;
    uint32_t DIEIDL;
    uint32_t DIEIDH;
    uint32_t rsvd4;
    uint32_t LPOMONCTL;
    uint32_t CLKTEST;
    uint32_t DFTCTRLREG1;
    uint32_t DFTCTRLREG2;
    uint32_t rsvd5;
    uint32_t rsvd6;
    uint32_t GPREG1;
    uint32_t rsvd7;
    uint32_t rsvd8;
    uint32_t rsvd9;
    uint32_t SSIR1;
    uint32_t SSIR2;
    uint32_t SSIR3;
    uint32_t SSIR4;
    uint32_t RAMGCR;
    uint32_t BMMCR1;
    uint32_t rsvd10;
    uint32_t CPURSTCR;
    uint32_t CLKCNTL;
    uint32_t ECPCNTL;
    uint32_t rsvd11;
    uint32_t DEVCR1;
    uint32_t SYSECR;
    uint32_t SYSESR;
    uint32_t SYSTASR;
    uint32_t GBLSTAT;
    uint32_t DEVID;
    uint32_t SSIVEC;
    uint32_t SSIF;
} sys_reg_type1_t;

/**
 * @brief System register type 2.
 * @details System register type 2 structure as defined by documentation.
 */
typedef struct sys_reg_type2
{
    uint32_t PLLCTL3;
    uint32_t rsvd1;
    uint32_t STCCLKDIV;
    uint32_t rsvd2[6];
    uint32_t ECPCNTL;
    uint32_t ECPCNTL1;
    uint32_t rsvd3[4];
    uint32_t CLK2CNTRL;
    uint32_t VCLKACON1;
    uint32_t rsvd4[4];
    uint32_t HCLKCNTL;
    uint32_t rsvd5[6];
    uint32_t CLKSLIP;
    uint32_t rsvd6;
    uint32_t IP1ECCERREN;
    uint32_t rsvd7[28];
    uint32_t EFC_CTLEN;
    uint32_t DIEIDL_REG0;
    uint32_t DIEIDH_REG1;
    uint32_t DIEIDL_REG2;
    uint32_t DIEIDH_REG3;
} sys_reg_type2_t;

#define SYS_REG_1_BASE ((sys_reg_type1_t *)0xFFFFFF00)
#define SYS_REG_2_BASE ((sys_reg_type2_t *)0xFFFFE100)

/**
 * @brief PCR register structure as per defined by the documentation.
 */
typedef struct pcr_reg
{
    uint32_t PMPROTSET0;
    uint32_t PMPROTSET1;
    uint32_t rsvd1[2];
    uint32_t PMPROTCLR0;
    uint32_t PMPROTCLR1;
    uint32_t rsvd2[2];
    uint32_t PPROTSET0;
    uint32_t PPROTSET1;
    uint32_t PPROTSET2;
    uint32_t PPROTSET3;
    uint32_t rsvd3[4];
    uint32_t PPROTCLR0;
    uint32_t PPROTCLR1;
    uint32_t PPROTCLR2;
    uint32_t PPROTCLR3;
    uint32_t rsvd4[4];
    uint32_t PCSPWRDWNSET0;
    uint32_t PCSPWRDWNSET1;
    uint32_t rsvd5[2];
    uint32_t PCSPWRDWNCLR0;
    uint32_t PCSPWRDWNCLR1;
    uint32_t rsvd6[2];
    uint32_t PSPWRDWNSET0;
    uint32_t PSPWRDWNSET1;
    uint32_t PSPWRDWNSET2;
    uint32_t PSPWRDWNSET3;
    uint32_t rsvd7[4];
    uint32_t PSPWRDWNCLR0;
    uint32_t PSPWRDWNCLR1;
    uint32_t PSPWRDWNCLR2;
    uint32_t PSPWRDWNCLR3;
    uint32_t rsvd8[4];
    uint32_t PDPWRDWNSET;
    uint32_t PDPWRDWNCLR;
    uint32_t rsvd9[78];
    uint32_t MSTIDWRENA;
    uint32_t MSTIDENA;
    uint32_t MSTIDDIAGCTRL;
    uint32_t rsvd10[61];
    struct
    {
        uint32_t PSxMSTID_L;
        uint32_t PSxMSTID_H;
    }PSxMSTID[32];
    struct
    {
        uint32_t PPSxMSTID_L;
        uint32_t PPSxMSTID_H;
    }PPSxMSTID[8];
    struct
    {
        uint32_t PPSExMSTID_L;
        uint32_t PPSExMSTID_H;
    }PPSExMSTID[32];
    uint32_t PCSxMSTID[32];
    uint32_t PPCSxMSTID[8];
} pcr_reg_t;


#define PCR_REG_1 ((pcr_reg_t *)0xFFFF1000)
#define PCR_REG_2 ((pcr_reg_t *)0xFCFF1000)
#define PCR_REG_3 ((pcr_reg_t *)0xFFF78000)

/**
 * @brief Flash register structure as per defined by the documentation.
 */
typedef struct flash_reg
{
    uint32_t FRDCNTL;
    uint32_t rsvd1;
    uint32_t EE_FEDACCTRL1;
    uint32_t rsvd2;
    uint32_t rsvd3;
    uint32_t FEDAC_PASTATUS;
    uint32_t FEDAC_PBSTATUS;
    uint32_t FEDAC_GBLSTATUS;
    uint32_t rsvd4;
    uint32_t FEDACSDIS;
    uint32_t FPRIM_ADD_TAG;
    uint32_t FDUP_ADD_TAG;
    uint32_t FBPROT;
    uint32_t FBSE;
    uint32_t FBBUSY;
    uint32_t FBAC;
    uint32_t FBPWRMODE;
    uint32_t FBPRDY;
    uint32_t FPAC1;
    uint32_t rsvd5;
    uint32_t FMAC;
    uint32_t FMSTAT;
    uint32_t FEMU_DMSW;
    uint32_t FEMU_DLSW;
    uint32_t FEMU_ECC;
    uint32_t FLOCK;
    uint32_t rsvd6;
    uint32_t FDIAGCTRL;
    uint32_t rsvd7;
    uint32_t FRAW_ADDR;
    uint32_t rsvd8;
    uint32_t FPAR_OVR;
    uint32_t rsvd9[13];
    uint32_t RCR_VALID;
    uint32_t ACC_THRESHOLD;
    uint32_t rsvd10;
    uint32_t FEDACSDIS2;
    uint32_t rsvd11;
    uint32_t rsvd12;
    uint32_t rsvd13;
    uint32_t RCR_VALUE0;
    uint32_t RCR_VALUE1;
    uint32_t rsvd14[108];
    uint32_t FSM_WR_ENA;
    uint32_t rsvd15[11];
    uint32_t EEPROM_CONFIG;
    uint32_t rsvd16;
    uint32_t FSM_SECTOR1;
    uint32_t FSM_SECTOR2;
    uint32_t rsvd17[78];
    uint32_t FCFG_BANK;

} flash_reg_t;


#define FLASH_REG ((flash_reg_t *)(0xFFF87000))
#define FSM_WR_ENA_HL       (*(volatile uint32_t *)0xFFF87288)
#define EEPROM_CONFIG_HL    (*(volatile uint32_t *)0xFFF872B8)


/**
 * @brief Flash power modes.
 */
typedef enum FLASH_POWER_MODE
{
    SYS_SLEEP   = 0U, /**< Alias for flash bank power mode sleep   */
    SYS_STANDBY = 1U, /**< Alias for flash bank power mode standby */
    SYS_ACTIVE  = 3U  /**< Alias for flash bank power mode active  */
}FLASH_POWER_MODE_E;

/** @enum SYS_CLOCK_SOURCE_E
*   @brief Alias names for clock sources
*/
typedef enum SYS_CLOCK_SOURCE
{
    SYS_OSC             = 0x0U,
    SYS_PLL1            = 0x1U,  /**< Alias for Pll1 clock Source                      */
    SYS_EXTERNAL1       = 0x3U,  /**< Alias for external clock Source                  */
    SYS_LPO_LOW         = 0x4U,  /**< Alias for low power oscillator low clock Source  */
    SYS_LPO_HIGH        = 0x5U,  /**< Alias for low power oscillator high clock Source */
    SYS_PLL2            = 0x6U,  /**< Alias for Pll2 clock Source                      */
    SYS_EXTERNAL2       = 0x7U,  /**< Alias for external 2 clock Source                */
    SYS_VCLK            = 0x9U,  /**< Alias for synchronous VCLK1 clock Source         */
    SYS_PLL2_ODCLK_8    = 0xEU,  /**< Alias for PLL2_post_ODCLK/8                      */
    SYS_PLL2_ODCLK_16   = 0xFU   /**< Alias for PLL2_post_ODCLK/8                      */
} SYS_CLOCK_SOURCE_E;

#define SYS_DOZE_MODE        0x000F3F02
#define SYS_SNOOZE_MODE      0x000F3F03
#define SYS_SLEEP_MODE       0x000FFFFF
#define LPO_TRIM_VALUE       (((*(volatile uint32_t *)0xF00801B4) & 0xFFFF0000)>>16U)
#define SYS_EXCEPTION        (*(volatile uint32_t *)0xFFFFFFE4)

void ja_bsp_init(void);
void bsp_soft_reset(void);

#endif /* __BSP_H__ */
