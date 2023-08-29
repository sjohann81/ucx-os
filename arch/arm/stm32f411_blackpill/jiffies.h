#define  JF_TIM_VALUE         (TIM11->CNT)

typedef int32_t jiffy_t;
typedef int (*jf_setfreq_pt)(uint32_t, uint32_t);

typedef volatile struct {
	jf_setfreq_pt setfreq;
	jiffy_t *value;
	uint32_t freq;
	uint32_t jiffies;
	jiffy_t jpus;
}jf_t;

int jf_setfreq(uint32_t jf_freq, uint32_t jiffies);
void jf_link_setfreq(jf_setfreq_pt pfun);
void jf_link_value(jiffy_t *v);
void jf_deinit(void);
int jf_init(uint32_t jf_freq, uint32_t jiffies);
jiffy_t jf_per_usec(void);
jiffy_t jf_value(void);
void jf_delay_us(int32_t usec);
int jf_check_usec(int32_t usec);
void jf_setup(void);
