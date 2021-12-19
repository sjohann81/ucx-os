#include <hal.h>

#define NULL			((void *)0)
#define USED			1
#define TRUE			1
#define FALSE			0
#define isprint(c)		(' '<=(c)&&(c)<='~')
#define isspace(c)		((c)==' '||(c)=='\t'||(c)=='\n'||(c)=='\r')
#define isdigit(c)		('0'<=(c)&&(c)<='9')
#define islower(c)		('a'<=(c)&&(c)<='z')
#define isupper(c)		('A'<=(c)&&(c)<='Z')
#define isalpha(c)		(islower(c)||isupper(c))
#define isalnum(c)		(isalpha(c)||isdigit(c))
#define min(a,b)		((a)<(b)?(a):(b))

#if BIG_ENDIAN

#define htons(n) (n)
#define ntohs(n) (n)
#define htonl(n) (n)
#define ntohl(n) (n)

#else

#define htons(n) (((((uint16_t)(n) & 0xFF)) << 8) | (((uint16_t)(n) & 0xFF00) >> 8))
#define ntohs(n) (((((uint16_t)(n) & 0xFF)) << 8) | (((uint16_t)(n) & 0xFF00) >> 8))

#define htonl(n) (((((uint32_t)(n) & 0xFF)) << 24) | \
                  ((((uint32_t)(n) & 0xFF00)) << 8) | \
                  ((((uint32_t)(n) & 0xFF0000)) >> 8) | \
                  ((((uint32_t)(n) & 0xFF000000)) >> 24))

#define ntohl(n) (((((uint32_t)(n) & 0xFF)) << 24) | \
                  ((((uint32_t)(n) & 0xFF00)) << 8) | \
                  ((((uint32_t)(n) & 0xFF0000)) >> 8) | \
                  ((((uint32_t)(n) & 0xFF000000)) >> 24))
                  
#define htonll(x) ((1==htonl(1)) ? (x) : (((uint64_t)htonl((x) & 0xFFFFFFFFUL)) << 32) | htonl((uint32_t)((x) >> 32)))
#define ntohll(x) ((1==ntohl(1)) ? (x) : (((uint64_t)ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t)((x) >> 32)))

#endif

char *_strcpy(char *dst, char *src);
char *_strncpy(char *s1, char *s2, int32_t n);
char *_strcat(char *dst, char *src);
char *_strncat(char *s1, char *s2, int32_t n);
int32_t _strcmp(char *s1, char *s2);
int32_t _strncmp(char *s1, char *s2, int32_t n);
char *_strstr(char *string, char *find);
int32_t _strlen(char *s);
char *_strchr(char *s, int32_t c);
char *_strpbrk(char *str, char *set);
char *_strsep(char **pp, char *delim);
char *_strtok(char *s, char *delim);
void *_memcpy(void *dst, void *src, uint32_t n);
void *_memmove(void *dst, void *src, uint32_t n);
int32_t _memcmp(void *cs, void *ct, uint32_t n);
void *_memset(void *s, int32_t c, uint32_t n);
int32_t _strtol(char *s, char **end, int32_t base);
int32_t _atoi(char *s);
int32_t _abs(int32_t n);
int32_t _random(void);
void _srand(uint32_t seed);
int32_t _puts(char *str);
char *_gets(char *s);
char *_getline(char *s);
int32_t _printf(const char *fmt, ...);
int32_t _sprintf(char *out, const char *fmt, ...);


