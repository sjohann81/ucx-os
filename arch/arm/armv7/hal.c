static void _stack_check(void)
{
	// struct tcb_s *task = kcb->task_current->data;
	// uint32_t check = 0x33333333;
	// uint32_t *stack_p = (uint32_t *)task->stack;

	// if (*stack_p != check) { //Si le stack pointer n'est pas a l'adresse 0x33333333 alors panic. 
	// 	hexdump((void *)task->stack, task->stack_sz);
	// 	printf("\n*** task %d, stack: 0x%p (size %d)\n", task->id,
	// 		task->stack, task->stack_sz);
	// 	krnl_panic(ERR_STACK_CHECK);
	// }
    return void; // Regarder avec Salma qui gere la memoire ce qui prend en compte normalement de checker la stack ptete donc, a verifier plus tard
}
// void _dispatch(void); // TODO: Regarder si implementation necessaire ou si implementation de base dans ucx.c est assez

uint64_t _read_us(void)
{
	jet_system_time() / 1000000; //Cette fonction retourne en ns et on retourne ne ms
}

void _delay_us(uint32_t usec){
	uint32_t start_time = _read_us();
	while (_read_us()-start_time < usec){}
}

void _delay_ms(uint32_t msec){
	_delay_us(msec*1000)
}

/* used on yield */
void SysTick_Handler2(void)
{
	struct tcb_s *task = kcb->task_current->data;

	// save current PSP, call the scheduler and get new PSP
	task_psp = &task->context[CONTEXT_PSP];

	if (!kcb->tasks->length)
		krnl_panic(ERR_NO_TASKS);
		
	_stack_check(); //
	krnl_schedule(); // Pas trouve l'implementation

	task = kcb->task_current->data;
	new_task_psp = &task->context[CONTEXT_PSP];
	
	/* trigger PendSV interrupt to perform a task schedule and context switch */
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

void _yield(void)
{
	void (*sysfunc)(void *) = (void *)(void *)SysTick_Handler2;

	lja_do_syscall(sysfunc, 0); //TODO: DONE, on utilise le syscall de  syscall.asm
}

void _context_init(jmp_buf *ctx, size_t sp, size_t ss, size_t ra)
{
	/*
		But: Retourner un contexte par reference avec 
		les bonnes valeurs pour le SP,PSP et RA?

		Implementation ci-dessous, fortement inspire de armv7/thread.c
	*/
    struct jet_context* ctx = (struct jet_context*)(sp - sizeof(*ctx));
    memset (ctx, 0, sizeof (struct jet_context));

    /* Startup flags are:
     *
     * N = 0
     * Z = 0
     * C = 0
     * V = 0
     * Q = 0
     * IT = 00
     * J = 0
     * RES = 000
     * GE = 000
     * IT = 000000
     * E = 1
     * A = 1
     * I = 0
     * F = 0
     * T = 0
     * M = 10000
     * */

    ctx->regs[1] = ra;
    ctx->lr = (uint32_t)ja_kernel_thread;
    ctx->sp = (uint32_t)ctx;
}

#define NUMBER_OF_PINS 16;
#define gioPORTA
int gpio_ll_setup(struct gpio_config_values_s *cfg)
{
	uint32_t modesel, pullsel;
	for (int i = 0; i < NUMBER_OF_PINS; i++) {
		if (cfg->pinsel & (1 << i)) {
			modesel = (cfg->mode & (3 << (i << 1))) >> (i << 1);
			pullsel = (cfg->pull & (3 << (i << 1))) >> (i << 1);

			switch (modesel) {
				case GPIO_INPUT:
					switch (cfg->port) {
						case GPIO_PORTA:
							gioPORTA->DIR = (uint32)((uint32)1 << i);
							switch (pullsel) {
								case GPIO_NOPULL:
									gioPORTA->PULDIS = (uint32)((uint32)1 << i);break;
								case GPIO_PULLUP: 
									gioPORTA->PSL = (uint32)((uint32)1 << i);break;
								case GPIO_PULLDOWN: 
									gioPORTA->PSL = (uint32)((uint32)0 << i);break;
								default: return -1;
								}
		
						case GPIO_PORTB:
							gioPORTB->DIR = (uint32)((uint32)1 << i);
							switch (pullsel) {
								case GPIO_NOPULL:
									gioPORTA->PULDIS = (uint32)((uint32)1 << i);break;
								case GPIO_PULLUP: 
									gioPORTA->PSL = (uint32)((uint32)1 << i);break;
								case GPIO_PULLDOWN: 
									gioPORTA->PSL = (uint32)((uint32)0 << i);break;
								default: return -1;
								}
							}
				case GPIO_OUTPUT:
					gioPORTA->PULDIS = (uint32)((uint32)0 << i);
					switch (cfg->port) {
						case GPIO_PORTA:  gioPORTA->DIR = (uint32)((uint32)0 << i);break;
						case GPIO_PORTB:  gioPORTB->DIR = (uint32)((uint32)0 << i);break;
						default: return -1;
					}
				case GPIO_OUTPUT_OD:
					gioPORTA->PULDIS = (uint32)((uint32)1 << i);
					switch (cfg->port) {
						case GPIO_PORTA:  gioPORTA->DIR = (uint32)((uint32)0 << i);break;
						case GPIO_PORTB:  gioPORTB->DIR = (uint32)((uint32)0 << i);break;
						default: return -1;
					}

			}
		}
	}
}


int gpio_ll_get(struct gpio_config_values_s *cfg){
	
	switch (cfg->port) {
		case GPIO_PORTA:  gioGetBit(gioPORTA, cfg->pinsel); break;
		case GPIO_PORTB:  gioGetBit(gioPORTB, cfg->pinsel); break;
		default: return -1;
	}
}
