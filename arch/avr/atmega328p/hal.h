/* file:          hal.h
 * description:   hardware abstraction layer (HAL) definitions for HF-RISC
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <stdint.h>
#include <setjmp.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* disable interrupts, return previous int status / enable interrupts */
#define _di()				_interrupt_set(0)
#define _ei(S)				_interrupt_set(S)

/* hardware dependent C library stuff */
//typedef uint8_t jmp_buf[32];

char _interrupt_set(char s);
//int32_t setjmp(jmp_buf env);
//void longjmp(jmp_buf env, int32_t val);
//extern int32_t _syscall(int32_t service, int32_t arg0, int32_t arg1, int32_t arg2);

void dispatcher(void);

void _putchar(char value);
int32_t _kbhit(void);
int32_t _getchar(void);

void _hardware_init(void);
void _timer_enable(void);
void _timer_disable(void);
void _interrupt_tick(void);
