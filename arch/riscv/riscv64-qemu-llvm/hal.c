/* file:          hal.c
 * description:   hardware abstraction layer for RISC-V (64 bits)
 * date:          05/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <hal.h>
#include <lib/libc.h>
typedef union{
	int64_t all;
	struct{
#if LITTLE_ENDIAN
		uint32_t low;
		int32_t high;
#else
		int32_t high;
		uint32_t low;
#endif
	} s;
} dwords;

int32_t __mulsi3(uint32_t a, uint32_t b){
	uint32_t answer = 0;

	while(b){
		if(b & 1)
			answer += a;
		a <<= 1;
		b >>= 1;
	}
	return answer;
}

int64_t __muldsi3(uint32_t a, uint32_t b){
	dwords r;

	const int32_t bits_in_word_2 = (int32_t)(sizeof(int32_t) * 8) / 2;
	const uint32_t lower_mask = (uint32_t)~0 >> bits_in_word_2;
	r.s.low = (a & lower_mask) * (b & lower_mask);
	uint32_t t = r.s.low >> bits_in_word_2;
	r.s.low &= lower_mask;
	t += (a >> bits_in_word_2) * (b & lower_mask);
	r.s.low += (t & lower_mask) << bits_in_word_2;
	r.s.high = t >> bits_in_word_2;
	t = r.s.low >> bits_in_word_2;
	r.s.low &= lower_mask;
	t += (b >> bits_in_word_2) * (a & lower_mask);
	r.s.low += (t & lower_mask) << bits_in_word_2;
	r.s.high += t >> bits_in_word_2;
	r.s.high += (a >> bits_in_word_2) * (b >> bits_in_word_2);

	return r.all;
}

int64_t __muldi3(int64_t a, int64_t b){
	dwords x;
	x.all = a;
	dwords y;
	y.all = b;
	dwords r;
	r.all = __muldsi3(x.s.low, y.s.low);
	r.s.high += __mulsi3(x.s.high, y.s.low) + __mulsi3(x.s.low, y.s.high);

	return r.all;
}

uint32_t __udivmodsi4(uint32_t num, uint32_t den, int32_t modwanted){
	uint32_t bit = 1;
	uint32_t res = 0;

	while (den < num && bit && !(den & (1L << 31))) {
		den <<= 1;
		bit <<= 1;
	}
	while (bit){
		if (num >= den){
			num -= den;
			res |= bit;
		}
		bit >>= 1;
		den >>= 1;
	}
	if (modwanted)
		return num;
	return res;
}

int32_t __divsi3(int32_t a, int32_t b){
	int32_t neg = 0;
	int32_t res;

	if (a < 0){
		a = -a;
		neg = !neg;
	}

	if (b < 0){
		b = -b;
		neg = !neg;
	}

	res = __udivmodsi4(a, b, 0);

	if (neg)
		res = -res;

	return res;
}

int32_t __modsi3(int32_t a, int32_t b){
	int32_t neg = 0;
	int32_t res;

	if (a < 0){
		a = -a;
		neg = 1;
	}

	if (b < 0)
		b = -b;

	res = __udivmodsi4(a, b, 1);

	if (neg)
		res = -res;

	return res;
}

uint32_t __udivsi3 (uint32_t a, uint32_t b){
	return __udivmodsi4(a, b, 0);
}

uint32_t __umodsi3 (uint32_t a, uint32_t b){
	return __udivmodsi4(a, b, 1);
}

uint64_t __udivmoddi4(uint64_t num, uint64_t den, uint64_t *rem_p){
	uint64_t quot = 0, qbit = 1;

	if (den == 0){
//		return 1 / ((uint32_t)den);
		return 1;
	}

	while ((int64_t)den >= 0){
		den <<= 1;
		qbit <<= 1;
	}

	while (qbit){
		if (den <= num){
			num -= den;
			quot += qbit;
		}
		den >>= 1;
		qbit >>= 1;
	}

	if (rem_p)
		*rem_p = num;

	return quot;
}

uint64_t __umoddi3(uint64_t num, uint64_t den){
	uint64_t v = 0;

	(void) __udivmoddi4(num, den, &v);
	return v;
}

uint64_t __udivdi3(uint64_t num, uint64_t den){
	return __udivmoddi4(num, den, NULL);
}

int64_t __moddi3(int64_t num, int64_t den){
	int minus = 0;
	int64_t v = 0;

	if (num < 0){
		num = -num;
		minus = 1;
	}
	if (den < 0){
		den = -den;
		minus ^= 1;
	}

	(void) __udivmoddi4(num, den, (uint64_t *)&v);
	if (minus)
		v = -v;

	return v;
}

int64_t __divdi3(int64_t num, int64_t den){
	int minus = 0;
	int64_t v;

	if (num < 0){
		num = -num;
		minus = 1;
	}
	if (den < 0){
		den = -den;
		minus ^= 1;
	}

	v = __udivmoddi4(num, den, NULL);
	if (minus)
		v = -v;

	return v;
}

/* hardware platform dependent stuff */
void _putchar(char value)		// polled putchar()
{
	while (!((NS16550A_UART0_CTRL_ADDR(NS16550A_LSR) & NS16550A_LSR_RI)));
	NS16550A_UART0_CTRL_ADDR(NS16550A_THR) = value;
}

int32_t _kbhit(void)
{
	return 0;
}

int32_t _getchar(void)
{
	return 0;
}

int32_t _interrupt_set(int32_t s)
{
	volatile int32_t val;
	
	val = read_csr(mstatus) & 0x8;
	if (s) {
		asm volatile ("csrs mstatus, 8");
	} else {
		asm volatile ("csrc mstatus, 8");
	}

	return val;
}

void _delay_ms(uint32_t msec)
{
	volatile uint32_t cur, last, delta, msecs;
	uint32_t cycles_per_msec;

	last = MTIME;
	delta = msecs = 0;
	cycles_per_msec = CPU_SPEED / 1000;
	while (msec > msecs) {
		cur = MTIME;
		if (cur < last)
			delta += (cur + (CPU_SPEED - last));
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
	cycles_per_usec = CPU_SPEED / 1000000;
	while (usec > usecs) {
		cur = MTIME;
		if (cur < last)
			delta += (cur + (CPU_SPEED - last));
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
	uint32_t divisor = CPU_SPEED / (16 * baud);

	NS16550A_UART0_CTRL_ADDR(NS16550A_LCR) = NS16550A_LCR_DLAB;
	NS16550A_UART0_CTRL_ADDR(NS16550A_DLM) = (divisor >> 8) & 0xff;
	NS16550A_UART0_CTRL_ADDR(NS16550A_DLL) = divisor & 0xff;
	NS16550A_UART0_CTRL_ADDR(NS16550A_LCR) = NS16550A_LCR_8BIT;
}

void _cpu_idle(void)
{
	asm volatile ("wfi");
}

void _irq_handler(uint32_t cause, uint32_t *stack)
{
	uint32_t val;
	
	val = read_csr(mcause);
	if (mtime_r() > mtimecmp_r()) {
		mtimecmp_w(mtime_r() + 0x1ffff);
		krnl_dispatcher();
	} else {
		printf("[%x]\n", val);
		for (;;);
	}

}

uint32_t _readcounter(void)
{
	return MTIME_L;
}

uint64_t _read_us(void)
{
	static uint64_t timeref = 0;
	static uint32_t tval2 = 0, tref = 0;

	if (tref == 0) _readcounter();
	if (_readcounter() < tref) tval2++;
	tref = _readcounter();
	timeref = ((uint64_t)tval2 << 32) + (uint64_t)_readcounter();

	return (timeref / (CPU_SPEED / 1000000));
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
	return MTIMECMP;
}

void mtimecmp_w(uint64_t val)
{
	MTIMECMP = val;
}

void _panic(void)
{
	volatile int * const exit_device = (int* const)0x100000;
	*exit_device = 0x5555;
	while (1);
}

void _hardware_init(void)
{
	uart_init(TERM_BAUD);
	mtimecmp_w(mtime_r() + 0x1ffff);
}

void _timer_enable(void)
{
	asm volatile ("csrs mstatus, 8");
}

void _timer_disable(void)
{
	asm volatile ("csrc mstatus, 8");
}

void _interrupt_tick(void)
{
	_ei();
}

void _context_init(jmp_buf *ctx, size_t sp, size_t ss, size_t ra)
{
	uint64_t *ctx_p;
	
	ctx_p = (uint64_t *)ctx;
	
	ctx_p[CONTEXT_SP] = sp + ss;
	ctx_p[CONTEXT_RA] = ra;
}
