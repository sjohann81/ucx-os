#include <ucx.h>
#include "ieee754.h"

float atof(const char *p)
{
	float val, power;
	int32_t i, sign;

	for (i = 0; isspace(p[i]); i++);

	sign = (p[i] == '-') ? -1 : 1;

	if (p[i] == '+' || p[i] == '-')
		i++;
	for (val = 0.0f; isdigit(p[i]); i++)
		val = 10.0f * val + (p[i] - '0');

	if (p[i] == '.')
		i++;
	for (power = 1.0f; isdigit(p[i]); i++) {
		val = 10.0f * val + (p[i] - '0');
		power *= 10.0f;
	}

	return sign * val / power;
}

int32_t ftoa(float f, char *outbuf, int32_t precision)
{
	int32_t mantissa, int_part, frac_part, exp2, i;
	char *p;
	union float_long fl;

	p = outbuf;

	if (f < 0.0) {
		*p = '-';
		f = -f;
		p++;
	}

	fl.f = f;

	exp2 = (fl.l >> 23) - 127;
	mantissa = (fl.l & 0xffffff) | 0x800000;
	frac_part = 0;
	int_part = 0;

	if (exp2 >= 31){
		return -1;	/* too large */
	} else {
		if (exp2 < -23) {
//			return -1;	/* too small */
		} else {
			if (exp2 >= 23) {
				int_part = mantissa << (exp2 - 23);
			} else {
				if (exp2 >= 0) {
					int_part = mantissa >> (23 - exp2);
					frac_part = (mantissa << (exp2 + 1)) & 0xffffff;
				} else {
					frac_part = (mantissa & 0xffffff) >> (-(exp2 + 1));
				}
			}
		}
	}

	if (int_part == 0) {
		*p = '0';
		p++;
	} else {
		itoa(int_part, p, 10);
		while(*p) p++;
	}
	*p = '.';
	p++;

	for (i = 0; i < precision; i++) {
		frac_part = (frac_part << 3) + (frac_part << 1);
		*p = (frac_part >> 24) + '0';
		p++;
		frac_part = frac_part & 0xffffff;
	}

	*p = 0;

	return 0;
}
