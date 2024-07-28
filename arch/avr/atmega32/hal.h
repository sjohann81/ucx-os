/* file:          hal.h
 * description:   hardware abstraction layer (HAL) definitions for ATMEGA32
 * date:          12/2022
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <stdint.h>
#include <setjmp.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stddef.h>

extern uint32_t __bss_start;
extern uint32_t __bss_end;
extern uint32_t __stack;

#define HAS_SIZE_T

#define __ARCH__	"ATMEGA32"

/* disable interrupts, return previous int status / enable interrupts */
#define _di()				_interrupt_set(0)
#define _ei()				_interrupt_set(1)
#define _enable_interrupts()		_interrupt_set(1)

#define CONTEXT_SP	18
#define CONTEXT_SR	20
#define CONTEXT_RA	21

char _interrupt_set(char s);

void delay_us(uint16_t);
uint64_t _read_us(void);

void _hardware_init(void);
void _timer_enable(void);
void _timer_disable(void);
void _interrupt_tick(void);
void _dispatch_init(jmp_buf env);
void _context_init(jmp_buf *ctx, size_t sp, size_t ss, size_t ra);

#define _delay_us(us)			delay_us(us)
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

#define DEFAULT_STACK_SIZE	192
