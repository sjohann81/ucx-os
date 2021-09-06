/* file:          hal.h
 * description:   hardware abstraction layer (HAL) definitions for ATMEGA328p
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <stdint.h>
#include <setjmp.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define __ARCH__	"ATMEGA328p"

/* disable interrupts, return previous int status / enable interrupts */
#define _di()				_interrupt_set(0)
#define _ei(S)				_interrupt_set(S)

char _interrupt_set(char s);

void dispatcher(void);

void _putchar(char value);
int32_t _kbhit(void);
int32_t _getchar(void);

void _hardware_init(void);
void _timer_enable(void);
void _timer_disable(void);
void _interrupt_tick(void);
