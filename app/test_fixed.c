/*
a = 3/4 and x = 3**-40
x=				0.8225263339969959081282058400607250283965E-19
(a+x) - a=			0.8225263339969958611685571972304851389214E-19
4*(a+x) - 3*x - 3=		0.8225263339969939352386128099946320829435E-19
0.2*(a+x) + 0.8*x - 0.15=	0.8225263339969961019098002456349667709187E-19
x/(a+x)=			0.1096701778662661210717332307615448941248E-18
1 - a/(a+x)=			0.1096701778662661469213066994179955694558E-18
1/(a/x+1)=			0.1096701778662661210717332307615448941248E-18
(1-(a+0.1*x)/(a+x))/0.9=	0.1096701778662661362216958972666852630998E-18
a/(a+x)=			0.9999999999999999998903298221337338530787
1 - x/(a+x)=			0.9999999999999999998903298221337338530787
1/(1+x/a)=			0.9999999999999999998903298221337338530787
(a+x/10)/(a+x) - 0.1*x/(a+x)=	0.9999999999999999998903298221337338530787
log((a+x)/a))=			0.1096701778662660506187957060995936986397E-18
log(a+x)-log(a)=		0.1096701778662661469213066994179955694558E-18
log(1+x/a)=			0.1096701778662660506187957060995936986397E-18
-log(1-x/(a+x))=		0.1096701778662661469273204733746047874730E-18
*/

#include <ucx.h>
#include <fixed.h>

fixed_t epsilon(void)
{
	fixed_t x = fix_val(1.0);

	while ((fix_val(1.0) + fix_div(x, fix_val(2.0))) > fix_val(1.0))
		x = fix_div(x, fix_val(2.0));

	return x;
}

void doit(fixed_t a, fixed_t x)
{
	char buf[30], buf2[30];

	fixtoa(a, buf, 6); fixtoa(x, buf2, 6);
	printf("\n\na = %s, x = %s\n", buf , buf2);
	fixtoa(x, buf, 6);
	printf("\nx                                            = %s", buf);
	fixtoa((a + x) - a, buf, 6);
	printf("\n(a + x) - a                                  = %s", buf);
	fixtoa(fix_mul(fix_val(4.0), (a + x)) - fix_mul(fix_val(3.0), x) - fix_val(3.0), buf, 6);
	printf("\n4.0 * (a + x) - 3 * x - 3                    = %s", buf);
	fixtoa(fix_mul(fix_val(0.2), (a + x)) + fix_mul(fix_val(0.8), x) - fix_val(0.15), buf, 6);
	printf("\n0.2 * (a + x) + 0.8 * x - 0.15               = %s", buf);
	fixtoa(fix_div(x, (a + x)), buf, 6);
	printf("\nx / (a + x)                                  = %s", buf);
	fixtoa(fix_val(1.0) - fix_div(a, (a + x)), buf, 6);
	printf("\n1.0 - a / (a + x)                            = %s", buf);
	fixtoa(fix_div(fix_val(1.0), (fix_div(a, x) + fix_val(1.0))), buf, 6);
	printf("\n1.0 / ( a / x + 1)                           = %s", buf);
	fixtoa(fix_div((fix_val(1.0) - fix_div((a + fix_mul(fix_val(0.1), x)), (a + x))), fix_val(0.9)), buf, 6);
	printf("\n(1.0 - (a + 0.1 * x) / (a + x)) / 0.9        = %s", buf);
	fixtoa(fix_div(a, (a + x)), buf, 6);
	printf("\na / ( a + x)                                 = %s", buf);
	fixtoa(fix_val(1.0) - fix_div(x, (a + x)), buf, 6);
	printf("\n1.0 - x / (a + x)                            = %s", buf);
	fixtoa(fix_div(fix_val(1.0), (fix_val(1.0) + fix_div(x, a))), buf, 6);
	printf("\n1.0 / (1 + x / a)                            = %s", buf);
	fixtoa(fix_div((a + fix_div(x, fix_val(10.0))), (a + x)) - fix_div(fix_mul(fix_val(0.1), x), (a + x)), buf, 6);
	printf("\n(a + x / 10.0) / (a + x) - 0.1 * x / (a + x) = %s", buf);
	fixtoa(fix_ln(fix_div((a + x), a)), buf, 6);
	printf("\nlog((a + x) / a))                            = %s", buf);
	fixtoa(fix_ln(a + x) - fix_ln(a), buf, 6);
	printf("\nlog(a + x) - log(a)                          = %s", buf);
	fixtoa(fix_ln(fix_val(1.0) + fix_div(x, a)), buf, 6);
	printf("\nlog(1.0 + x / a)                             = %s", buf);
	fixtoa(-fix_ln(fix_val(1.0) - fix_div(x, (a + x))), buf, 6);
	printf("\n-log(1.0 - x / (a + x))                      = %s", buf);
	printf("\n");
}

void testfp(void)
{
	int i;
	fixed_t a, x;
	char buf[30];

	fixtoa(epsilon(), buf, 6);
	printf("\nmachine epsilon: %s\n", buf);

	a = fix_div(fix_val(3.0), fix_val(4.0));
	for (i = 3; i > -7; i--) {
		x = fix_pow(fix_val(3.0), fix_val(i));
		doit(a, x);
	}

}

void task0()
{
	testfp();
	
	for (;;);
}

int32_t app_main(void)
{
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);

	return 1;
}
