enum {TIMER_DISABLED, TIMER_ONESHOT, TIMER_AUTORELOAD};

struct timer_s {
	uint16_t timer_id;
	void *(*timer_cb)(void *arg);
	uint32_t time;
	uint32_t countdown;
	uint8_t mode;
};

void timer_handler();
int32_t ucx_timer_create(void *(*timer_cb)(void *arg), uint32_t time);
int32_t ucx_timer_destroy(uint16_t timer_id);
int32_t ucx_timer_start(uint16_t timer_id, uint8_t mode);
int32_t ucx_timer_cancel(uint16_t timer_id);
