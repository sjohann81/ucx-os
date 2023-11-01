/* file:          libc.c
 * description:   small C library
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

char *ucx_strcpy(char *s1, const char *s2)
{
	char *os1 = s1;

	while ((*s1++ = *s2++));
	
	return os1;
}

char *ucx_strncpy(char *s1, const char *s2, int32_t n)
{
	int32_t i;
	char *os1 = s1;

	for (i = 0; i < n; i++) {
		if ((*s1++ = *s2++) == '\0') {
			while (++i < n)
				*s1++ = '\0';
			return os1;
		}
	}
	
	return os1;
}

char *ucx_strcat(char *s1, const char *s2)
{
	char *os1 = s1;

	while (*s1++);
	--s1;
	while ((*s1++ = *s2++));
	
	return os1;
}

char *ucx_strncat(char *s1, const char *s2, int32_t n)
{
	char *os1 = s1;

	while (*s1++);
	--s1;
	while ((*s1++ = *s2++)) {
		if (--n < 0) {
			*--s1 = '\0';
			break;
		}
	}
		
	return os1;
}

int32_t ucx_strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2++)
		if (*s1++ == '\0')
			return 0;

	return (*s1 - *--s2);
}

int32_t ucx_strncmp(const char *s1, const char *s2, int32_t n)
{
	while (--n >= 0 && *s1 == *s2++)
		if (*s1++ == '\0')
			return 0;

	return (n < 0 ? 0 : *s1 - *--s2);
}

char *ucx_strstr(const char *s1, const char *s2)
{
	int32_t i;

	while (1) {
		for (i = 0; s1[i] == s2[i] && s2[i]; ++i);
		if (s2[i] == 0)
			return (char *)s1;
			
		if (*s1++ == 0)
			return 0;
	}
}

int32_t ucx_strlen(const char *s1)
{
	int32_t n;

	n = 0;
	while (*s1++)
		n++;

	return n;
}

char *ucx_strchr(const char *s1, int32_t c)
{
	while (*s1 != (char)c)
		if (!*s1++)
			return 0;

	return (char *)s1;
}

char *ucx_strpbrk(const char *s1, const char *s2)
{
	char c;
	const char *p;

	for (c = *s1; c != 0; s1++, c = *s1) {
		for (p = s2; *p != 0; p++) {
			if (c == *p) {
				return (char *)s1;
			}
		}
	}
	return 0;

}

char *ucx_strsep(char **pp, const char *delim)
{
	char *p, *q;

	if (!(p = *pp))
		return 0;

	if ((q = ucx_strpbrk(p, delim))) {
		*pp = q + 1;
		*q = '\0';
	} else
		*pp = 0;

	return p;
}

char *ucx_strtok(char *s, const char *delim)
{
	const char *spanp;
	int32_t c, sc;
	char *tok;
	static char *last;

	if (s == 0 && (s = last) == 0)
		return 0;

	cont:
	c = *s++;
	for (spanp = delim; (sc = *spanp++) != 0;) {
		if (c == sc)
		goto cont;
	}

	if (c == 0) {
		last = 0;
		return 0;
	}
	tok = s - 1;

	while (1) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = 0;
				else
					s[-1] = 0;
				last = s;
				return (tok);
			}
		} while (sc != 0);
	}
}

char *ucx_strtok_r(char *s, const char *delim, char **holder)
{
	if (s)
		*holder = s;

	do {
		s = strsep(holder, delim);
	} while (s && !*s);

	return s;
}

int32_t ucx_strtol(const char *s, char **end, int32_t base)
{
	int32_t i;
	uint32_t ch, value=0, neg=0;

	if (s[0] == '-') {
		neg = 1;
		++s;
	}
	if (s[0] == '0' && s[1] == 'x') {
		base = 16;
		s += 2;
	}
	for (i = 0; i <= 8; ++i) {
		ch = *s++;
		if ('0' <= ch && ch <= '9')
			ch -= '0';
		else if ('A' <= ch && ch <= 'Z')
			ch = ch - 'A' + 10;
		else if ('a' <= ch && ch <= 'z')
			ch = ch - 'a' + 10;
		else
			break;
		value = value * base + ch;
	}
	if (end)
		*end = (char*)s - 1;
	if (neg)
		value = -(int32_t)value;
		
	return value;
}

int32_t ucx_atoi(const char *s)
{
	int32_t n, f;

	n = 0;
	f = 0;
	
	for (;;s++) {
		switch (*s) {
		case ' ':
		case '\t':
			continue;
		case '-':
			f++;
		case '+':
			s++;
		}
		break;
	}
	while (*s >= '0' && *s <= '9')
		n = n * 10 + *s++ - '0';

	return (f ? -n : n);
}

void ucx_itoa(int32_t i, char *s, int32_t base)
{
	char c;
	char *p = s;
	char *q = s;
	uint32_t h;

	if (base == 16) {
		h = (uint32_t)i;
		do {
			*q++ = '0' + (h % base);
		} while (h /= base);
		if ((i >= 0) && (i < 16)) *q++ = '0';
		for (*q = 0; p <= --q; p++){
			(*p > '9') ? (c = *p + 39) : (c = *p);
			(*q > '9') ? (*p = *q + 39) : (*p = *q);
			*q = c;
		}
	} else {
		if (i >= 0) {
			do {
				*q++ = '0' + (i % base);
			} while (i /= base);
		} else {
			*q++ = '-';
			p++;
			do {
				*q++ = '0' - (i % base);
			} while (i /= base);
		}
		for (*q = 0; p <= --q; p++) {
			c = *p;
			*p = *q;
			*q = c;
		}
	}
}

void *ucx_memcpy(void *dst, const void *src, uint32_t n)
{
	char *r1 = dst;
	const char *r2 = src;

	while (n--)
		*r1++ = *r2++;

	return dst;
}

void *ucx_memmove(void *dst, const void *src, uint32_t n)
{
	char *s = (char *)dst;
	char *p = (char *)src;

	if (p >= s) {
		while (n--)
			*s++ = *p++;
	} else {
		s += n;
		p += n;
		while (n--)
			*--s = *--p;
	}

	return dst;
}

int32_t ucx_memcmp(const void *cs, const void *ct, uint32_t n)
{
	char *r1 = (char *)cs;
	char *r2 = (char *)ct;

	while (n && (*r1 == *r2)) {
		++r1;
		++r2;
		--n;
	}

	return (n == 0) ? 0 : ((*r1 < *r2) ? -1 : 1);
}

void *ucx_memset(void *s, int32_t c, uint32_t n)
{
	char *p = (char *)s;

	while (n--)
		*p++ = (char)c;

	return s;
}

int32_t ucx_abs(int32_t n)
{
	return n >= 0 ? n : -n;
}


static uint32_t rand1=0xbaadf00d;

int32_t ucx_random(void)
{
	rand1 = rand1 * 1103515245 + 12345;
	return (uint32_t)(rand1 >> 16) & 32767;
}

void ucx_srand(uint32_t seed)
{
	rand1 = seed;
}

int32_t ucx_puts(const char *str)
{
	while (*str)
		_putchar(*str++);
	_putchar('\n');

	return 0;
}

char *ucx_gets(char *s)
{
	int32_t c;
	char *cs;

	cs = s;
	while ((c = _getchar()) != '\n' && c >= 0)
		*cs++ = c;
	if (c < 0 && cs == s)
		return 0;
	*cs++ = '\0';
	
	return s;
}

char *ucx_fgets(char *s, int n, void *f)
{
	int ch;
	char *p = s;

	while (n > 1) {
		ch = _getchar();
		*p++ = ch;
		n--;
		if (ch == '\n')
			break;
	}
	if (n)
		*p = '\0';

	return s;
}

char *ucx_getline(char *s)
{
	int32_t c, i = 0;
	char *cs;

	cs = s;
	while ((c = _getchar()) != '\n' && c >= 0) {
		if (++i == 80) {
			*cs = '\0';
			break;
		}
		*cs++ = c;
	}
	if (c < 0 && cs == s)
		return 0;
		
	*cs++ = '\0';
	
	return (s);
}

/* printf() / sprintf() stuff */

static uint32_t divide(long *n, int base)
{
	uint32_t res;

	res = ((uint32_t)*n) % base;
	*n = (long)(((uint32_t)*n) / base);
	return res;
}

static int toint(const char **s)
{
	int i = 0;
	while (isdigit((int)**s))
		i = i * 10 + *((*s)++) - '0';
		
	return i;
}

static void printchar(char **str, int32_t c){
	if (str) {
		**str = c;
		++(*str);
	} else {
		if (c) _putchar(c);
	}
}

static int ucx_vsprintf(char **buf, const char *fmt, va_list args)
{
	char **p, *str;
	const char *digits = "0123456789abcdef";
	char pad, tmp[16];
	int width, base, sign, i;
	long num;

	for (p = buf; *fmt; fmt++) {
		if (*fmt != '%') {
			printchar(p, *fmt);
			continue;
		}
		/* get flags */
		++fmt;
		pad = ' ';
		if (*fmt == '0') {
			pad = '0';
			fmt++;
		}
		/* get width */
		width = -1;
		if (isdigit(*fmt)) {
			width = toint(&fmt);
		}
		base = 10;
		sign = 0;
		switch (*fmt) {
		case 'c':
			printchar(p, (char)va_arg(args, int));
			continue;
		case 's':
			str = va_arg(args, char *);
			if (str == 0)
				str = "<NULL>";
			for (; *str && width != 0; str++, width--) {
				printchar(p, *str);
			}
			while (width-- > 0)
				printchar(p, pad);
			continue;
		case 'l':
			fmt++;
			num = va_arg(args, long);
			break;
		case 'X':
		case 'x':
			base = 16;
			num = va_arg(args, long);
			break;
		case 'd':
			sign = 1;
		case 'u':
			num = va_arg(args, int);
			break;
		case 'p':
			base = 16;
			num = va_arg(args, size_t);
			width = sizeof(size_t);
			break;
		default:
			continue;
		}
		if (sign && num < 0) {
			num = -num;
			printchar(p, '-');
			width--;
		}
		i = 0;
		if (num == 0)
			tmp[i++] = '0';
		else
			while (num != 0)
				tmp[i++] = digits[divide(&num, base)];
		width -= i;
		while (width-- > 0)
			printchar(p, pad);
		while (i-- > 0)
			printchar(p, tmp[i]);
	}
	printchar(p, '\0');

	return 0;
}

int32_t ucx_printf(const char *fmt, ...)
{
	va_list args;
	int32_t v;

	va_start(args, fmt);
	v = ucx_vsprintf(0, fmt, args);
	va_end(args);
	return v;
}

int32_t ucx_sprintf(char *out, const char *fmt, ...)
{
	va_list args;
	int32_t v;

	va_start(args, fmt);
	v = ucx_vsprintf(&out, fmt, args);
	va_end(args);
	return v;
}
