/* file:          hal.h
 * description:   hardware abstraction layer (HAL) definitions ARM (stm32)
 * date:          05/2023
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <stm32f4xx.h>
#include <system_stm32f4xx.h>
#include <stm32f4xx_conf.h>
#include <stddef.h>

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

extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */

#define ID_UNIQUE_ADDRESS		0x1FFF7A10
#define ID_UNIQUE_8(x)			((x >= 0 && x < 12) ? (*(uint8_t *) (ID_UNIQUE_ADDRESS + (x))) : 0)

#define __ARCH__	"ARM Cortex M4 (stm32f4xx)"

/* hardware dependent C library stuff */
#define CONTEXT_RA	0
#define CONTEXT_SP	9
#define CONTEXT_PSP	10

typedef uint32_t jmp_buf[20];

void _enable_interrupts(void);
void _ei(void);
void _di(void);
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

void syscall(void (*func)(void *), void *args) __attribute__((optimize("1")));

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

#define DEFAULT_STACK_SIZE	2048
