float __addsf3(float a1, float a2);
float __subsf3(float a1, float a2);
int32_t __cmpsf2(float a1, float a2);
int32_t __ltsf2(float a, float b);
int32_t __lesf2(float a, float b);
int32_t __gtsf2(float a, float b);
int32_t __gesf2(float a, float b);
int32_t __eqsf2(float a, float b);
int32_t __nesf2(float a, float b);
float __mulsf3(float a1, float a2);
float __divsf3(float a1, float a2);
float __negsf2(float a1);
int32_t __fixsfsi(float a_fp);
uint32_t __fixunssfsi(float a_fp);
float __floatsisf(int32_t af);
float __floatunsisf(uint32_t af);

double __extendsfdf2 (float a1);
float __truncdfsf2 (double a1);

float __aeabi_fadd(float a, float b) { return __addsf3(a, b); }
float __aeabi_fsub(float a, float b) { return __subsf3(a, b); }
float __aeabi_fmul(float a, float b) { return __mulsf3(a, b); }
float __aeabi_fdiv(float a, float b) { return __divsf3(a, b); }

float __aeabi_i2f(int a) { return __floatsisf(a); }
float __aeabi_f2iz(int a) { return __fixsfsi(a); }
double __aeabi_f2d(int a) { return __extendsfdf2(a); }
float __aeabi_d2f(int a) { return __truncdfsf2(a); }

int __aeabi_fcmplt(float a, float b) { return __ltsf2(a, b); }
int __aeabi_fcmple(float a, float b) { return __lesf2(a, b); }
int __aeabi_fcmpgt(float a, float b) { return __gtsf2(a, b); }
int __aeabi_fcmpge(float a, float b) { return __gesf2(a, b); }
int __aeabi_fcmpeq(float a, float b) { return __eqsf2(a, b); }
int __aeabi_fcmpne(float a, float b) { return __nesf2(a, b); }
