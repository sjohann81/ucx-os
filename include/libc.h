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

char *strcpy(char *dst, char *src);
char *strncpy(char *s1, char *s2, int32_t n);
char *strcat(char *dst, char *src);
char *strncat(char *s1, char *s2, int32_t n);
int32_t strcmp(char *s1, char *s2);
int32_t strncmp(char *s1, char *s2, int32_t n);
char *strstr(char *string, char *find);
int32_t strlen(char *s);
char *strchr(char *s, int32_t c);
char *strpbrk(char *str, char *set);
char *strsep(char **pp, char *delim);
char *strtok(char *s, char *delim);
void *memcpy(void *dst, void *src, uint32_t n);
void *memmove(void *dst, void *src, uint32_t n);
int32_t memcmp(void *cs, void *ct, uint32_t n);
void *memset(void *s, int32_t c, uint32_t n);
int32_t strtol(char *s, char **end, int32_t base);
int32_t atoi(char *s);
int32_t puts(char *str);
char *gets(char *s);
char *getline(char *s);
int32_t random(void);
int32_t abs(int32_t n);
void putchar(char c);
int32_t puts(char *str);
char *gets(char *s);
int32_t random(void);
void srand(uint32_t seed);
int32_t hexdump(char *buf, uint32_t size);
uint16_t crc16(int8_t *data, uint32_t len);
uint32_t crc32(int8_t *data, uint32_t len);
int32_t printf(char *fmt, ...);
int32_t sprintf(char *out, char *fmt, ...);
