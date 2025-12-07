#include <ucx.h>


/*
 * - iterate the list and for each element:
 * 	* if TIMER_DISABLED, continue
 * 	* countdown--
 * 	* check if countdown == 0
 * 		- if so, call timer callback
 * 		- if TIMER_AUTORELOAD, reset countdown
 * 		- if TIMER_ONESHOT, set TIMER_DISABLED
 * - yield
 * two implementations: based on systick and based on system uptime
 */

static struct node_s *timer_update_systick(struct node_s *node, void *arg)
{
	struct timer_s *timer = node->data;
	uint32_t tick_diff = (size_t)arg;

	if (timer->mode == TIMER_DISABLED)
		return 0;
	
	while (timer->countdown && tick_diff) {
		timer->countdown--;
		tick_diff--;
	}
	
	if (!timer->countdown) {
		timer->timer_cb(arg);
		if (timer->mode == TIMER_AUTORELOAD)
			timer->countdown = timer->time;
		else
			timer->mode = TIMER_DISABLED;
	}
	
	return 0;
}

static struct node_s *timer_update(struct node_s *node, void *arg)
{
	struct timer_s *timer = node->data;
	uint64_t time = (size_t)arg;

	if (timer->mode == TIMER_DISABLED)
		return 0;
	
	if (time > timer->timecmp) {
		timer->timer_cb(arg);
		if (timer->mode == TIMER_AUTORELOAD)
			timer->timecmp += timer->time;
		else
			timer->mode = TIMER_DISABLED;
	}
	
	return 0;
}


/* 
 * timer task	-> manages the timer dlist and callbacks. calls timer_handler()
 * two implementations: based on systick and based on system uptime
 */

void timer_handler_systick()
{
	uint32_t tick_diff;
	
#ifndef MULTICORE
	static uint32_t last_tick;
	
	if (!last_tick) {
		last_tick = kcb->ticks;
		
		return;
	}
	
	if (kcb->ticks != last_tick) {
		tick_diff = kcb->ticks - last_tick;
		last_tick = kcb->ticks;
		list_foreach(kcb->timer_lst, timer_update_systick, (void *)(size_t)tick_diff);
	}
#else
	static uint32_t last_tick[MAX_CORES];

	if (!last_tick[_cpu_id()]) {
		last_tick[_cpu_id()] = kcb[_cpu_id()]->ticks;
		
		return;
	}
	
	if (kcb[_cpu_id()]->ticks != last_tick[_cpu_id()]) {
		tick_diff = kcb[_cpu_id()]->ticks - last_tick[_cpu_id()];
		last_tick[_cpu_id()] = kcb[_cpu_id()]->ticks;
		list_foreach(kcb[_cpu_id()]->timer_lst, timer_update_systick, (void *)(size_t)tick_diff);
	}
#endif
	
	ucx_task_yield();
}

void timer_handler()
{
	uint64_t time;
	
	time = ucx_uptime();
#ifndef MULTICORE
	list_foreach(kcb->timer_lst, timer_update, (void *)(size_t)time);
#else
	list_foreach(kcb[_cpu_id()]->timer_lst, timer_update, (void *)(size_t)time);
#endif
	
	ucx_task_yield();
}


/*
 * creates a new timer
 * - if kcb->timer_lst is empty, create a list
 * - create a struct timer_s, fill it and add to the list
 * - return timer_id on success (ERR_OK) or error (ERR_FAIL)
 */
int32_t ucx_timer_create(void *(*timer_cb)(void *arg), uint32_t time)
{
	struct node_s *node;
	struct timer_s *timer;
	static uint16_t timer_id = 0x6000;
	
#ifndef MULTICORE
	if (!kcb->timer_lst)
		kcb->timer_lst = list_create();

	if (!kcb->timer_lst)
		return ERR_FAIL;
		
	timer = malloc(sizeof(struct timer_s));
	if (!timer)
		return ERR_FAIL;
		
	node = list_pushback(kcb->timer_lst, timer);
#else
	if (!kcb[_cpu_id()]->timer_lst)
		kcb[_cpu_id()]->timer_lst = list_create();

	if (!kcb[_cpu_id()]->timer_lst)
		return ERR_FAIL;
		
	timer = malloc(sizeof(struct timer_s));
	if (!timer)
		return ERR_FAIL;
		
	node = list_pushback(kcb[_cpu_id()]->timer_lst, timer);
#endif
	if (!node) {
		free(timer);
		return ERR_FAIL;
	}
		
	timer->timer_id = timer_id++;
	timer->timer_cb = timer_cb;
	timer->time = time;
	timer->countdown = time;
	timer->timecmp = 0;
	timer->mode = TIMER_DISABLED;
	
	return timer->timer_id;
}


static struct node_s *timer_id_cmp(struct node_s *node, void *arg)
{
	struct timer_s *timer = node->data;
	uint16_t timer_id = (size_t)arg;
	
	if (timer->timer_id == timer_id)
		return node;
	else
		return 0;
}

/*
 * removes a timer from the list
 * - iterate the list to find a struct timer_s with the timer_id
 * - remove struct timer_s from the list
 * - return ERR_OK on success or error
 */

int32_t ucx_timer_destroy(uint16_t timer_id)
{
	struct node_s *node;
	struct timer_s *timer;

#ifndef MULTICORE
	if (!kcb->timer_lst)
		return ERR_FAIL;
		
	node = list_foreach(kcb->timer_lst, timer_id_cmp, (void *)(size_t)timer_id);
	if (!node)
		return ERR_FAIL;
	
	timer = node->data;
	free(timer);
	list_remove(kcb->timer_lst, node);
#else
	if (!kcb[_cpu_id()]->timer_lst)
		return ERR_FAIL;
		
	node = list_foreach(kcb[_cpu_id()]->timer_lst, timer_id_cmp, (void *)(size_t)timer_id);
	if (!node)
		return ERR_FAIL;
	
	timer = node->data;
	free(timer);
	list_remove(kcb[_cpu_id()]->timer_lst, node);
#endif
	
	return ERR_OK;
}

/*
 * starts a timer countdown
 * - sets timer compare and set timer mode
 * - return ERR_OK or error
 */
int32_t ucx_timer_start(uint16_t timer_id, uint8_t mode)
{
	struct node_s *node;
	struct timer_s *timer;

#ifndef MULTICORE
	if (!kcb->timer_lst)
		return ERR_FAIL;
		
	node = list_foreach(kcb->timer_lst, timer_id_cmp, (void *)(size_t)timer_id);
#else
	if (!kcb[_cpu_id()]->timer_lst)
		return ERR_FAIL;
		
	node = list_foreach(kcb[_cpu_id()]->timer_lst, timer_id_cmp, (void *)(size_t)timer_id);
#endif
	if (!node)
		return ERR_FAIL;
	
	timer = node->data;
	timer->countdown = timer->time;
	timer->timecmp = ucx_uptime() + timer->time;
	timer->mode = mode;
	
	return ERR_OK;
}

/*
 * stops a timer countdown
 * - sets TIMER_DISABLED
 * - return ERR_OK or error
 */
int32_t ucx_timer_cancel(uint16_t timer_id)
{
	struct node_s *node;
	struct timer_s *timer;

#ifndef MULTICORE
	if (!kcb->timer_lst)
		return ERR_FAIL;
		
	node = list_foreach(kcb->timer_lst, timer_id_cmp, (void *)(size_t)timer_id);
#else
	if (!kcb[_cpu_id()]->timer_lst)
		return ERR_FAIL;
		
	node = list_foreach(kcb[_cpu_id()]->timer_lst, timer_id_cmp, (void *)(size_t)timer_id);
#endif
	if (!node)
		return ERR_FAIL;
	
	timer = node->data;
	timer->mode = TIMER_DISABLED;
	
	return ERR_OK;
}
