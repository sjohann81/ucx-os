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
	
	kcb_p->tcb_p = 0;
	kcb_p->tcb_first = 0;
	kcb_p->ctx_switches = 0;
	kcb_p->id = 0;
	
	printf("UCX/OS boot on %s\n", __ARCH__);
#ifndef UCX_OS_HEAP_SIZE
	ucx_heap_init((size_t *)&_heap_start, (size_t)&_heap_size);
	printf("heap_init(), %d bytes free\n", (size_t)&_heap_size);
#else
	ucx_heap_init((size_t *)&_heap, UCX_OS_HEAP_SIZE);
	printf("heap_init(), %d bytes free\n", UCX_OS_HEAP_SIZE);
#endif
	pr = app_main();
	
	setjmp(kcb_p->context);
	
	krnl_sched_init(pr);
	
	/* never reached */
	return 0;
}
