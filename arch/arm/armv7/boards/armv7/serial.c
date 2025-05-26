/*
 * serial.c
 *
 *  Created on: 10 mai 2019
 *      Author: Alexy
 */

#include "bsp/serial.h"

void init_serial(void)
{
    /** @b initialize @b SCI1 */
    /** - bring SCI1 out of reset */
        sciREG1->GCR0 = 0U;
        sciREG1->GCR0 = 1U;

        /** - Disable all interrupts */
        sciREG1->CLEARINT    = 0xFFFFFFFFU;
        sciREG1->CLEARINTLVL = 0xFFFFFFFFU;

        /** - global control 1 */
        sciREG1->GCR1 =  (uint32_t)((uint32_t)1U << 25U)  /* enable transmit */
                      | (uint32_t)((uint32_t)1U << 24U)  /* enable receive */
                      | (uint32_t)((uint32_t)1U << 5U)   /* internal clock (device has no clock pin) */
                      | (uint32_t)((uint32_t)(2U-1U) << 4U)  /* number of stop bits */
                      | (uint32_t)((uint32_t)0U << 3U)  /* even parity, otherwise odd */
                      | (uint32_t)((uint32_t)0U << 2U)  /* enable parity */
                      | (uint32_t)((uint32_t)1U << 1U);  /* asynchronous timing mode */

        /** - set baudrate */
        sciREG1->BRS = 487U;  /* baudrate */

        /** - transmission length */
        sciREG1->FORMAT = 8U - 1U;  /* length */

        /** - set SCI1 pins functional mode */
        sciREG1->PIO0 = (uint32_t)((uint32_t)1U << 2U)  /* tx pin */
                     | (uint32_t)((uint32_t)1U << 1U); /* rx pin */

        /** - set SCI1 pins default output value */
        sciREG1->PIO3 = (uint32_t)((uint32_t)0U << 2U)  /* tx pin */
                     | (uint32_t)((uint32_t)0U << 1U); /* rx pin */

        /** - set SCI1 pins output direction */
        sciREG1->PIO1 = (uint32_t)((uint32_t)0U << 2U)  /* tx pin */
                     | (uint32_t)((uint32_t)0U << 1U); /* rx pin */

        /** - set SCI1 pins open drain enable */
        sciREG1->PIO6 = (uint32_t)((uint32_t)0U << 2U)  /* tx pin */
                     | (uint32_t)((uint32_t)0U << 1U); /* rx pin */

        /** - set SCI1 pins pullup/pulldown enable */
        sciREG1->PIO7 = (uint32_t)((uint32_t)0U << 2U)  /* tx pin */
                     | (uint32_t)((uint32_t)0U << 1U); /* rx pin */

        /** - set SCI1 pins pullup/pulldown select */
        sciREG1->PIO8 = (uint32_t)((uint32_t)1U << 2U)  /* tx pin */
                     | (uint32_t)((uint32_t)1U << 1U);  /* rx pin */

        /** - set interrupt level */
        sciREG1->SETINTLVL = (uint32_t)((uint32_t)0U << 26U)  /* Framing error */
                          | (uint32_t)((uint32_t)0U << 25U)  /* Overrun error */
                          | (uint32_t)((uint32_t)0U << 24U)  /* Parity error */
                          | (uint32_t)((uint32_t)0U << 9U)  /* Receive */
                          | (uint32_t)((uint32_t)0U << 8U)  /* Transmit */
                          | (uint32_t)((uint32_t)0U << 1U)  /* Wakeup */
                          | (uint32_t)((uint32_t)0U << 0U);  /* Break detect */

        /** - set interrupt enable */
        sciREG1->SETINT = (uint32_t)((uint32_t)0U << 26U)  /* Framing error */
                       | (uint32_t)((uint32_t)0U << 25U)  /* Overrun error */
                       | (uint32_t)((uint32_t)0U << 24U)  /* Parity error */
                       | (uint32_t)((uint32_t)0U << 9U)  /* Receive */
                       | (uint32_t)((uint32_t)0U << 1U)  /* Wakeup */
                       | (uint32_t)((uint32_t)0U << 0U);  /* Break detect */

        /** - Finaly start SCI1 */
        sciREG1->GCR1 |= 0x80U;
}

void serial_put_byte(char byte)
{
    while ((sciREG1->FLR & (uint32_t)SCI_TX_INT) == 0U)
    {
    } /* Wait */

    sciREG1->TD = byte;
}

void serial_write(const char* string, uint32_t size)
{
    uint32_t i;
    for(i = 0; i < size; ++i)
    {
        while ((sciREG1->FLR & (uint32_t)SCI_TX_INT) == 0U)
        {
        } /* Wait */

        sciREG1->TD = *string++;
    }
}
