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

char *ucx_strcpy(char *dst, char *src);
char *ucx_strncpy(char *s1, char *s2, int32_t n);
char *ucx_strcat(char *dst, char *src);
char *ucx_strncat(char *s1, char *s2, int32_t n);
int32_t ucx_strcmp(char *s1, char *s2);
int32_t ucx_strncmp(char *s1, char *s2, int32_t n);
char *ucx_strstr(char *string, char *find);
int32_t ucx_strlen(char *s);
char *ucx_strchr(char *s, int32_t c);
char *ucx_strpbrk(char *str, char *set);
char *ucx_strsep(char **pp, char *delim);
char *ucx_strtok(char *s, char *delim);
int32_t ucx_strtol(char *s, char **end, int32_t base);
void *ucx_memcpy(void *dst, void *src, uint32_t n);
void *ucx_memmove(void *dst, void *src, uint32_t n);
int32_t ucx_memcmp(void *cs, void *ct, uint32_t n);
void *ucx_memset(void *s, int32_t c, uint32_t n);
int32_t ucx_abs(int32_t n);
int32_t ucx_random(void);
void ucx_srand(uint32_t seed);
int32_t ucx_puts(char *str);
char *ucx_gets(char *s);
char *ucx_getline(char *s);
int32_t ucx_printf(const char *fmt, ...);
int32_t ucx_sprintf(char *out, const char *fmt, ...);


