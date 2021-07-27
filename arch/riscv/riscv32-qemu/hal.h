/* file:          hal.h
 * description:   hardware abstraction layer (HAL) definitions for RISC-V (32 bits)
 * date:          05/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

/* C type extensions */
typedef unsigned char			uint8_t;
typedef char				int8_t;
typedef unsigned short int		uint16_t;
typedef short int			int16_t;
typedef unsigned int			uint32_t;
typedef int				int32_t;
typedef unsigned long long		uint64_t;
typedef long long			int64_t;
typedef unsigned long			size_t;

extern uint32_t _stack_start;		/* Start of the STACK memory. */
extern uint32_t _stack_end;		/* End of the STACK memory. */
extern uint32_t _heap_start;		/* Start of the HEAP memory. */
extern uint32_t _heap_end;		/* End of the HEAP memory (one byte past the last byte of this memory). */
extern uint32_t _heap_size;		/* Size of HEAP memory. */
extern uint32_t _sidata;		/* Start address for the contents initialization of the .data
						section. defined in linker script. */
extern uint32_t _sdata;			/* Start address for the .data section, defined in linker script */
extern uint32_t _edata;			/* End address for the .data section, defined in linker script. */
extern uint32_t _sbss;			/* Start address for the .bss section, defined in linker script. */
extern uint32_t _ebss;			/* End address for the .bss section, defined in linker script. */
extern uint32_t _end;			/* Start address of the heap memory, defined in linker script. */

#define __ARCH__	"RV32 (Qemu)"

/* disable interrupts, return previous int status / enable interrupts */
#define _di()				_interrupt_set(0)
#define _ei(S)				_interrupt_set(S)

#define read_csr(reg) ({ uint32_t __tmp; asm volatile ("csrr %0, " #reg : "=r"(__tmp)); __tmp; })
#define write_csr(reg, val) ({ asm volatile ("csrw " #reg ", %0" :: "rK"(val)); })

#define NS16550A_UART0_CTRL_ADDR(a)	*(uint8_t*) (0x10000000 + (a))
#define NS16550A_RBR			0x00
#define NS16550A_THR      		0x00
#define NS16550A_IER      		0x01
#define NS16550A_DLL      		0x00
#define NS16550A_DLM      		0x01
#define NS16550A_FCR      		0x02
#define NS16550A_LCR      		0x03
#define NS16550A_MCR      		0x04
#define NS16550A_LSR      		0x05
#define NS16550A_MSR      		0x06
#define NS16550A_SCR      		0x07
#define NS16550A_LCR_DLAB 		0x80
#define NS16550A_LCR_8BIT 		0x03
#define NS16550A_LCR_PODD 		0x08
#define NS16550A_LSR_DA   		0x01
#define NS16550A_LSR_OE   		0x02
#define NS16550A_LSR_PE   		0x04
#define NS16550A_LSR_FE   		0x08
#define NS16550A_LSR_BI   		0x10
#define NS16550A_LSR_RE  	 	0x20
#define NS16550A_LSR_RI   		0x40
#define NS16550A_LSR_EF   		0x80

#define MTIME				(*(volatile uint64_t *)(0x0200bff8))
#define MTIMECMP			(*(volatile uint64_t *)(0x02004000))
#define MTIME_L				(*(volatile uint32_t *)(0x0200bff8))
#define MTIME_H				(*(volatile uint32_t *)(0x0200bffc))
#define MTIMECMP_L			(*(volatile uint32_t *)(0x02004000))
#define MTIMECMP_H			(*(volatile uint32_t *)(0x02004004))

/* hardware dependent C library stuff */
typedef uint32_t jmp_buf[20];

int32_t _interrupt_set(int32_t s);
int32_t setjmp(jmp_buf env);
void longjmp(jmp_buf env, int32_t val);

void dispatcher(void);

void _putchar(char value);
int32_t _kbhit(void);
int32_t _getchar(void);

void _delay_ms(uint32_t msec);
void _delay_us(uint32_t usec);

void _hardware_init(void);
void _timer_enable(void);
void _timer_disable(void);
void _interrupt_tick(void);

uint64_t mtime_r(void);
void mtime_w(uint64_t val);
uint64_t mtimecmp_r(void);
void mtimecmp_w(uint64_t val);
