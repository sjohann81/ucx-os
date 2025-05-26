/*
 * vim.c
 *
 *  Created on: 11 mai 2019
 *      Author: Alexy
 */

#include "bsp/vim.h"

#include "bsp/rti.h"

extern void defaultint(void);
extern void timer_int_wrapper(void);

typedef volatile struct vimRam
{
    t_isrFuncPTR ISR[VIM_CHANNELS];
} vimRAM_t;

#define vimRAM ((vimRAM_t *)0xFFF82000U)

static const t_isrFuncPTR vim_table[128U] =
{
    &defaultint,
    &defaultint,          /* Channel 0   */
    &defaultint,          /* Channel 1   */
    &timer_int_wrapper,          /* Channel 2   */
    &defaultint,          /* Channel 3   */
    &defaultint,          /* Channel 4   */
    &defaultint,          /* Channel 5   */
    &defaultint,          /* Channel 6   */
    &defaultint,          /* Channel 7   */
    &defaultint,          /* Channel 8   */
    &defaultint,          /* Channel 9   */
    &defaultint,          /* Channel 10  */
    &defaultint,          /* Channel 11  */
    &defaultint,          /* Channel 12  */
    &defaultint,          /* Channel 13  */
    &defaultint,          /* Channel 14  */
    &defaultint,          /* Channel 15  */
    &defaultint,          /* Channel 16  */
    &defaultint,          /* Channel 17  */
    &defaultint,          /* Channel 18  */
    &defaultint,          /* Channel 19  */
    &defaultint,          /* Channel 20  */
    &defaultint,          /* Channel 21  */
    &defaultint,          /* Channel 22  */
    &defaultint,          /* Channel 23  */
    &defaultint,          /* Channel 24  */
    &defaultint,          /* Channel 25  */
    &defaultint,          /* Channel 26  */
    &defaultint,          /* Channel 27  */
    &defaultint,          /* Channel 28  */
    &defaultint,          /* Channel 29  */
    &defaultint,          /* Channel 30  */
    &defaultint,          /* Channel 31  */
    &defaultint,          /* Channel 32  */
    &defaultint,          /* Channel 33  */
    &defaultint,          /* Channel 34  */
    &defaultint,          /* Channel 35  */
    &defaultint,          /* Channel 36  */
    &defaultint,          /* Channel 37  */
    &defaultint,          /* Channel 38  */
    &defaultint,          /* Channel 39  */
    &defaultint,          /* Channel 40  */
    &defaultint,          /* Channel 41  */
    &defaultint,          /* Channel 42  */
    &defaultint,          /* Channel 43  */
    &defaultint,          /* Channel 44  */
    &defaultint,          /* Channel 45  */
    &defaultint,          /* Channel 46  */
    &defaultint,          /* Channel 47  */
    &defaultint,          /* Channel 48  */
    &defaultint,          /* Channel 49  */
    &defaultint,          /* Channel 50  */
    &defaultint,          /* Channel 51  */
    &defaultint,          /* Channel 52  */
    &defaultint,          /* Channel 53  */
    &defaultint,          /* Channel 54  */
    &defaultint,          /* Channel 55  */
    &defaultint,          /* Channel 56  */
    &defaultint,          /* Channel 57  */
    &defaultint,          /* Channel 58  */
    &defaultint,          /* Channel 59  */
    &defaultint,          /* Channel 60  */
    &defaultint,          /* Channel 61  */
    &defaultint,          /* Channel 62  */
    &defaultint,          /* Channel 63  */
    &defaultint,          /* Channel 64  */
    &defaultint,          /* Channel 65  */
    &defaultint,          /* Channel 66  */
    &defaultint,          /* Channel 67  */
    &defaultint,          /* Channel 68  */
    &defaultint,          /* Channel 69  */
    &defaultint,          /* Channel 70  */
    &defaultint,          /* Channel 71  */
    &defaultint,          /* Channel 72  */
    &defaultint,          /* Channel 73  */
    &defaultint,          /* Channel 74  */
    &defaultint,          /* Channel 75  */
    &defaultint,          /* Channel 76  */
    &defaultint,          /* Channel 77  */
    &defaultint,          /* Channel 78  */
    &defaultint,          /* Channel 79  */
    &defaultint,          /* Channel 80  */
    &defaultint,          /* Channel 81  */
    &defaultint,          /* Channel 82  */
    &defaultint,          /* Channel 83  */
    &defaultint,          /* Channel 84  */
    &defaultint,          /* Channel 85  */
    &defaultint,          /* Channel 86  */
    &defaultint,          /* Channel 87  */
    &defaultint,          /* Channel 88  */
    &defaultint,          /* Channel 89  */
    &defaultint,          /* Channel 90  */
    &defaultint,          /* Channel 91  */
    &defaultint,          /* Channel 92  */
    &defaultint,          /* Channel 93  */
    &defaultint,          /* Channel 94  */
    &defaultint,          /* Channel 95  */
    &defaultint,          /* Channel 96  */
    &defaultint,          /* Channel 97  */
    &defaultint,          /* Channel 98  */
    &defaultint,          /* Channel 99  */
    &defaultint,          /* Channel 100 */
    &defaultint,          /* Channel 101 */
    &defaultint,          /* Channel 102 */
    &defaultint,          /* Channel 103 */
    &defaultint,          /* Channel 104 */
    &defaultint,          /* Channel 105 */
    &defaultint,          /* Channel 106 */
    &defaultint,          /* Channel 107 */
    &defaultint,          /* Channel 108 */
    &defaultint,          /* Channel 109 */
    &defaultint,          /* Channel 110 */
    &defaultint,          /* Channel 111 */
    &defaultint,          /* Channel 112 */
    &defaultint,          /* Channel 113 */
    &defaultint,          /* Channel 114 */
    &defaultint,          /* Channel 115 */
    &defaultint,          /* Channel 116 */
    &defaultint,          /* Channel 117 */
    &defaultint,          /* Channel 118 */
    &defaultint,          /* Channel 119 */
    &defaultint,          /* Channel 120 */
    &defaultint,          /* Channel 121 */
    &defaultint,          /* Channel 122 */
    &defaultint,          /* Channel 123 */
    &defaultint,          /* Channel 124 */
    &defaultint,          /* Channel 125 */
    &defaultint,          /* Channel 126 */
};

/* SourceId : VIM_SourceId_002 */
/* DesignId : VIM_DesignId_002 */
/* Requirements : HL_CONQ_VIM_SR5 */
/** @fn void vimChannelMap(uint32_t request, uint32_t channel, t_isrFuncPTR handler)
*   @brief Map selected interrupt request to the selected channel
*
*    @param[in] request: Interrupt request number 2..95
*    @param[in] channel: VIM Channel number 2..95
*    @param[in] handler: Address of the interrupt handler
*
*   This function will map selected interrupt request to the selected channel.
*
*/
void vimChannelMap(uint32_t request, uint32_t channel, t_isrFuncPTR handler)
{
    uint32_t i,j;

    i = channel >> 2U;              /* Find the register to configure */
    j = channel -(i<<2U);           /* Find the offset of the type    */
    j = 3U-j;                       /* reverse the byte order         */
    j = j<<3U;                      /* find the bit location          */

    /*Mapping the required interrupt request to the required channel*/
    vimREG->CHANCTRL[i] &= ~(uint32_t)((uint32_t)0xFFU << j);
    vimREG->CHANCTRL[i] |= (request << j);

    /*Updating VIMRAM*/
    vimRAM->ISR[channel + 1U] = handler;
}

/* SourceId : VIM_SourceId_003 */
/* DesignId : VIM_DesignId_003 */
/* Requirements : HL_CONQ_VIM_SR3 */
/** @fn void vimEnableInterrupt(uint32_t channel, systemInterrupt_t inttype)
*   @brief Enable interrupt for the the selected channel
*
*    @param[in] channel: VIM Channel number 2..95
*    @param[in] inttype: Interrupt type
*                        - SYS_IRQ: Selected channel will be enabled as IRQ
*                        - SYS_FIQ: Selected channel will be enabled as FIQ
*
*   This function will enable interrupt for the selected channel.
*
*/
void vimEnableInterrupt(uint32_t channel, systemInterrupt_t inttype)
{

    if (channel >= 96U)
    {
        if(inttype == SYS_IRQ)
        {
            vimREG->FIRQPR3 &= ~(uint32_t)((uint32_t)1U << (channel-96U));
        }
        else
        {
            vimREG->FIRQPR3 |= ((uint32_t)1U << (channel-96U));
        }
        vimREG->REQMASKSET3 = (uint32_t)1U << (channel-96U);
    }
    else if (channel >= 64U)
    {
        if(inttype == SYS_IRQ)
        {
            vimREG->FIRQPR2 &= ~(uint32_t)((uint32_t)1U << (channel-64U));
        }
        else
        {
            vimREG->FIRQPR2 |= ((uint32_t)1U << (channel-64U));
        }
        vimREG->REQMASKSET2 = (uint32_t)1U << (channel-64U);
    }
    else if (channel >= 32U)
    {
        if(inttype == SYS_IRQ)
        {
            vimREG->FIRQPR1 &= ~(uint32_t)((uint32_t)1U << (channel-32U));
        }
        else
        {
            vimREG->FIRQPR1 |= ((uint32_t)1U << (channel-32U));
        }
        vimREG->REQMASKSET1 = (uint32_t)1U << (channel-32U);
    }
    else if (channel >= 2U)
    {
        if(inttype == SYS_IRQ)
        {
            vimREG->FIRQPR0 &= ~(uint32_t)((uint32_t)1U << channel);
        }
        else
        {
            vimREG->FIRQPR0 |= ((uint32_t)1U << channel);
        }
        vimREG->REQMASKSET0 = (uint32_t)1U << channel;
    }
    else
    {
        /* Empty */
    }
}

/* SourceId : VIM_SourceId_004 */
/* DesignId : VIM_DesignId_004 */
/* Requirements : HL_CONQ_VIM_SR5 */
/** @fn void vimDisableInterrupt(uint32_t channel)
*   @brief Disable interrupt for the the selected channel
*
*    @param[in] channel: VIM Channel number 2..95
*
*   This function will disable interrupt for the selected channel.
*
*/
void vimDisableInterrupt(uint32_t channel)
{

    if (channel >= 96U)
    {
        vimREG->REQMASKCLR3 = (uint32_t)1U << (channel-96U);
    }
    else if (channel >= 64U)
    {
        vimREG->REQMASKCLR2 = (uint32_t)1U << (channel-64U);
    }
    else if (channel >=32U)
    {
        vimREG->REQMASKCLR1 = (uint32_t)1U << (channel-32U);
    }
    else if (channel >= 2U)
    {
        vimREG->REQMASKCLR0 = (uint32_t)1U << channel;
    }
    else
    {
        /* Empty */
    }
}

/* SourceId : VIM_SourceId_006 */
/* DesignId : VIM_DesignId_006 */
/* Requirements : HL_CONQ_VIM_SR6 */
void vimECCErrorHandler(void)
{
    while(1);
}

void init_vim(void)
{
    /* Initialize VIM table */
    {
        uint32_t i;

        for (i = 0U; i < VIM_CHANNELS; i++)
        {
            vimRAM->ISR[i] = vim_table[i];
        }
    }
    vimREG->FBVECADDR = (uint32_t)&vimECCErrorHandler;

    /* set IRQ/FIQ priorities */
    vimREG->FIRQPR0 = (uint32_t)((uint32_t)SYS_FIQ << 0U)
                    | (uint32_t)((uint32_t)SYS_FIQ << 1U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 2U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 3U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 4U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 5U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 6U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 7U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 8U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 9U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 10U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 11U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 12U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 13U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 14U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 15U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 16U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 17U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 18U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 19U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 20U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 21U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 22U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 23U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 24U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 25U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 26U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 27U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 28U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 29U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 30U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 31U);

    vimREG->FIRQPR1 = (uint32_t)((uint32_t)SYS_IRQ << 0U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 1U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 2U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 3U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 4U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 5U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 6U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 7U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 8U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 9U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 10U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 11U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 12U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 13U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 14U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 15U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 16U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 17U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 18U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 19U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 20U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 21U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 22U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 23U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 24U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 25U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 26U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 27U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 28U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 29U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 30U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 31U);


    vimREG->FIRQPR2 = (uint32_t)((uint32_t)SYS_IRQ << 0U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 1U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 2U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 3U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 4U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 5U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 6U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 7U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 8U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 9U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 10U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 11U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 12U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 13U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 14U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 15U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 16U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 17U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 18U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 19U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 20U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 21U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 22U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 23U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 24U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 25U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 26U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 27U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 28U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 29U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 30U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 31U);

    vimREG->FIRQPR3 = (uint32_t)((uint32_t)SYS_IRQ << 0U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 1U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 2U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 3U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 4U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 5U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 6U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 7U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 8U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 9U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 10U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 11U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 12U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 13U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 14U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 15U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 16U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 17U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 18U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 19U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 20U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 21U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 22U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 23U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 24U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 25U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 26U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 27U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 28U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 29U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 30U)
                    | (uint32_t)((uint32_t)SYS_IRQ << 31U);


    /* enable interrupts */
    vimREG->REQMASKSET0 = (uint32_t)((uint32_t)1U << 0U)
                        | (uint32_t)((uint32_t)1U << 1U)
                        | (uint32_t)((uint32_t)1U << 2U)
                        | (uint32_t)((uint32_t)0U << 3U)
                        | (uint32_t)((uint32_t)0U << 4U)
                        | (uint32_t)((uint32_t)0U << 5U)
                        | (uint32_t)((uint32_t)0U << 6U)
                        | (uint32_t)((uint32_t)0U << 7U)
                        | (uint32_t)((uint32_t)0U << 8U)
                        | (uint32_t)((uint32_t)0U << 9U)
                        | (uint32_t)((uint32_t)0U << 10U)
                        | (uint32_t)((uint32_t)0U << 11U)
                        | (uint32_t)((uint32_t)0U << 12U)
                        | (uint32_t)((uint32_t)0U << 13U)
                        | (uint32_t)((uint32_t)0U << 14U)
                        | (uint32_t)((uint32_t)0U << 15U)
                        | (uint32_t)((uint32_t)0U << 16U)
                        | (uint32_t)((uint32_t)0U << 17U)
                        | (uint32_t)((uint32_t)0U << 18U)
                        | (uint32_t)((uint32_t)0U << 19U)
                        | (uint32_t)((uint32_t)0U << 20U)
                        | (uint32_t)((uint32_t)0U << 21U)
                        | (uint32_t)((uint32_t)0U << 22U)
                        | (uint32_t)((uint32_t)0U << 23U)
                        | (uint32_t)((uint32_t)0U << 24U)
                        | (uint32_t)((uint32_t)0U << 25U)
                        | (uint32_t)((uint32_t)0U << 26U)
                        | (uint32_t)((uint32_t)0U << 27U)
                        | (uint32_t)((uint32_t)0U << 28U)
                        | (uint32_t)((uint32_t)0U << 29U)
                        | (uint32_t)((uint32_t)0U << 30U)
                        | (uint32_t)((uint32_t)0U << 31U);

    vimREG->REQMASKSET1 = (uint32_t)((uint32_t)0U << 0U)
                        | (uint32_t)((uint32_t)0U << 1U)
                        | (uint32_t)((uint32_t)0U << 2U)
                        | (uint32_t)((uint32_t)0U << 3U)
                        | (uint32_t)((uint32_t)0U << 4U)
                        | (uint32_t)((uint32_t)0U << 5U)
                        | (uint32_t)((uint32_t)0U << 6U)
                        | (uint32_t)((uint32_t)0U << 7U)
                        | (uint32_t)((uint32_t)0U << 8U)
                        | (uint32_t)((uint32_t)0U << 9U)
                        | (uint32_t)((uint32_t)0U << 10U)
                        | (uint32_t)((uint32_t)0U << 11U)
                        | (uint32_t)((uint32_t)0U << 12U)
                        | (uint32_t)((uint32_t)0U << 13U)
                        | (uint32_t)((uint32_t)0U << 14U)
                        | (uint32_t)((uint32_t)0U << 15U)
                        | (uint32_t)((uint32_t)0U << 16U)
                        | (uint32_t)((uint32_t)0U << 17U)
                        | (uint32_t)((uint32_t)0U << 18U)
                        | (uint32_t)((uint32_t)0U << 19U)
                        | (uint32_t)((uint32_t)0U << 20U)
                        | (uint32_t)((uint32_t)0U << 21U)
                        | (uint32_t)((uint32_t)0U << 22U)
                        | (uint32_t)((uint32_t)0U << 23U)
                        | (uint32_t)((uint32_t)0U << 24U)
                        | (uint32_t)((uint32_t)0U << 25U)
                        | (uint32_t)((uint32_t)0U << 26U)
                        | (uint32_t)((uint32_t)0U << 27U)
                        | (uint32_t)((uint32_t)0U << 28U)
                        | (uint32_t)((uint32_t)0U << 29U)
                        | (uint32_t)((uint32_t)0U << 30U)
                        | (uint32_t)((uint32_t)0U << 31U);

    vimREG->REQMASKSET2 = (uint32_t)((uint32_t)0U << 0U)
                        | (uint32_t)((uint32_t)0U << 1U)
                        | (uint32_t)((uint32_t)0U << 2U)
                        | (uint32_t)((uint32_t)0U << 3U)
                        | (uint32_t)((uint32_t)0U << 4U)
                        | (uint32_t)((uint32_t)0U << 5U)
                        | (uint32_t)((uint32_t)0U << 6U)
                        | (uint32_t)((uint32_t)0U << 7U)
                        | (uint32_t)((uint32_t)0U << 8U)
                        | (uint32_t)((uint32_t)0U << 9U)
                        | (uint32_t)((uint32_t)0U << 10U)
                        | (uint32_t)((uint32_t)0U << 11U)
                        | (uint32_t)((uint32_t)0U << 12U)
                        | (uint32_t)((uint32_t)0U << 13U)
                        | (uint32_t)((uint32_t)0U << 14U)
                        | (uint32_t)((uint32_t)0U << 15U)
                        | (uint32_t)((uint32_t)0U << 16U)
                        | (uint32_t)((uint32_t)0U << 17U)
                        | (uint32_t)((uint32_t)0U << 18U)
                        | (uint32_t)((uint32_t)0U << 19U)
                        | (uint32_t)((uint32_t)0U << 20U)
                        | (uint32_t)((uint32_t)0U << 21U)
                        | (uint32_t)((uint32_t)0U << 22U)
                        | (uint32_t)((uint32_t)0U << 23U)
                        | (uint32_t)((uint32_t)0U << 24U)
                        | (uint32_t)((uint32_t)0U << 25U)
                        | (uint32_t)((uint32_t)0U << 26U)
                        | (uint32_t)((uint32_t)0U << 27U)
                        | (uint32_t)((uint32_t)0U << 28U)
                        | (uint32_t)((uint32_t)0U << 29U)
                        | (uint32_t)((uint32_t)0U << 30U)
                        | (uint32_t)((uint32_t)0U << 31U);

    vimREG->REQMASKSET3 = (uint32_t)((uint32_t)0U << 0U)
                        | (uint32_t)((uint32_t)0U << 1U)
                        | (uint32_t)((uint32_t)0U << 2U)
                        | (uint32_t)((uint32_t)0U << 3U)
                        | (uint32_t)((uint32_t)0U << 4U)
                        | (uint32_t)((uint32_t)0U << 5U)
                        | (uint32_t)((uint32_t)0U << 6U)
                        | (uint32_t)((uint32_t)0U << 7U)
                        | (uint32_t)((uint32_t)0U << 8U)
                        | (uint32_t)((uint32_t)0U << 9U)
                        | (uint32_t)((uint32_t)0U << 10U)
                        | (uint32_t)((uint32_t)0U << 11U)
                        | (uint32_t)((uint32_t)0U << 12U)
                        | (uint32_t)((uint32_t)0U << 13U)
                        | (uint32_t)((uint32_t)0U << 14U)
                        | (uint32_t)((uint32_t)0U << 15U)
                        | (uint32_t)((uint32_t)0U << 16U)
                        | (uint32_t)((uint32_t)0U << 17U)
                        | (uint32_t)((uint32_t)0U << 18U)
                        | (uint32_t)((uint32_t)0U << 19U)
                        | (uint32_t)((uint32_t)0U << 20U)
                        | (uint32_t)((uint32_t)0U << 21U)
                        | (uint32_t)((uint32_t)0U << 22U)
                        | (uint32_t)((uint32_t)0U << 23U)
                        | (uint32_t)((uint32_t)0U << 24U)
                        | (uint32_t)((uint32_t)0U << 25U)
                        | (uint32_t)((uint32_t)0U << 26U)
                        | (uint32_t)((uint32_t)0U << 27U)
                        | (uint32_t)((uint32_t)0U << 28U)
                        | (uint32_t)((uint32_t)0U << 29U)
                        | (uint32_t)((uint32_t)0U << 30U)
                        | (uint32_t)((uint32_t)0U << 31U);

    /* Set Capture event sources */
    vimREG->CAPEVT = ((uint32_t)((uint32_t)0U << 0U)
                     |(uint32_t)((uint32_t)0U << 16U));
}
