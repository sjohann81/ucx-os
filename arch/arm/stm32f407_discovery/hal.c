/* file:          hal.c
 * description:   hardware abstraction layer (HAL) definitions ARM (stm32)
 * date:          05/2023
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <hal.h>
#include <usart.h>
#include <lib/libc.h>
#include <lib/dump.h>
#include <kernel/kernel.h>
#include <jiffies.h>

#ifdef USB_SERIAL

#include "usbd_cdc_vcp.h"

USB_OTG_CORE_HANDLE USB_OTG_dev;

#endif

/*
libc basic I/O support
*/

void _putchar(char value)
{
#ifdef USB_SERIAL
	VCP_putchar(value);
#else
	uart_tx(1, value);
#endif
}

int32_t _kbhit(void)
{
#ifdef USB_SERIAL
	return VCP_kbhit();
#else
	if (uart_rxsize(1) > 0)
		return 1;
	else
		return 0;
#endif
}

int32_t _getchar(void)
{
#ifdef USB_SERIAL
	uint8_t value;
	
	while (1) {
		if (VCP_getchar(&value))
			return value;
	}
#else
	return uart_rx(1);
#endif
}


/* delay routines */
void _delay_ms(uint32_t msec)
{
	jf_delay_us(1000 * msec);
}

void _delay_us(uint32_t usec)
{
	jf_delay_us(usec);
}

static uint64_t timeref = 0;

uint64_t _read_us(void)
{
	return timeref;
}


/* hardware dependent stuff and interrupt management */

extern int main(void);

void Dummy_Handler(void);
void Reset_Handler(void);
void SysTick_Handler(void);
void NMI_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void HardFault_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void MemManage_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void BusFault_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void UsageFault_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void SVC_Handler(void);// __attribute__ ((weak, alias ("Dummy_Handler")));
void DebugMon_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void PendSV_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));

void WWDG_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void PVD_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TAMP_STAMP_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void RTC_WKUP_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void FLASH_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void RCC_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void EXTI0_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void EXTI1_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void EXTI2_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void EXTI3_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void EXTI4_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Stream0_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Stream1_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Stream2_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Stream3_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Stream4_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Stream5_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Stream6_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void ADC_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void CAN1_TX_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void CAN1_RX0_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void CAN1_RX1_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void CAN1_SCE_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void EXTI9_5_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM1_BRK_TIM9_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM1_UP_TIM10_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM1_TRG_COM_TIM11_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM1_CC_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM2_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM3_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM4_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void I2C1_EV_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void I2C1_ER_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void I2C2_EV_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void I2C2_ER_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void SPI1_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void SPI2_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void USART1_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void USART2_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void USART3_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void EXTI15_10_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void RTC_Alarm_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void OTG_FS_WKUP_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM8_BRK_TIM12_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM8_UP_TIM13_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM8_TRG_COM_TIM14_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM8_CC_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Stream7_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void FSMC_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void SDIO_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM5_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void SPI3_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void UART4_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void UART5_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM6_DAC_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM7_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA2_Stream0_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA2_Stream1_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA2_Stream2_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA2_Stream3_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA2_Stream4_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void ETH_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void ETH_WKUP_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void CAN2_TX_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void CAN2_RX0_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void CAN2_RX1_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void CAN2_SCE_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void OTG_FS_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA2_Stream5_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA2_Stream6_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA2_Stream7_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void USART6_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void I2C3_EV_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void I2C3_ER_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void OTG_HS_EP1_OUT_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void OTG_HS_EP1_IN_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void OTG_HS_WKUP_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void OTG_HS_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DCMI_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void CRYP_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void HASH_RNG_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void FPU_IRQHandler(void) __attribute__ ((weak, alias ("Dummy_Handler")));


/* Exception vector table */

__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
	/* Predefined exception types of the ARM Cortex-M3/4 core. */
	(void (*)(void)) &_stack_start,
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0,                                 /* Reserved */
	0,                                 /* Reserved */
	0,                                 /* Reserved */
	0,                                 /* Reserved */
	SVC_Handler,
	DebugMon_Handler,
	0,                                 /* Reserved */
	PendSV_Handler,
	SysTick_Handler,

	/* External interrupts - IRQs (0 - 239)
	* The number of external interrupt inputs is defined by chip manufacturers. A maximum of 240 external interrupt inputs can
	* be supported. In addition, the Cortex-M3/4 also has an NMI interrupt input. When it is asserted, the NMI-ISR is executed
	* unconditionally.
	*/

	/* External Interrupts */
	WWDG_IRQHandler,                   /* Window WatchDog              */
	PVD_IRQHandler,                    /* PVD through EXTI Line detection */
	TAMP_STAMP_IRQHandler,             /* Tamper and TimeStamps through the EXTI line */
	RTC_WKUP_IRQHandler,               /* RTC Wakeup through the EXTI line */
	FLASH_IRQHandler,                  /* FLASH                        */
	RCC_IRQHandler,                    /* RCC                          */
	EXTI0_IRQHandler,                  /* EXTI Line0                   */
	EXTI1_IRQHandler,                  /* EXTI Line1                   */
	EXTI2_IRQHandler,                  /* EXTI Line2                   */
	EXTI3_IRQHandler,                  /* EXTI Line3                   */
	EXTI4_IRQHandler,                  /* EXTI Line4                   */
	DMA1_Stream0_IRQHandler,           /* DMA1 Stream 0                */
	DMA1_Stream1_IRQHandler,           /* DMA1 Stream 1                */
	DMA1_Stream2_IRQHandler,           /* DMA1 Stream 2                */
	DMA1_Stream3_IRQHandler,           /* DMA1 Stream 3                */
	DMA1_Stream4_IRQHandler,           /* DMA1 Stream 4                */
	DMA1_Stream5_IRQHandler,           /* DMA1 Stream 5                */
	DMA1_Stream6_IRQHandler,           /* DMA1 Stream 6                */
	ADC_IRQHandler,                    /* ADC1, ADC2 and ADC3s         */
	CAN1_TX_IRQHandler,                /* CAN1 TX                      */
	CAN1_RX0_IRQHandler,               /* CAN1 RX0                     */
	CAN1_RX1_IRQHandler,               /* CAN1 RX1                     */
	CAN1_SCE_IRQHandler,               /* CAN1 SCE                     */
	EXTI9_5_IRQHandler,                /* External Line[9:5]s          */
	TIM1_BRK_TIM9_IRQHandler,          /* TIM1 Break and TIM9          */
	TIM1_UP_TIM10_IRQHandler,          /* TIM1 Update and TIM10        */
	TIM1_TRG_COM_TIM11_IRQHandler,     /* TIM1 Trigger and Commutation and TIM11 */
	TIM1_CC_IRQHandler,                /* TIM1 Capture Compare         */
	TIM2_IRQHandler,                   /* TIM2                         */
	TIM3_IRQHandler,                   /* TIM3                         */
	TIM4_IRQHandler,                   /* TIM4                         */
	I2C1_EV_IRQHandler,                /* I2C1 Event                   */
	I2C1_ER_IRQHandler,                /* I2C1 Error                   */
	I2C2_EV_IRQHandler,                /* I2C2 Event                   */
	I2C2_ER_IRQHandler,                /* I2C2 Error                   */
	SPI1_IRQHandler,                   /* SPI1                         */
	SPI2_IRQHandler,                   /* SPI2                         */
	USART1_IRQHandler,                 /* USART1                       */
	USART2_IRQHandler,                 /* USART2                       */
	USART3_IRQHandler,                 /* USART3                       */
	EXTI15_10_IRQHandler,              /* External Line[15:10]s        */
	RTC_Alarm_IRQHandler,              /* RTC Alarm (A and B) through EXTI Line */
	OTG_FS_WKUP_IRQHandler,            /* USB OTG FS Wakeup through EXTI line */
	TIM8_BRK_TIM12_IRQHandler,         /* TIM8 Break and TIM12         */
	TIM8_UP_TIM13_IRQHandler,          /* TIM8 Update and TIM13        */
	TIM8_TRG_COM_TIM14_IRQHandler,     /* TIM8 Trigger and Commutation and TIM14 */
	TIM8_CC_IRQHandler,                /* TIM8 Capture Compare         */
	DMA1_Stream7_IRQHandler,           /* DMA1 Stream7                 */
	FSMC_IRQHandler,                   /* FSMC                         */
	SDIO_IRQHandler,                   /* SDIO                         */
	TIM5_IRQHandler,                   /* TIM5                         */
	SPI3_IRQHandler,                   /* SPI3                         */
	UART4_IRQHandler,                  /* UART4                        */
	UART5_IRQHandler,                  /* UART5                        */
	TIM6_DAC_IRQHandler,               /* TIM6 and DAC1&2 underrun errors */
	TIM7_IRQHandler,                   /* TIM7                         */
	DMA2_Stream0_IRQHandler,           /* DMA2 Stream 0                */
	DMA2_Stream1_IRQHandler,           /* DMA2 Stream 1                */
	DMA2_Stream2_IRQHandler,           /* DMA2 Stream 2                */
	DMA2_Stream3_IRQHandler,           /* DMA2 Stream 3                */
	DMA2_Stream4_IRQHandler,           /* DMA2 Stream 4                */
	ETH_IRQHandler,                    /* Ethernet                     */
	ETH_WKUP_IRQHandler,               /* Ethernet Wakeup through EXTI line */
	CAN2_TX_IRQHandler,                /* CAN2 TX                      */
	CAN2_RX0_IRQHandler,               /* CAN2 RX0                     */
	CAN2_RX1_IRQHandler,               /* CAN2 RX1                     */
	CAN2_SCE_IRQHandler,               /* CAN2 SCE                     */
	OTG_FS_IRQHandler,                 /* USB OTG FS                   */
	DMA2_Stream5_IRQHandler,           /* DMA2 Stream 5                */
	DMA2_Stream6_IRQHandler,           /* DMA2 Stream 6                */
	DMA2_Stream7_IRQHandler,           /* DMA2 Stream 7                */
	USART6_IRQHandler,                 /* USART6                       */
	I2C3_EV_IRQHandler,                /* I2C3 event                   */
	I2C3_ER_IRQHandler,                /* I2C3 error                   */
	OTG_HS_EP1_OUT_IRQHandler,         /* USB OTG HS End Point 1 Out   */
	OTG_HS_EP1_IN_IRQHandler,          /* USB OTG HS End Point 1 In    */
	OTG_HS_WKUP_IRQHandler,            /* USB OTG HS Wakeup through EXTI */
	OTG_HS_IRQHandler,                 /* USB OTG HS                   */
	DCMI_IRQHandler,                   /* DCMI                         */
	CRYP_IRQHandler,                   /* CRYP crypto                  */
	HASH_RNG_IRQHandler,               /* Hash and Rng                 */
	FPU_IRQHandler                     /* FPU                          */
};

void Dummy_Handler(void)
{
}

void Reset_Handler(void)
{
	uint32_t *source;
	uint32_t *dest;

	/* Copy the initialized data of the ".data" segment from the FLASH to the area in the RAM. */
	source = &_sidata;
	dest = &_sdata;

	while (dest < &_edata) {
		*dest = *source;
		dest++;
		source++;
	}

	/* Clear the ".bss" segment */
	dest = &_sbss;

	while (dest < &_ebss)
		*dest++ = 0;

	/* The runtime environment is set. So, the main function is called */
	main();
	
	/* If it is necessary handling any error status from main function, it may be done here. */
	for (;;);
}

volatile uint32_t *task_psp, *new_task_psp;

void SysTick_Handler(void)
{
	static uint32_t tval2 = 0, tref = 0;

	// update microsecond counter
	if (jf_value() < tref) tval2++;
	tref = jf_value();
	timeref = ((uint64_t)tval2 << 16) + (uint64_t)jf_value();

	// save current PSP, call the scheduler and get new PSP
	task_psp = &kcb_p->tcb_p->context[CONTEXT_PSP];
	krnl_dispatcher();
	new_task_psp = &kcb_p->tcb_p->context[CONTEXT_PSP];
	
	/* trigger PendSV interrupt to perform a task schedule and context switch */
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}


/* system call interface */

void syscall(void (*func)(void *), void *args) __attribute__((optimize("1")));

void syscall(void (*func)(void *), void *args){
     // by convention func is in r0 and args is in r1
     asm volatile("svc 11");
}

typedef void (*svcall_t)(void *);

void SVC_Handler(void)
{
	register uint32_t *frame;
	register svcall_t call;
	register void *args;
	// assumes PSP in use when service_call() invoked
	asm volatile ("MRS %0, psp\n\t" : "=r" (frame));
	call = (svcall_t)frame[0];
	args = (void *)(frame[1]);
	call(args);
}


/* kernel auxiliary routines */

void _cpu_idle(void)
{
}

static void _stack_check(void)
{
	uint32_t check = 0x33333333;
	uint32_t *stack_p = (uint32_t *)kcb_p->tcb_p->stack;
	
	if (*stack_p != check) {
		ucx_hexdump((void *)kcb_p->tcb_p->stack, kcb_p->tcb_p->stack_sz);
		printf("\n*** HALT - task %d, stack %08x (%d) check failed\n", kcb_p->tcb_p->id,
			(uint32_t)kcb_p->tcb_p->stack, (uint32_t)kcb_p->tcb_p->stack_sz);
		for (;;);
	}
		
}

static void _delay_update(void)
{
	struct tcb_s *tcb_ptr = kcb_p->tcb_first;
	
	for (;;	tcb_ptr = tcb_ptr->tcb_next) {
		if (tcb_ptr->state == TASK_BLOCKED && tcb_ptr->delay > 0) {
			tcb_ptr->delay--;
			if (tcb_ptr->delay == 0)
				tcb_ptr->state = TASK_READY;
		}
		if (tcb_ptr->tcb_next == kcb_p->tcb_first) break;
	}
}

void _dispatch(void)
{
	_delay_update();
	_stack_check();
	krnl_schedule();
}

void _yield(void)
{
	void (*sysfunc)(void *) = (void *)(void *)SysTick_Handler;

	syscall(sysfunc, 0);
}

void _hardware_init(void)
{
	uint32_t ticks;
	
	/* Call the SystemInit code from CMSIS interface */
	SystemInit();
	SystemCoreClockUpdate();

	/* custom Systick initialization (timer mask disabled) */
	ticks = SystemCoreClock / F_TIMER;
	SysTick->LOAD = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;
	NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
	
	/* set PendSV interrupt for the lowest priority */
	NVIC_SetPriority(PendSV_IRQn, 0xFF);

	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* GPIOC Peripheral clock enable. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/* configure board LED as output */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* setup TIM11 for jiffies */
	jf_setup();

#ifdef USB_SERIAL
	/* GPIOA Peripheral clock enable. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Configure PA8 in output pushpull mode. */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_15);
	_delay_ms(100);

	USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);

	char buf[80];
	
	/* wait from user data terminal to boot */
	gets(buf);
#else
	/* configure USART 1 */
	uart_init(1, 57600, 0);
#endif
	/* turn board LED off */
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void _dispatch_init(jmp_buf env)
{
	uint32_t *ctx_p;
	
	ctx_p = (uint32_t *)env;
	// Set PSP to top of task 0 stack
	__set_PSP((ctx_p[CONTEXT_PSP] + 16*4));
	// Switch to use Process Stack, unprivileged state
	__set_CONTROL(0x3);
	// Execute ISB after changing CONTROL (architectural recommendation)
	__ISB();
	
	kcb_p->tcb_p->task();
}

void _di(void)
{
	asm volatile (	"cpsid i\n\t");
}

void _ei(void)
{
	asm volatile (	"cpsie i\n\t"
			"isb\n\t");
}

void _timer_enable(void)
{
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void _timer_disable(void)
{
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void _context_init(jmp_buf *ctx, size_t sp, size_t ss, size_t ra)
{
	uint32_t *ctx_p;
	uint32_t *ptr;
	
	ctx_p = (uint32_t *)ctx;
	
	ctx_p[CONTEXT_SP] = sp + ss;
	ctx_p[CONTEXT_RA] = ra;
	
	ctx_p[CONTEXT_PSP] = sp + ss - 16*4;
	ptr = (uint32_t *)ctx_p[CONTEXT_PSP];
	*(ptr + 14) = ra;
	*(ptr + 15) = 0x01000000;
}
