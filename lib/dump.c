/* file:          dump.c
 * description:   memory dump functions
 * date:          12/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

void ucx_printhex(int n, int digits)
{
	int a;

	while (digits > 0) {
		digits--;
		a = (n >> (digits << 2)) & 0xf;
		if (a > 9)
			a = a + '7';
		else
			a = a + '0';
		_putchar(a);
	}
}

int32_t ucx_hexdump(char *buf, uint32_t size)
{
	uint32_t k, l;
	char ch;

	buf = (char *)((size_t)buf & ~0xf);
	
	for (k = 0; k < size; k += 16) {
		_putchar('\n');
		ucx_printhex((size_t)buf + k, 8);
		_putchar(' ');
		
		for (l = 0; l < 16; l++) {
			ucx_printhex((uint8_t)buf[k + l], 2);
			_putchar(' ');
			if (l == 7)
				_putchar(' ');
		}
		
		_putchar(' '); _putchar('|');
		
		for (l = 0; l < 16; l++) {
			ch = (uint8_t)buf[k + l];
			if ((ch >= 32) && (ch <= 126))
				_putchar(ch);
			else
				_putchar('.');
		}
		
		_putchar('|');
	}

	return 0;
}
