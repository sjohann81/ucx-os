/* IEEE single-precision definitions */
#define SNG_EXPBITS	8
#define SNG_FRACBITS	23
#define SNG_EXP_BIAS	127
#define SNG_EXP_INFNAN	255
#define EXCESS		126
#define SIGNBIT		0x80000000
#define HIDDEN		(1 << 23)
#define SIGN(fp)	((fp) & SIGNBIT)
#define EXP(fp)		(((fp) >> 23) & 0xFF)
#define MANT(fp)	(((fp) & 0x7FFFFF) | HIDDEN)
#define PACK(s,e,m)	((s) | ((e) << 23) | (m))

union float_long{
	float f;
	int32_t l;
	uint32_t u;
};

float atof(const char *p);
int32_t ftoa(float f, char *outbuf, int32_t precision);
