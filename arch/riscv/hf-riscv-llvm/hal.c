/* file:          hal.c
 * description:   hardware abstraction layer for HF-RISC
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <hal.h>
#include <lib/libc.h>

/*
libc basic I/O support
*/

#ifndef DEBUG_PORT
void _putchar(char value){		// polled putchar()
	while (UARTCAUSE & MASK_UART0_WRITEBUSY);
	UART0 = value;
}

int32_t _kbhit(void){
	return UARTCAUSE & MASK_UART0_DATAAVAIL;
}

int32_t _getchar(void){			// polled getch()
	while (!_kbhit());
	return UART0;
}
#else
void _putchar(char value){		// polled putchar()
	DEBUG_ADDR = value;
}

int32_t _kbhit(void){
	return 0;
}

int32_t _getchar(void){			// polled getch()
	return DEBUG_ADDR;
}
#endif


/*
software implementation of multiply/divide and 64-bit routines
*/

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

int64_t __ashldi3(int64_t u, uint32_t b){
	dwords uu, w;
	uint32_t bm;

	if (b == 0)
		return u;

	uu.all = u;
	bm = 32 - b;

	if (bm <= 0){
		w.s.low = 0;
		w.s.high = (uint32_t) uu.s.low << -bm;
	}else{
		const uint32_t carries = (uint32_t) uu.s.low >> bm;

		w.s.low = (uint32_t) uu.s.low << b;
		w.s.high = ((uint32_t) uu.s.high << b) | carries;
	}

	return w.all;
}

int64_t __ashrdi3(int64_t u, uint32_t b){
	dwords uu, w;
	uint32_t bm;

	if (b == 0)
		return u;

	uu.all = u;
	bm = 32 - b;

	if (bm <= 0){
		/* w.s.high = 1..1 or 0..0 */
		w.s.high = uu.s.high >> 31;
		w.s.low = uu.s.low >> -bm;
	}else{
		const uint32_t carries = (uint32_t) uu.s.high << bm;

		w.s.high = uu.s.high >> b;
		w.s.low = ((uint32_t) uu.s.low >> b) | carries;
	}

	return w.all;
}

int64_t __lshrdi3(int64_t u, uint32_t b){
	dwords uu, w;
	uint32_t bm;

	if (b == 0)
		return u;

	uu.all = u;
	bm = 32 - b;

	if (bm <= 0){
		w.s.high = 0;
		w.s.low = (uint32_t) uu.s.high >> -bm;
	}else{
		const uint32_t carries = (uint32_t) uu.s.high << bm;

		w.s.high = (uint32_t) uu.s.high >> b;
		w.s.low = ((uint32_t) uu.s.low >> b) | carries;
	}

	return w.all;
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


/* delay routines */

void _delay_ms(uint32_t msec)
{
	volatile uint32_t cur, last, delta, msecs;
	uint32_t cycles_per_msec;

	last = TIMER0;
	delta = msecs = 0;
	cycles_per_msec = CPU_SPEED / 1000;
	while (msec > msecs) {
		cur = TIMER0;
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

	last = TIMER0;
	delta = usecs = 0;
	cycles_per_usec = CPU_SPEED / 1000000;
	while (usec > usecs) {
		cur = TIMER0;
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


/* kernel auxiliary routines */

void timer1ctc_handler(void)
{
	krnl_dispatcher();
}

void _hardware_init(void)
{
	_di();
	TIMER1PRE = TIMERPRE_DIV16;

	/* unlock TIMER1 for reset */
	TIMER1 = TIMERSET;
	TIMER1 = 0;

	/* TIMER1 frequency: (39063 * 16) = 250000 cycles (10ms timer @ 25MHz) */
	TIMER1CTC = 15625;

	_ei(1);
}

void _timer_enable(void)
{
	TIMERMASK |= MASK_TIMER1CTC;		/* enable interrupt mask for TIMER1 CTC events */
}

void _timer_disable(void)
{
	TIMERMASK &= ~MASK_TIMER1CTC;		/* disable interrupt mask for TIMER1 CTC events */
}

void _interrupt_tick(void)
{
	_ei(1);
}
