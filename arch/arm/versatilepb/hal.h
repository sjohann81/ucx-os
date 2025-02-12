/* file:          hal.h
 * description:   hardware abstraction layer (HAL) definitions ARM (Versatilepb)
 * date:          11/2022
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

#define __ARCH__	"ARMv6 (Versatilepb)"

/* disable interrupts, return previous int status / enable interrupts */
#define IRQ_FLAG			0x80
#define _di()				_interrupt_set(0)
#define _ei()				_interrupt_set(1)

/* peripherals */

/* http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0183g/I18381.html*/
#define UART0_BASE			0x101f1000
#define UART0DR				(*(volatile uint32_t *)(UART0_BASE+0x000))
#define UART0FR				(*(volatile uint32_t *)(UART0_BASE+0x018))
#define UARTFR_TXFF			0x20
#define UARTFR_RXFF			0x10

#define TIMER0_BASE			0x101e2000
#define TIMER0_LOAD			(*(volatile uint32_t *)(TIMER0_BASE+0x00))
#define TIMER0_VALUE			(*(volatile uint32_t *)(TIMER0_BASE+0x04))
#define TIMER0_CONTROL			(*(volatile uint32_t *)(TIMER0_BASE+0x08))
#define TIMER0_INTCLR			(*(volatile uint32_t *)(TIMER0_BASE+0x0c))
#define TIMER0_RIS			(*(volatile uint32_t *)(TIMER0_BASE+0x10))
#define TIMER0_MIS			(*(volatile uint32_t *)(TIMER0_BASE+0x14))
#define TIMER0_BGLOAD			(*(volatile uint32_t *)(TIMER0_BASE+0x18))

#define TIMER3_BASE			0x101e3020
#define TIMER3_LOAD			(*(volatile uint32_t *)(TIMER3_BASE+0x00))
#define TIMER3_VALUE			(*(volatile uint32_t *)(TIMER3_BASE+0x04))
#define TIMER3_CONTROL			(*(volatile uint32_t *)(TIMER3_BASE+0x08))
#define TIMER3_INTCLR			(*(volatile uint32_t *)(TIMER3_BASE+0x0c))
#define TIMER3_RIS			(*(volatile uint32_t *)(TIMER3_BASE+0x10))
#define TIMER3_MIS			(*(volatile uint32_t *)(TIMER3_BASE+0x14))
#define TIMER3_BGLOAD			(*(volatile uint32_t *)(TIMER3_BASE+0x18))

#define TIMER_EN			0x80
#define TIMER_PERIODIC			0x40
#define TIMER_INTEN			0x20
#define TIMER_32BIT			0x02
//#define TIMER_ONESHOT			0x01

#define TIMCLK				1000000

/* interrupt controller */
#define NVIC_BASE			0x10140000
#define VIC_IRQSTATUS			(*(volatile uint32_t *)(NVIC_BASE+0x000))
#define VIC_FIQSTATUS			(*(volatile uint32_t *)(NVIC_BASE+0x004))
#define VIC_RAWINTR			(*(volatile uint32_t *)(NVIC_BASE+0x008))
#define VIC_INTSELECT			(*(volatile uint32_t *)(NVIC_BASE+0x00c))
#define VIC_IRQENABLE			(*(volatile uint32_t *)(NVIC_BASE+0x010))
#define VIC_IRQENCLEAR			(*(volatile uint32_t *)(NVIC_BASE+0x014))
#define VIC_IRQSOFT			(*(volatile uint32_t *)(NVIC_BASE+0x018))
#define VIC_IRQSOFTCLEAR		(*(volatile uint32_t *)(NVIC_BASE+0x01c))
#define VIC_PROTECT			(*(volatile uint32_t *)(NVIC_BASE+0x020))
#define VIC_VECTADDR			(*(volatile uint32_t *)(NVIC_BASE+0x030))
#define VIC_DEFVECTADDR			(*(volatile uint32_t *)(NVIC_BASE+0x034))

/*
source:
https://github.com/Luminger/Alice-1121-Modem/blob/master/kernel/linux/include/asm-arm/arch-versatile/platform.h
https://github.com/Luminger/Alice-1121-Modem/blob/master/kernel/linux/include/asm-arm/arch-versatile/irqs.h
*/
#define INT_WDOGINT                     0	/* Watchdog timer */
#define INT_SOFTINT                     1	/* Software interrupt */
#define INT_COMMRx                      2	/* Debug Comm Rx interrupt */
#define INT_COMMTx                      3	/* Debug Comm Tx interrupt */
#define INT_TIMERINT0_1                 4	/* Timer 0 and 1 */
#define INT_TIMERINT2_3                 5	/* Timer 2 and 3 */
#define INT_GPIOINT0                    6	/* GPIO 0 */
#define INT_GPIOINT1                    7	/* GPIO 1 */
#define INT_GPIOINT2                    8	/* GPIO 2 */
#define INT_GPIOINT3                    9	/* GPIO 3 */
#define INT_RTCINT                      10	/* Real Time Clock */
#define INT_SSPINT                      11	/* Synchronous Serial Port */
#define INT_UARTINT0                    12	/* UART 0 on development chip */
#define INT_UARTINT1                    13	/* UART 1 on development chip */
#define INT_UARTINT2                    14	/* UART 2 on development chip */
#define INT_SCIINT                      15	/* Smart Card Interface */
#define INT_CLCDINT                     16	/* CLCD controller */
#define INT_DMAINT                      17	/* DMA controller */
#define INT_PWRFAILINT                  18	/* Power failure */
#define INT_MBXINT                      19	/* Graphics processor */
#define INT_GNDINT                      20	/* Reserved */

#define INTMASK_WDOGINT                 (1 << INT_WDOGINT)
#define INTMASK_SOFTINT                 (1 << INT_SOFTINT)
#define INTMASK_COMMRx                  (1 << INT_COMMRx)
#define INTMASK_COMMTx                  (1 << INT_COMMTx)
#define INTMASK_TIMERINT0_1             (1 << INT_TIMERINT0_1)
#define INTMASK_TIMERINT2_3             (1 << INT_TIMERINT2_3)
#define INTMASK_GPIOINT0                (1 << INT_GPIOINT0)
#define INTMASK_GPIOINT1                (1 << INT_GPIOINT1)
#define INTMASK_GPIOINT2                (1 << INT_GPIOINT2)
#define INTMASK_GPIOINT3                (1 << INT_GPIOINT3)
#define INTMASK_RTCINT                  (1 << INT_RTCINT)
#define INTMASK_SSPINT                  (1 << INT_SSPINT)
#define INTMASK_UARTINT0                (1 << INT_UARTINT0)
#define INTMASK_UARTINT1                (1 << INT_UARTINT1)
#define INTMASK_UARTINT2                (1 << INT_UARTINT2)
#define INTMASK_SCIINT                  (1 << INT_SCIINT)
#define INTMASK_CLCDINT                 (1 << INT_CLCDINT)
#define INTMASK_DMAINT                  (1 << INT_DMAINT)
#define INTMASK_PWRFAILINT              (1 << INT_PWRFAILINT)
#define INTMASK_MBXINT                  (1 << INT_MBXINT)
#define INTMASK_GNDINT                  (1 << INT_GNDINT)

/* hardware dependent C library stuff */
#define CONTEXT_SP	8
#define CONTEXT_RA	9

typedef uint32_t jmp_buf[20];

int32_t _interrupt_set(int32_t s);
int32_t setjmp(jmp_buf env);
void longjmp(jmp_buf env, int32_t val);
void _dispatch_init(jmp_buf env);

void _delay_ms(uint32_t msec);
void _delay_us(uint32_t usec);
uint64_t _read_us(void);

void _hardware_init(void);
void _timer_enable(void);
void _timer_disable(void);
void _interrupt_tick(void);
void _context_init(jmp_buf *ctx, size_t sp, size_t ss, size_t ra);

#define strcpy(dst, src)		ucx_strcpy(dst, src)
#define strncpy(s1, s2, n)		ucx_strncpy(s1, s2, n)
#define strcat(dst, src)		ucx_strcat(dst, src)
#define strncat(dst, src, n)		ucx_strncat(dst, src, n)
#define strcmp(s1, s2)			ucx_strcmp(s1, s2)
#define strncmp(s1, s2, n)		ucx_strncmp(s1, s2, n)
#define strstr(string, find)		ucx_strstr(string, find)
#define strlen(s)			ucx_strlen(s)
#define strchr(s, c)			ucx_strchr(s, c)
#define strpbrk(str, set)		ucx_strpbrk(str, set)
#define strsep(pp, delim)		ucx_strsep(pp, delim)
#define strtok(s, delim)		ucx_strtok(s, delim)
#define strtok_r(s, delim, holder)	ucx_strtok_r(s, delim, holder)
#define strtol(s, end, base)		ucx_strtol(s, end, base)
#define atoi(s)				ucx_atoi(s)
#define itoa(i, s, base)		ucx_itoa(i, s, base)
#define memcpy(dst, src, n)		ucx_memcpy(dst, src, n)
#define memmove(dst, src, n)		ucx_memmove(dst, src, n)
#define memcmp(cs, ct, n)		ucx_memcmp(cs, ct, n)
#define memset(s, c, n)			ucx_memset(s, c, n)
#define abs(n)				ucx_abs(n)
#define random()			ucx_random()
#define srand(seed)			ucx_srand(seed)
#define puts(str)			ucx_puts(str)
#define gets(s)				ucx_gets(s)
#define fgets(s, n, f)			ucx_fgets(s, n, f)
#define getline(s)			ucx_getline(s)
#define printf(fmt, ...)		ucx_printf(fmt, ##__VA_ARGS__)
#define sprintf(out, fmt, ...)		ucx_sprintf(out, fmt, ##__VA_ARGS__)

#define malloc(n)			ucx_malloc(n)
#define free(n)				ucx_free(n)
#define calloc(n, t)			ucx_calloc(n, t) 
#define realloc(p, s)			ucx_realloc(p, s)

void krnl_dispatcher(void);

#define DEFAULT_STACK_SIZE	4096
