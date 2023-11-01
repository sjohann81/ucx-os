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

/* the following deal with IEEE double-precision numbers */
#define EXCESSD		1022
#define HIDDEND		(1 << 20)
#define EXPD(fp)	(((fp.l.upper) >> 20) & 0x7FF)
#define SIGND(fp)	((fp.l.upper) & SIGNBIT)
#define MANTD(fp)	(((((fp.l.upper) & 0xFFFFF) | HIDDEND) << 10) | (fp.l.lower >> 22))
#define HIDDEND_LL	((long long)1 << 52)
#define MANTD_LL(fp)	((fp.ll & (HIDDEND_LL-1)) | HIDDEND_LL)
#define PACKD_LL(s,e,m)	(((long long)((s)+((e)<<20))<<32)|(m))

union double_long {
	double d;
	struct {
#ifdef LITTLE_ENDIAN
		uint32_t lower;
		int32_t upper;
#else
		int32_t upper;
		uint32_t lower;
#endif
	} l;
	uint64_t ll;
};

float atof(const char *p);
int32_t ftoa(float f, char *outbuf, int32_t precision);
