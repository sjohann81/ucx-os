/*
 * bsp.c
 *
 *  Created on: 10 mai 2019
 *      Author: Alexy
 */

#include <bsp.h>

#include <core/time.h>
#include <interrupt.h>
#include <space.h>

#include <bsp/rti.h>
#include <bsp/serial.h>
#include <bsp/vim.h>

#include <asp/entries.h>
#include <core/debug.h>

extern void __TI_auto_init(void);

static void init_periph(void)
{
    /** - Disable Peripherals before peripheral powerup*/
    SYS_REG_1_BASE->CLKCNTL &= 0xFFFFFEFF;

    /** - Release peripherals from reset and enable clocks to all peripherals */
    /** - Power-up all peripherals */
    PCR_REG_1->PSPWRDWNCLR0 = 0xFFFFFFFF;
    PCR_REG_1->PSPWRDWNCLR1 = 0xFFFFFFFF;
    PCR_REG_1->PSPWRDWNCLR2 = 0xFFFFFFFF;
    PCR_REG_1->PSPWRDWNCLR3 = 0xFFFFFFFF;
    PCR_REG_2->PSPWRDWNCLR0 = 0xFFFFFFFF;
    PCR_REG_2->PSPWRDWNCLR1 = 0xFFFFFFFF;
    PCR_REG_2->PSPWRDWNCLR2 = 0xFFFFFFFF;
    PCR_REG_2->PSPWRDWNCLR3 = 0xFFFFFFFF;
    PCR_REG_3->PSPWRDWNCLR0 = 0xFFFFFFFF;
    PCR_REG_3->PSPWRDWNCLR1 = 0xFFFFFFFF;
    PCR_REG_3->PSPWRDWNCLR2 = 0xFFFFFFFF;
    PCR_REG_3->PSPWRDWNCLR3 = 0xFFFFFFFF;

    /** - Enable Peripherals */
    SYS_REG_1_BASE->CLKCNTL |= 0x00000100U;
}

static void init_pll(void)
{
    /* Disable PLL1 and PLL2 */
    SYS_REG_1_BASE->CSDISSET = 0x00000002U | 0x00000040U;
    while((SYS_REG_1_BASE->CSDIS & 0x42U) != 0x42U)
    {
    /* Wait */
    }

    /* Clear Global Status Register */
    SYS_REG_1_BASE->GBLSTAT = 0x301U;

    /** - Configure PLL control registers */
    /** @b Initialize @b Pll1: */

    /**   - Setup pll control register 1:
    *     - Setup reset on oscillator slip
    *     - Setup bypass on pll slip
    *     - setup Pll output clock divider to max before Lock
    *     - Setup reset on oscillator fail
    *     - Setup reference clock divider
    *     - Setup Pll multiplier
    */
    SYS_REG_1_BASE->PLLCTL1 =  (uint32_t)0x00000000U
                        |  (uint32_t)0x20000000U
                        |  (uint32_t)((uint32_t)0x1FU << 24U)
                        |  (uint32_t)0x00000000U
                        |  (uint32_t)((uint32_t)(8U - 1U)<< 16U)
                        |  (uint32_t)(0x9500U);

    /**   - Setup pll control register 2
    *     - Setup spreading rate
    *     - Setup bandwidth adjustment
    *     - Setup internal Pll output divider
    *     - Setup spreading amount
    */
    SYS_REG_1_BASE->PLLCTL2 =  (uint32_t)((uint32_t)255U << 22U)
                        |  (uint32_t)((uint32_t)7U << 12U)
                        |  (uint32_t)((uint32_t)(1U - 1U) << 9U)
                        |  (uint32_t)61U;

    /** @b Initialize @b Pll2: */

    /**   - Setup pll2 control register :
    *     - setup Pll output clock divider to max before Lock
    *     - Setup reference clock divider
    *     - Setup internal Pll output divider
    *     - Setup Pll multiplier
    */
    SYS_REG_2_BASE->PLLCTL3 = (uint32_t)((uint32_t)(1U - 1U) << 29U)
                        | (uint32_t)((uint32_t)0x1FU << 24U)
                        | (uint32_t)((uint32_t)(8U - 1U)<< 16U)
                        | (uint32_t)(0x9500U);

    /** - Enable PLL(s) to start up or Lock */
    SYS_REG_1_BASE->CSDIS = 0x00000000U
                      | 0x00000000U
                      | 0x00000008U
                      | 0x00000080U
                      | 0x00000000U
                      | 0x00000000U
                      | 0x00000000U
                      | 0x00000004U;
}

static void init_flash(void)
{
    /** - Setup flash read mode, address wait states and data wait states */
    FLASH_REG->FRDCNTL =  0x00000000U
                       | (uint32_t)((uint32_t)3U << 8U)
                       |  3U;

    /** - Setup flash access wait states for bank 7 */
    FSM_WR_ENA_HL    = 0x5U;
    EEPROM_CONFIG_HL = 0x00000002U
                     | (uint32_t)((uint32_t)9U << 16U);

    /** - Disable write access to flash state machine registers */
    FSM_WR_ENA_HL    = 0x2U;

    /** - Setup flash bank power modes */
    FLASH_REG->FBPWRMODE = 0x00000000U
                          | (uint32_t)((uint32_t)SYS_ACTIVE << 14U) /* BANK 7 */
                          | (uint32_t)((uint32_t)SYS_ACTIVE << 2U)  /* BANK 1 */
                          | (uint32_t)((uint32_t)SYS_ACTIVE << 0U); /* BANK 0 */
}

static void init_clock(void)
{
    uint32_t SYS_CSVSTAT, SYS_CSDIS;

    /** @b Initialize @b Clock @b Tree: */
    /** - Setup system clock divider for HCLK */
    SYS_REG_2_BASE->HCLKCNTL = 1U;

    /** - Disable / Enable clock domain */
    SYS_REG_1_BASE->CDDIS = (uint32_t)((uint32_t)0U << 4U ) /* AVCLK1 , 1 - OFF, 0 - ON */
                      | (uint32_t)((uint32_t)1U << 5U ) /* AVCLK2 , 1 - OFF, 0 - ON */
                      | (uint32_t)((uint32_t)0U << 8U ) /* VCLK3 , 1 - OFF, 0 - ON */
                      | (uint32_t)((uint32_t)0U << 9U ) /* VCLK4 , 1 - OFF, 0 - ON */
                      | (uint32_t)((uint32_t)0U << 10U) /* AVCLK3 , 1 - OFF, 0 - ON */
                      | (uint32_t)((uint32_t)0U << 11U); /* AVCLK4 , 1 - OFF, 0 - ON */


    /* Always check the CSDIS register to make sure the clock source is turned on and check
     * the CSVSTAT register to make sure the clock source is valid. Then write to GHVSRC to switch the clock.
     */
    /** - Wait for until clocks are locked */
    SYS_CSVSTAT = SYS_REG_1_BASE->CSVSTAT;
    SYS_CSDIS = SYS_REG_1_BASE->CSDIS;
    while ((SYS_CSVSTAT & ((SYS_CSDIS ^ 0xFFU) & 0xFFU)) != ((SYS_CSDIS ^ 0xFFU) & 0xFFU))
    {
        SYS_CSVSTAT = SYS_REG_1_BASE->CSVSTAT;
        SYS_CSDIS = SYS_REG_1_BASE->CSDIS;
    } /* Wait */

    /** - Map device clock domains to desired sources and configure top-level dividers */
    /** - All clock domains are working off the default clock sources until now */
    /** - The below assignments can be easily modified using the HALCoGen GUI */

    /** - Setup GCLK, HCLK and VCLK clock source for normal operation, power down mode and after wakeup */
    SYS_REG_1_BASE->GHVSRC = (uint32_t)((uint32_t)SYS_PLL1 << 24U)
                       | (uint32_t)((uint32_t)SYS_PLL1 << 16U)
                       | (uint32_t)((uint32_t)SYS_PLL1 << 0U);

    /** - Setup RTICLK1 and RTICLK2 clocks */
    SYS_REG_1_BASE->RCLKSRC = (uint32_t)((uint32_t)1U << 24U)        /* RTI2 divider (Not applicable for lock-step device)  */
                        | (uint32_t)((uint32_t)SYS_VCLK << 16U) /* RTI2 clock source (Not applicable for lock-step device) */
                        | (uint32_t)((uint32_t)1U << 8U)         /* RTI1 divider */
                        | (uint32_t)((uint32_t)SYS_VCLK << 0U); /* RTI1 clock source */

    /** - Setup asynchronous peripheral clock sources for AVCLK1 and AVCLK2 */
    SYS_REG_1_BASE->VCLKASRC = (uint32_t)((uint32_t)SYS_VCLK << 8U)
                         | (uint32_t)((uint32_t)SYS_VCLK << 0U);

    /** - Setup synchronous peripheral clock dividers for VCLK1, VCLK2, VCLK3 */
    SYS_REG_1_BASE->CLKCNTL  = (SYS_REG_1_BASE->CLKCNTL & 0xF0FFFFFFU)
                         | (uint32_t)((uint32_t)1U << 24U);
    SYS_REG_1_BASE->CLKCNTL  = (SYS_REG_1_BASE->CLKCNTL & 0xFFF0FFFFU)
                         | (uint32_t)((uint32_t)1U << 16U);

    SYS_REG_2_BASE->CLK2CNTRL = (SYS_REG_2_BASE->CLK2CNTRL & 0xFFFFFFF0U)
                         | (uint32_t)((uint32_t)1U << 0U);

    SYS_REG_2_BASE->VCLKACON1 =  (uint32_t)((uint32_t)(1U - 1U) << 24U)
                           | (uint32_t)((uint32_t)0U << 20U)
                           | (uint32_t)((uint32_t)SYS_VCLK << 16U)
                           | (uint32_t)((uint32_t)(1U - 1U) << 8U)
                           | (uint32_t)((uint32_t)0U << 4U)
                           | (uint32_t)((uint32_t)SYS_VCLK << 0U);

    /* Now the PLLs are locked and the PLL outputs can be sped up */
    /* The R-divider was programmed to be 0xF. Now this divider is changed to programmed value */
    SYS_REG_1_BASE->PLLCTL1 = (SYS_REG_1_BASE->PLLCTL1 & 0xE0FFFFFFU) | (uint32_t)((uint32_t)(1U - 1U) << 24U);
    /*SAFETYMCUSW 134 S MR:12.2 <APPROVED> " Clear and write to the volatile register " */
    SYS_REG_2_BASE->PLLCTL3 = (SYS_REG_2_BASE->PLLCTL3 & 0xE0FFFFFFU) | (uint32_t)((uint32_t)(1U - 1U) << 24U);

    /* Enable/Disable Frequency modulation */
    SYS_REG_1_BASE->PLLCTL2 |= 0x00000000U;
}

static void init_pins(void)
{
    /** - set ECLK pins functional mode */
    SYS_REG_1_BASE->SYSPC1 = 0U;

    /** - set ECLK pins default output value */
    SYS_REG_1_BASE->SYSPC4 = 0U;

    /** - set ECLK pins output direction */
    SYS_REG_1_BASE->SYSPC2 = 1U;

    /** - set ECLK pins open drain enable */
    SYS_REG_1_BASE->SYSPC7 = 0U;

    /** - set ECLK pins pullup/pulldown enable */
    SYS_REG_1_BASE->SYSPC8 = 0U;

    /** - set ECLK pins pullup/pulldown select */
    SYS_REG_1_BASE->SYSPC9 = 1U;

    /** - Setup ECLK */
    SYS_REG_1_BASE->ECPCNTL = (uint32_t)((uint32_t)0U << 24U)
                        | (uint32_t)((uint32_t)0U << 23U)
                        | (uint32_t)((uint32_t)(8U - 1U) & 0xFFFFU);
}

static void init_lpo(void)
{
    uint32_t u32clocktestConfig;
    /* Save user clocktest register configuration */
    u32clocktestConfig = SYS_REG_1_BASE->CLKTEST;

    /*The TRM states OTP TRIM value should be stepped to avoid large changes in the HF LPO clock that would result in a LPOCLKMON fault. At issue is the TRM does not specify what the maximum step is so there is no metric to use for the SW implementation - the routine can temporarily disable the LPOCLKMON range check so the sudden change will not cause a fault.*/
    /* Disable clock range detection*/

    SYS_REG_1_BASE->CLKTEST = (SYS_REG_1_BASE->CLKTEST
                        | (uint32_t)((uint32_t)0x1U << 24U))
                        & (uint32_t)(~((uint32_t)0x1U << 25U));
    /*SAFETYMCUSW 139 S MR:13.7 <APPROVED> "Hardware status bit read check" */
    if(LPO_TRIM_VALUE != 0xFFFFU)
    {

        SYS_REG_1_BASE->LPOMONCTL  = (uint32_t)((uint32_t)1U << 24U)
                               | (uint32_t)((uint32_t)LPO_TRIM_VALUE);
    }
    else
    {

        SYS_REG_1_BASE->LPOMONCTL   = (uint32_t)((uint32_t)1U << 24U)
                                     | (uint32_t)((uint32_t)16U << 8U)
                                     | (uint32_t)((uint32_t)16U);

    }

    /* Restore the user clocktest register value configuration */
    SYS_REG_1_BASE->CLKTEST = u32clocktestConfig;

}

void ja_bsp_init(void)
{
    init_pll();
    init_periph();
    init_lpo();
    init_flash();
    init_clock();
    init_pins();
    init_serial();
    __TI_auto_init();
    serial_write("BSP Initialized\r\n", 17);

    init_vim();
    serial_write("VIM Initialized\r\n", 17);

    init_rti();
    serial_write("RTI Initialized\r\n", 17);

    jet_console_init_all();
    serial_write("Console Initialized\r\n", 21);

    rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE0);
    rtiSetPeriod(rtiREG1, rtiCOMPARE0, RTI_FREQUENCY / POK_TIMER_FREQUENCY);
    rtiStartCounter(rtiREG1, 0);
    rtiStartCounter(rtiREG1, 1);
    serial_write("Timers Initialized\r\n", 20);
}


void pok_bsp_get_info(void *addr) {
    pok_fatal("pok_bsp_get_info unimplemented on armv7");
}

void bsp_soft_reset(void)
{
    SYS_REG_1_BASE->SYSECR |= (1 << 15);
}
