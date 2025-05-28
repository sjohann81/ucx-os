/* file:          main.c
 * description:   UCX/OS kernel
 * date:          10/2022
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

uint32_t __bss_end; //TODO: Make it compile without this definition

/* main() function, called from the C runtime */

int32_t main(void)
{
	struct tcb_s *task;
	int32_t pr;
	
	// _hardware_init(); Je pars du principe que c'est fait avant TODO: RAS
	// pok_arch_init();

	printf("UCX/OS v%s boot on %s\n", __VER__, __ARCH__);
#ifndef UNKNOWN_HEAP
	ucx_heap_init((size_t *)&_heap_start, (size_t)&_heap_size); //TODO: RAS
	printf("heap_init(), %d bytes free\n", (size_t)&_heap_size);
#else
	ucx_heap_init((size_t *)&__bss_end, ((size_t)&__stack - (size_t)&__bss_end - 256)); //TODO: RAS
	printf("heap_init(), %d bytes free\n", ((size_t)&__stack - (size_t)&__bss_end - 256));
#endif
	kcb->tasks = list_create(); //TODO: RAS
	
	if (!kcb->tasks)
		krnl_panic(ERR_KCB_ALLOC); //Appel  vers ucx.c, TODO: RAS

	pr = app_main(); // Appel defini dans les fonctions de kernel/app, donc TODO: RAS
	setjmp(kcb->context); // fonction en asm dans  arch, en gros on stock l'endroit ou en est du contexte. Le compilateur peut se debrouiller d'apres la documentation ARM, TODO: RAS 
	
	if (!kcb->tasks->length)
		krnl_panic(ERR_NO_TASKS);  //Appel  vers ucx.c, TODO: RAS

	kcb->preemptive = pr ? 'y' : 'n';
	kcb->task_current = kcb->tasks->head->next;
	task = kcb->task_current->data;

	// _dispatch_init(task->context); // Defini dans hal.c directement, TODO: CHECK
	ja_context_jump(task->context);

	/* never reached */
	return 0;
}
