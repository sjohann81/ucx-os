enum {TIMER_DISABLED, TIMER_ONESHOT, TIMER_AUTORELOAD};

struct timer_s {
	uint16_t timer_id;
	void *(*timer_cb)(void *arg);
	uint32_t time;
	uint32_t countdown;
	uint64_t timecmp;
	uint8_t mode;
};

void timer_handler();
void timer_handler_systick();
int32_t ucx_timer_create(void *(*timer_cb)(void *arg), uint32_t time);
int32_t ucx_timer_destroy(uint16_t timer_id);
int32_t ucx_timer_start(uint16_t timer_id, uint8_t mode);
int32_t ucx_timer_cancel(uint16_t timer_id);

#if F_TIMER == 0
#define MS_TO_TICKS(ms) (((unsigned long)(ms) * (unsigned long)(F_TIMER_FIXED)) / 1000)
#else
#define MS_TO_TICKS(ms) (((unsigned long)(ms) * (unsigned long)(F_TIMER)) / 1000)
#endif
