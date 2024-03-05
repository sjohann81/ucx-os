#include <ucx.h>

char *__env[1] = { 0 };
char **environ = __env;
int errno = 0;

void *_sbrk(int incr)
{
	extern uint32_t _end, _stack;
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0)
		heap_end = (char *)&_end;

	prev_heap_end = heap_end;

	if (heap_end + incr > (char *)_stack) {
		errno = ENOMEM;
		return (void *) -1;
	}

	heap_end += incr;

	return (void *)prev_heap_end;
}

int _getpid(void)
{
	return 1;
}

int _kill(int pid, int sig)
{
	errno = EINVAL;
	return -1;
}

void _exit (int status)
{
	_kill(status, -1);
	while (1);
}

int _write(int file, char *ptr, int len)
{
	int idx;

	for (idx = 0; idx < len; idx++)
		//__io_putchar(*ptr++);
		_putchar(*ptr++);

	return len;
}

int _close(int file)
{
	return -1;
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	return 1;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int _read(int file, char *ptr, int len)
{
	int idx;

	for (idx = 0; idx < len; idx++)
		//*ptr++ = __io_getchar();
		*ptr++ = _getchar();

	return len;
}

int _open(char *path, int flags, ...)
{
	return -1;
}

int _wait(int *status)
{
	errno = ECHILD;
	return -1;
}

int _unlink(char *name)
{
	errno = ENOENT;
	return -1;
}
/*
int _times(struct tms *buf)
{
	return -1;
}
*/
int _stat(char *file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _link(char *old, char *new)
{
	errno = EMLINK;
	return -1;
}

int _fork(void)
{
	errno = EAGAIN;
	return -1;
}

int _execve(char *name, char **argv, char **env)
{
	errno = ENOMEM;
	return -1;
}
