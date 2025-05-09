/* file:          hal.c
 * description:   hardware abstraction layer (HAL) definitions ARM (stm32)
 * date:          05/2023
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <hal.h>
#include <console.h>
#include <usart.h>
#include <lib/libc.h>
#include <lib/dump.h>
#include <lib/list.h>
#include <kernel/kernel.h>
#include <kernel/ecodes.h>


/*
libc basic I/O support
*/
static int __putchar(int value)		// polled putchar()
{
	usart_tx(USART_PORT, value);
	
	return value;
}

static int __kbhit(void)
{
	if (usart_rxsize(USART_PORT) > 0)
		return 1;
	else
		return 0;
}

static int __getchar(void)			// polled getch()
{
	return usart_rx(USART_PORT);
}

/* timer setup for delays */
static void tim11_config()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	/* Enable clock for TIM11 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
	
	/* preset default values of the timer struct */
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	
	/* TIM11 value of prescaler, period and mode */
	TIM_TimeBaseInitStruct.TIM_Prescaler = SystemCoreClock / 1000000;
	TIM_TimeBaseInitStruct.TIM_Period = 65535;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	/* TIM11 initialize */
	TIM_TimeBaseInit(TIM11, &TIM_TimeBaseInitStruct);
}

static void tim11_start()
{
	/* Stop TIM11 */
	TIM_Cmd(TIM11, DISABLE);
	TIM11->CNT = 0;
	/* Start TIM11 */
	TIM_Cmd(TIM11, ENABLE);
}

/* delay routines */
void _delay_ms(uint32_t msec)
{
	volatile uint64_t start;

	start = _read_us();
	while ((_read_us() - start) < (uint64_t)msec * 1000);
}

void _delay_us(uint32_t usec)
{
	volatile uint64_t start;

	start = _read_us();
	while ((_read_us() - start) < (uint64_t)usec);
}

uint64_t _read_us(void)
{
	static uint64_t timeref = 0;
	static uint32_t tval2 = 0;
	static uint16_t tref = 0;
	volatile uint16_t time = TIM11->CNT;

	if (time < tref) tval2++;
	tref = time;
	timeref = ((uint64_t)tval2 << 16) + (uint64_t)time;

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
	struct tcb_s *task = kcb->task_current->data;

	// update microsecond counter
	_read_us();

	// save current PSP, call the scheduler and get new PSP
	task_psp = &task->context[CONTEXT_PSP];
	krnl_dispatcher();
	task = kcb->task_current->data;
	new_task_psp = &task->context[CONTEXT_PSP];
	
	/* trigger PendSV interrupt to perform a task schedule and context switch */
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/* system call interface */
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
	struct tcb_s *task = kcb->task_current->data;
	uint32_t check = 0x33333333;
	uint32_t *stack_p = (uint32_t *)task->stack;

	if (*stack_p != check) {
		hexdump((void *)task->stack, task->stack_sz);
		printf("\n*** task %d, stack: 0x%p (size %d)\n", task->id,
			task->stack, task->stack_sz);
		krnl_panic(ERR_STACK_CHECK);
	}
		
}

static struct node_s *_delay_update(struct node_s *node, void *arg)
{
	struct tcb_s *task = node->data;
	
	if (task->state == TASK_BLOCKED && task->delay > 0) {
		task->delay--;
		if (task->delay == 0)
			task->state = TASK_READY;
	}
	
	return 0;
}

void _dispatch(void)
{
	if (!kcb->tasks->length)
		krnl_panic(ERR_NO_TASKS);
		
	_stack_check();
	list_foreach(kcb->tasks, _delay_update, (void *)0);
	krnl_schedule();
}

/* used on yield */
void SysTick_Handler2(void)
{
	struct tcb_s *task = kcb->task_current->data;

	// save current PSP, call the scheduler and get new PSP
	task_psp = &task->context[CONTEXT_PSP];

	if (!kcb->tasks->length)
		krnl_panic(ERR_NO_TASKS);
		
	_stack_check();
	krnl_schedule();

	task = kcb->task_current->data;
	new_task_psp = &task->context[CONTEXT_PSP];
	
	/* trigger PendSV interrupt to perform a task schedule and context switch */
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

void _yield(void)
{
	void (*sysfunc)(void *) = (void *)(void *)SysTick_Handler2;

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

	/* setup TIM11 for jiffies */
	tim11_config();
	tim11_start();

	/* configure USART */
	usart_init(USART_PORT, USART_BAUD, 0);
	
	_stdout_install(__putchar);
	_stdin_install(__getchar);
	_stdpoll_install(__kbhit);

#ifdef USB_SERIAL
	_delay_ms(2000);
#endif
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

static void timer_enable(void)
{
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

static void timer_disable(void)
{
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void _timer_enable(void)
{
	void (*sysfunc)(void *) = (void *)(void *)timer_enable;
	
	syscall(sysfunc, 0);
}

void _timer_disable(void)
{
	void (*sysfunc)(void *) = (void *)(void *)timer_disable;
	
	syscall(sysfunc, 0);
}

void _dispatch_init(jmp_buf env)
{
	uint32_t *ctx_p;
	struct tcb_s *task = kcb->task_current->data;

	if ((kcb->preemptive == 'y'))
		timer_enable();
	
	ctx_p = (uint32_t *)env;
	// Set PSP to top of task 0 stack
	__set_PSP((ctx_p[CONTEXT_PSP] + 16*4));
	// Switch to use Process Stack, unprivileged state
	__set_CONTROL(0x3);
	// Execute ISB after changing CONTROL (architectural recommendation)
	__ISB();
	
	/* configure FP state save behaviour */
//	FPU->FPCCR &= ~(FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk);
//	FPU->FPCCR &= ~FPU_FPCCR_LSPEN_Msk;

	task->task();
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
