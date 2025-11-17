/* file:          main.c
 * description:   UCX/OS kernel
 * date:          10/2022
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

static volatile int boot = 0;

/* main() function, called from the C runtime */

int main(void)
{
	struct tcb_s *task;
	int32_t pr;
	
	_hardware_init();
	
	printf("UCX/OS v%s\n", __VER__);
#ifndef UNKNOWN_HEAP
	ucx_heap_init((size_t *)&_heap_start, (size_t)&_heap_size);
	printf("heap_init() 0x%p - 0x%p, %d bytes free\n",
	(size_t *)&_heap_start, (size_t)&_heap_end, (size_t)&_heap_size);
#else
	ucx_heap_init((size_t *)&__bss_end, ((size_t)&__stack - (size_t)&__bss_end - DEFAULT_STACK_SIZE));
	printf("heap_init() 0x%p - 0x%p, %d bytes free\n",
	(size_t *)&__bss_end, ((size_t)&__stack - DEFAULT_STACK_SIZE),
	((size_t)&__stack - (size_t)&__bss_end - DEFAULT_STACK_SIZE));
#endif
	kcb->tasks = list_create();
	
	if (!kcb->tasks)
		krnl_panic(ERR_KCB_ALLOC);

	pr = app_main();
	setjmp(kcb->context);
	
	if (!kcb->tasks->length)
		krnl_panic(ERR_NO_TASKS);

	kcb->preemptive = pr ? 'y' : 'n';
	kcb->task_current = kcb->tasks->head->next;
	task = kcb->task_current->data;
	
	printf("core %d ready.\n", _cpu_id());
	boot = 1;
	
	_dispatch_init(task->context);
	
	/* never reached */
	return 0;
}


/* main1() function, called from the C runtime for other cores on SMPs */

int app_main0(void) __attribute__ ((weak, alias ("dummy_app")));
int app_main1(void) __attribute__ ((weak, alias ("dummy_app")));
int app_main2(void) __attribute__ ((weak, alias ("dummy_app")));
int app_main3(void) __attribute__ ((weak, alias ("dummy_app")));

static int (*const app_vector[])(void) = {
	app_main0,
	app_main1,
	app_main2,
	app_main3,
};

int dummy_app(void)
{
	while (1) {
		_cpu_idle();
	}
	
	// not reached
	return 0;
}

int main1(void)
{
	while (!boot);
	
	printf("core %d ready.\n", _cpu_id());
	
	if (_cpu_id() < sizeof(app_vector) / sizeof(int *))
		app_vector[_cpu_id()]();
	else
		dummy_app();
	
	return 0;
}
