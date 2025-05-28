#include <hal.h>
#include <kernel/kernel.h>
#include <kernel/ecodes.h>
#include </home/victorgilbert/Portage_UCX-OS_TMS570/repo/ucx-os-forked/drivers/bus/include/i2c.h>
#include </home/victorgilbert/Portage_UCX-OS_TMS570/repo/ucx-os-forked/drivers/bus/include/pwm.h>
#include <HL_reg_gio.h>
#include <thread.h>
#include <gpio.h>
#include <gpio_ll.h>
#include <libpok_legacy/syscall.h>

// pok_ret_t lja_do_syscall (pok_syscall_id_t syscall_id, pok_syscall_args_t* args);

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
    return ; // Regarder avec Salma qui gere la memoire ce qui prend en compte normalement de checker la stack ptete donc, a verifier plus tard
}
// void _dispatch(void); // TODO: Regarder si implementation necessaire ou si implementation de base dans ucx.c est assez

uint64_t _read_us(void)
{
	return ja_system_time() / 1000000; //Cette fonction retourne en ns et on retourne ne ms
}

void _delay_us(uint32_t usec){
	uint32_t start_time = _read_us();
	while (_read_us()-start_time < usec){}
}

void _delay_ms(uint32_t msec){
	_delay_us(msec*1000);
}

volatile uint32_t *task_psp, *new_task_psp;
/* used on yield */
void SysTick_Handler2(void)
{
	//TODO: NOT GOOD
	// struct tcb_s *task = &kcb->task_current->data;

	// save current PSP, call the scheduler and get new PSP
	// task_psp = kcb->task_current->data->context[CONTEXT_PSP];

	// if (!kcb->tasks->length) krnl_panic(ERR_NO_TASKS);
	// _stack_check(); //
	// krnl_schedule(); // Pas trouve l'implementation

	// kcb->task_current->data = kcb->task_current->data;
	// new_task_psp = &kcb->task_current->data->context[CONTEXT_PSP];
	
	// /* trigger PendSV interrupt to perform a task schedule and context switch */
	// SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}


void _yield(void)
{
	void (*sysfunc)(void *) = (void *)(void *)SysTick_Handler2;

	// lja_do_syscall(sysfunc, 0); //TODO: DONE, on utilise le syscall de  syscall.asm
}

// struct jmp_buf;
// extern "C" void ja_kernel_thread();
void _context_init(jmp_buf *ctx, size_t sp, size_t ss, size_t ra)
{
	/*
		But: Retourner un contexte par reference avec 
		les bonnes valeurs pour le SP,PSP et RA?

		Implementation ci-dessous, fortement inspire de armv7/thread.c
	*/
    struct jet_context* new_ctx = (struct jet_context*)(sp - sizeof(*ctx));
    memset (new_ctx, 0, sizeof (struct jet_context));

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

	new_ctx->regs[1] = ra;
	// new_ctx->lr = (uint32_t)ja_kernel_thread; TODO: FIX
	new_ctx->sp = (uint32_t)ctx;
}

#define NUMBER_OF_PINS 16
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
									gioPORTA->PULDIS |= (uint32)((uint32)1 << i);
									return 0;
								case GPIO_PULLUP: 
									gioPORTA->PSL |= (uint32)((uint32)1 << i);
									return 0;
								case GPIO_PULLDOWN: 
									gioPORTA->PSL &= ~(uint32)((uint32)1 << i);
									return 0;
								default: return -1;
								}
		
						case GPIO_PORTB:
							gioPORTB->DIR = (uint32)((uint32)1 << i);
							switch (pullsel) {
								case GPIO_NOPULL:
									gioPORTA->PULDIS |= (uint32)((uint32)1 << i);
									return 0;
								case GPIO_PULLUP: 
									gioPORTA->PSL |= (uint32)((uint32)1 << i);
									return 0;
								case GPIO_PULLDOWN: 
									gioPORTA->PSL &= ~(uint32)((uint32)1 << i);
									return 0;
								default: return -1;
								}
							}
				case GPIO_OUTPUT:
					gioPORTA->PULDIS = (uint32)((uint32)0 << i);
					switch (cfg->port) {
						case GPIO_PORTA:  gioPORTA->DIR &= ~(uint32)((uint32)1 << i);
							return 0;
						case GPIO_PORTB:  gioPORTB->DIR &= ~(uint32)((uint32)1 << i);
							return 0;
						default: return -1;
					}
				case GPIO_OUTPUT_OD:
					gioPORTA->PULDIS = (uint32)((uint32)1 << i);
					switch (cfg->port) {
						case GPIO_PORTA:  gioPORTA->DIR &= ~(uint32)((uint32)1 << i);
							return 0;
						case GPIO_PORTB:  gioPORTB->DIR &= ~(uint32)((uint32)1 << i);
							return 0;
						default: return -1;
					}

			}
		}
	}
}


int gpio_ll_get(struct gpio_config_values_s *cfg){
	
	switch (cfg->port) {
		case GPIO_PORTA:  return gioGetBit(gioPORTA, cfg->pinsel); break;
		case GPIO_PORTB:  return gioGetBit(gioPORTB, cfg->pinsel); break;
		default: return -1;
	}
}

int gpio_ll_set(struct gpio_config_values_s *cfg, int val){
	switch (cfg->port) {
		case GPIO_PORTA:  return gioSetBit(gioPORTA, cfg->pinsel, val); break;
		case GPIO_PORTB:  return gioSetBit(gioPORTB, cfg->pinsel, val); break;
		default: return -1;
	}
}

int gpio_ll_clear(struct gpio_config_values_s *cfg, int val){
	// switch (cfg->port) {
	// 	case GPIO_PORTA:  gioSetBit(gioPORTA, cfg->pinsel, val); break;
	// 	case GPIO_PORTB:  gioSetBit(gioPORTA, cfg->pinsel, val); break;
	// }
	return gpio_ll_set(&cfg, val);
}

int gpio_ll_toggle(struct gpio_config_values_s *cfg, int val){
	switch (cfg->port) {
		case GPIO_PORTA:  return gioToggleBit(gioPORTA, cfg->pinsel); break;
		case GPIO_PORTB:  return gioToggleBit(gioPORTB, cfg->pinsel); break;
		default: return -1;
	}
}

static void (*int_cb[MAX_INT_SOURCES])(void) = { 0 };

int gpio_ll_int_attach(struct gpio_config_values_s *cfg, int pin, void (*callback)(), int trigger){
	//Trigger: GPIO_RISING, GPIO_FALLING -- je comprends pas trop

	switch (cfg->port) {
		case GPIO_PORTA:  
			int_cb[pin] = callback;
			return gioEnableNotification(gioPORTA, pin); 
			break;
		case GPIO_PORTB:  		
			int_cb[pin + 8U] = callback;
			return gioEnableNotification(gioPORTB, pin); 
			break;
		default: return -1;
	}
}

int i2c_ll_init(struct i2c_hw_config_values_s *config_values){
	// i2cBASE_t I2C_InitStruct;
	// gpio_config_values_s GPIO_InitStruct;
	
	// switch (config_values->port) {
	// case I2C_PORT1:
	return -1;
}

int i2c_ll_deinit(struct i2c_hw_config_values_s *config_values){return -1;}
int i2c_ll_start(struct i2c_hw_config_values_s *config_values){return -1;}
int i2c_ll_restart(struct i2c_hw_config_values_s *config_values){return -1;}
int i2c_ll_stop(struct i2c_hw_config_values_s *config_values){return -1;}
int i2c_ll_wr_addr(struct i2c_hw_config_values_s *config_values, unsigned short addr){return -1;}
int i2c_ll_rd_addr(struct i2c_hw_config_values_s *config_values, unsigned short addr){return -1;}
int i2c_ll_write(struct i2c_hw_config_values_s *config_values, unsigned char val){return -1;}
int i2c_ll_read(struct i2c_hw_config_values_s *config_values, uint8_t nack){return -1;}

long pwm_ll_setup(struct pwm_config_values_s *cfg){return (long)0;}
int pwm_ll_get(struct pwm_config_values_s *cfg, unsigned channel, unsigned *pulse){return -1;}
int pwm_ll_set(struct pwm_config_values_s *cfg, unsigned channel, unsigned pulse){return -1;}
