#include <ucx.h>

static int _stdout_default(int c)
{
	(void)(c);

	return 0;
}

static int _stdin_default(void)
{
	return 0;
}

static int _stdpoll_default(void)
{
	return 0;
}

static int (*_stdout_hook)(int) = _stdout_default;
static int (*_stdin_hook)(void) = _stdin_default;
static int (*_stdpoll_hook)(void) = _stdpoll_default;

void _stdout_install(int (*hook)(int))
{
	_stdout_hook = hook;
}

void _stdin_install(int (*hook)(void))
{
	_stdin_hook = hook;
}

void _stdpoll_install(int (*hook)(void))
{
	_stdpoll_hook = hook;
}

int _putchar(int c)
{
	(*_stdout_hook)(c);
	
	return 0;
}

int _getchar(void)
{
	return (*_stdin_hook)();
}

int _kbhit(void)
{
	return (*_stdpoll_hook)();
}

