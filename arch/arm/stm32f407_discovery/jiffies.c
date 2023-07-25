#include <hal.h>
#include <jiffies.h>

int jf_setfreq(uint32_t jf_freq, uint32_t jiffies)
{
	uint32_t psc = 0;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);

	if (jf_freq)
		psc = (SystemCoreClock / jf_freq) - 1;

	if (psc < 0xffff)
		TIM11->PSC = psc;
	else
		return 1;

	if (jiffies < 0xffff)
		TIM11->ARR = jiffies;
	else
		return 1;
		
	TIM11->CR1 |= TIM_CR1_CEN;
	
	return 0;
}

static jf_t _jf;

void jf_link_setfreq(jf_setfreq_pt pfun)
{
	_jf.setfreq = pfun;
}

void jf_link_value(jiffy_t *v)
{
	_jf.value = v;
}

int jf_init(uint32_t jf_freq, uint32_t jiffies)
{
	if (_jf.setfreq) {
		if (_jf.setfreq(jf_freq, jiffies))
			return 1;

		_jf.jiffies = jiffies;
		_jf.freq = jf_freq;
		_jf.jpus = jf_per_usec();
		
		return 0;
	}
	return 1;
}

jiffy_t jf_per_usec(void)
{
	jiffy_t jf = _jf.freq / 1000000;

	if (jf <= _jf.jiffies)
		return jf;
	else
		return 0;
}

jiffy_t jf_value(void)
{
	return *_jf.value;
}

void jf_delay_us(int32_t usec)
{
	jiffy_t m, m2, m1 = *_jf.value;

	usec *= _jf.jpus;
	if (*_jf.value - m1 > usec)
		return;

	while (usec > 0) {
		m2 = *_jf.value;
		m = m2 - m1;
		usec -= (m > 0) ? m : _jf.jiffies + m;
		m1 = m2;
	}
}

int jf_check_usec(int32_t usec)
{
	static jiffy_t m1 = -1, cnt;
	jiffy_t m, m2;

	if (m1 == -1) {
		m1 = *_jf.value;
		cnt = _jf.jpus * usec;
	}

	if (cnt > 0) {
		m2 = *_jf.value;
		m = m2-m1;
		cnt -= (m > 0) ? m : _jf.jiffies + m;
		m1 = m2;
		
		return 1;
	} else {
		m1 = -1;
		
		return 0;
	}
}

void jf_setup(void)
{
	jf_link_setfreq((jf_setfreq_pt)jf_setfreq);
	jf_link_value((jiffy_t*)&JF_TIM_VALUE);
	jf_init(SystemCoreClock, 1000);
}
