/* file:          main.c
 * description:   UCX/OS kernel
 * date:          10/2022
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

/* main() function, called from the C runtime */

int32_t main(void)
{
	int32_t pr;
	
	_hardware_init();
	
	printf("UCX/OS boot on %s\n", __ARCH__);
#ifndef UNKNOWN_HEAP
	ucx_heap_init((size_t *)&_heap_start, (size_t)&_heap_size);
	printf("heap_init(), %d bytes free\n", (size_t)&_heap_size);
#else
	ucx_heap_init((size_t *)&__bss_end, ((size_t)&__stack - (size_t)&__bss_end - DEFAULT_STACK_SIZE));
	printf("heap_init(), %d bytes free\n", ((size_t)&__stack - (size_t)&__bss_end - DEFAULT_STACK_SIZE));
#endif
	pr = app_main();

	setjmp(kcb_p->context);
	kcb_p->tcb_p = kcb_p->tcb_first;
	
	if (pr) {
		kcb_p->preemptive = 'y';
		_timer_enable();
	} else {
		kcb_p->preemptive = 'n';
	}

	_dispatch_init(kcb_p->tcb_p->context);
	
	/* never reached */
	return 0;
}
