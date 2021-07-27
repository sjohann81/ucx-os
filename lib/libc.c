/* file:          libc.c
 * description:   small C library
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

char *_strcpy(char *dst, char *src){
	char *dstSave=dst;
	int32_t c;

	do{
		c = *dst++ = *src++;
	} while(c);
	return dstSave;
}

char *_strncpy(char *s1, char *s2, int32_t n){
	int32_t i;
	char *os1;

	os1 = s1;
	for (i = 0; i < n; i++)
		if ((*s1++ = *s2++) == '\0') {
			while (++i < n)
				*s1++ = '\0';
			return(os1);
		}
	return(os1);
}

char *_strcat(char *dst, char *src){
	int32_t c;
	char *dstSave=dst;

	while(*dst)
		++dst;
	do {
		c = *dst++ = *src++;
	} while(c);

	return dstSave;
}

char *_strncat(char *s1, char *s2, int32_t n){
	char *os1;

	os1 = s1;
	while (*s1++);
	--s1;
	while ((*s1++ = *s2++))
		if (--n < 0) {
			*--s1 = '\0';
			break;
		}
	return(os1);
}

int32_t _strcmp(char *s1, char *s2){
	while (*s1 == *s2++)
		if (*s1++ == '\0')
			return(0);

	return(*s1 - *--s2);
}

int32_t _strncmp(char *s1, char *s2, int32_t n){
	while (--n >= 0 && *s1 == *s2++)
		if (*s1++ == '\0')
			return(0);

	return(n<0 ? 0 : *s1 - *--s2);
}

char *_strstr(char *string, char *find){
	int32_t i;

	for (;;) {
		for(i = 0; string[i] == find[i] && find[i]; ++i);
		if(find[i] == 0)
			return (char *)string;
		if(*string++ == 0)
			return 0;
	}
}

int32_t _strlen(char *s){
	int32_t n;

	n = 0;
	while (*s++)
		n++;

	return(n);
}

char *_strchr(char *s, int32_t c){
	while (*s != (char)c)
		if (!*s++)
			return 0;

	return (char *)s;
}

char *_strpbrk(char *str, char *set){
	char c, *p;

	for (c = *str; c != 0; str++, c = *str) {
		for (p = set; *p != 0; p++) {
			if (c == *p) {
				return str;
			}
		}
	}
	return 0;

}

char *_strsep(char **pp, char *delim){
	char *p, *q;

	if (!(p = *pp))
		return 0;
	if ((q = _strpbrk (p, delim))) {
		*pp = q + 1;
		*q = '\0';
	}else	*pp = 0;

	return p;
}

char *_strtok(char *s, char *delim){
	char *spanp;
	int32_t c, sc;
	char *tok;
	static char *last;

	if (s == NULL && (s = last) == NULL)
		return (NULL);

	cont:
	c = *s++;
	for (spanp = delim; (sc = *spanp++) != 0;){
		if (c == sc)
		goto cont;
	}

	if (c == 0){
		last = NULL;
		return (NULL);
	}
	tok = s - 1;

	for (;;) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				last = s;
				return (tok);
			}
		} while (sc != 0);
	}
}

void *_memcpy(void *dst, void *src, uint32_t n){
	char *r1 = dst;
	char *r2 = src;

	while (n--)
		*r1++ = *r2++;

	return dst;
}

void *_memmove(void *dst, void *src, uint32_t n){
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

int32_t _memcmp(void *cs, void *ct, uint32_t n){
	char *r1 = (char *)cs;
	char *r2 = (char *)ct;

	while (n && (*r1 == *r2)) {
		++r1;
		++r2;
		--n;
	}

	return (n == 0) ? 0 : ((*r1 < *r2) ? -1 : 1);
}

void *_memset(void *s, int32_t c, uint32_t n) {
	char *p = (char *)s;

	while (n--)
		*p++ = (char)c;

	return s;
}

int32_t _strtol(char *s, char **end, int32_t base) {
	int32_t i;
	uint32_t ch, value=0, neg=0;

	if(s[0] == '-'){
		neg = 1;
		++s;
	}
	if (s[0] == '0' && s[1] == 'x'){
		base = 16;
		s += 2;
	}
	for (i = 0; i <= 8; ++i){
		ch = *s++;
		if('0' <= ch && ch <= '9')
			ch -= '0';
		else if('A' <= ch && ch <= 'Z')
			ch = ch - 'A' + 10;
		else if('a' <= ch && ch <= 'z')
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

int32_t _abs(int32_t n){
	return n>=0 ? n:-n;
}


static uint32_t rand1=0xbaadf00d;

int32_t _random(void){
	rand1 = rand1 * 1103515245 + 12345;
	return (uint32_t)(rand1 >> 16) & 32767;
}

void _srand(uint32_t seed){
	rand1 = seed;
}

int32_t _puts(char *str){
	while (*str)
		_putchar(*str++);
	_putchar('\n');

	return 0;
}

char *_gets(char *s){
	int32_t c;
	char *cs;

	cs = s;
	while ((c = _getchar()) != '\n' && c >= 0)
		*cs++ = c;
	if (c < 0 && cs == s)
		return(NULL);
	*cs++ = '\0';
	return(s);
}

char *_getline(char *s){
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
		return(NULL);
	*cs++ = '\0';
	return(s);
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
	if (str){
		**str = c;
		++(*str);
	}else{
		if (c) _putchar(c);
	}
}

static int vsprintf(char **buf, const char *fmt, va_list args)
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
			if (str == NULL)
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

int32_t _printf(char *fmt, ...){
	va_list args;
	int32_t v;

	va_start(args, fmt);
	v = vsprintf(0, fmt, args);
	va_end(args);
	return v;
}

int32_t _sprintf(char *out, char *fmt, ...){
	va_list args;
	int32_t v;

	va_start(args, fmt);
	v = vsprintf(&out, fmt, args);
	va_end(args);
	return v;
}

int32_t hexdump(char *buf, uint32_t size){
	uint32_t k, l;
	char ch;

	buf = (char *)((size_t)buf & 0xfffffff0);
	for (k = 0; k < size; k += 16) {
		_printf("\n%08x ", buf + k);
		for(l = 0; l < 16; l++){
			_printf("%02x ", (uint8_t)buf[k + l]);
			if (l == 7) _putchar(' ');
		}
		_printf(" |");
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
