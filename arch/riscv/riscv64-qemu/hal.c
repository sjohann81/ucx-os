/* file:          hal.c
 * description:   hardware abstraction layer for RISC-V (64 bits)
 * date:          05/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <hal.h>
#include <console.h>
#include <lib/libc.h>
#include <lib/list.h>
#include <kernel/kernel.h>
#include <riscv.h>

/* hardware platform dependent stuff */
static int __putchar(int value)		// polled putchar()
{
	while (!((NS16550A_UART0_CTRL_ADDR(NS16550A_LSR) & NS16550A_LSR_RI)));
	NS16550A_UART0_CTRL_ADDR(NS16550A_THR) = value;
	
	return value;
}

static int __kbhit(void)
{
	return 0;
}

static int __getchar(void)
{
	return 0;
}

int32_t _interrupt_set(int32_t s)
{
	static int int_status = 0;

	(!s) ? int_status-- : int_status++;
	if (int_status < 0)
		asm volatile ("csrc mstatus, 8");
	else
		asm volatile ("csrs mstatus, 8");
	
	return int_status;
}

void _delay_ms(uint32_t msec)
{
	volatile uint32_t cur, last, delta, msecs;
	uint32_t cycles_per_msec;

	last = MTIME;
	delta = msecs = 0;
	cycles_per_msec = F_CPU / 1000;
	while (msec > msecs) {
		cur = MTIME;
		if (cur < last)
			delta += (cur + (F_CPU - last));
		else
			delta += (cur - last);
		last = cur;
		if (delta >= cycles_per_msec) {
			msecs += delta / cycles_per_msec;
			delta %= cycles_per_msec;
		}
	}
}

void _delay_us(uint32_t usec)
{
	volatile uint32_t cur, last, delta, usecs;
	uint32_t cycles_per_usec;

	last = MTIME;
	delta = usecs = 0;
	cycles_per_usec = F_CPU / 1000000;
	while (usec > usecs) {
		cur = MTIME;
		if (cur < last)
			delta += (cur + (F_CPU - last));
		else
			delta += (cur - last);
		last = cur;
		if (delta >= cycles_per_usec) {
			usecs += delta / cycles_per_usec;
			delta %= cycles_per_usec;
		}
	}
}

static void uart_init(uint32_t baud)
{
	uint32_t divisor = F_CPU / (16 * baud);

	NS16550A_UART0_CTRL_ADDR(NS16550A_LCR) = NS16550A_LCR_DLAB;
	NS16550A_UART0_CTRL_ADDR(NS16550A_DLM) = (divisor >> 8) & 0xff;
	NS16550A_UART0_CTRL_ADDR(NS16550A_DLL) = divisor & 0xff;
	NS16550A_UART0_CTRL_ADDR(NS16550A_LCR) = NS16550A_LCR_8BIT;
}

void _panic(void)
{
	volatile int * const exit_device = (int* const)0x100000;
	*exit_device = 0x5555;
	while (1);
}

void _irq_handler(uint64_t cause, uint64_t epc)
{
	uint64_t val;
	
	val = read_csr(mcause);
	if (mtime_r() > mtimecmp_r()) {
		mtimecmp_w(mtime_r() + (F_CPU / F_TIMER));
		krnl_dispatcher();
	} else {
		printf("[%x]\n", val);
		_panic();
	}

}

uint32_t _readcounter(void)
{
	return MTIME_L;
}

uint64_t _read_us(void)
{
	uint64_t timeref;

	timeref = (uint64_t)MTIME_H << 32 | (uint64_t)MTIME_L;

	return (timeref / (F_CPU / 1000000));
}

void _cpu_idle(void)
{
	asm volatile ("wfi");
}

uint16_t _cpu_id(void)
{
	return r_mhartid();
}

uint64_t mtime_r(void)
{
	return MTIME;
}

void mtime_w(uint64_t val)
{
	MTIME = val;
}

uint64_t mtimecmp_r(void)
{
	volatile uint64_t *addr;
	
	addr = &MTIMECMP + _cpu_id();
	
	return *addr;
}

void mtimecmp_w(uint64_t val)
{
	volatile uint64_t *addr;
	
	addr = &MTIMECMP + _cpu_id();
	*addr = val;
}

void _hardware_init(void)
{
	uart_init(USART_BAUD);

	_stdout_install(__putchar);
	_stdin_install(__getchar);
	_stdpoll_install(__kbhit);
}

void _timer_enable(void)
{
	uint64_t mie;
	
	mie = r_mie();
	mie |= 0x80;
	w_mie(mie);
}

void _timer_disable(void)
{
	uint64_t mie;
	
	mie = r_mie();
	mie &= ~0x80;
	w_mie(mie);
}

void _interrupt_tick(void)
{
#ifndef MULTICORE
	struct tcb_s *task = kcb->task_current->data;
#else
	struct tcb_s *task = kcb[_cpu_id()]->task_current->data;
#endif
	_read_us();
	/* task is run for the first time */
	if ((uint64_t)task->task == task->context[CONTEXT_RA])
		asm volatile ("csrs mstatus, 8");
}

extern void __dispatch_init(jmp_buf env);

void _dispatch_init(jmp_buf env)
{
	uint64_t mip;
	
#ifndef MULTICORE
	if (kcb->preemptive == 'y') {
#else
	if (kcb[_cpu_id()]->preemptive == 'y') {
#endif
		/* clear pending timer interrupts */
		mip = r_mip();
		mip &= ~0x80;
		w_mip(mip);
		
		/* set timer to the future */
		mtimecmp_w(mtime_r() + (F_CPU / F_TIMER));
		
		/* enable timer interrupts */
		_timer_enable();
	}

	__dispatch_init(env);
}

void _context_init(jmp_buf *ctx, size_t sp, size_t ss, size_t ra)
{
	uint64_t *ctx_p;
	
	ctx_p = (uint64_t *)ctx;
	
	ctx_p[CONTEXT_SP] = sp + ss;
	ctx_p[CONTEXT_RA] = ra;
}
