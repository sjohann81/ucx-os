#include <ucx.h>


static int (*const syscalls[])() = {
	/* UNIX syscalls */
	[SYS_FORK] = sys_fork,
	[SYS_EXIT] = sys_exit,
	[SYS_WAIT] = sys_wait,
	[SYS_PIPE] = sys_pipe,
	[SYS_KILL] = sys_kill,
	[SYS_EXEC] = sys_execve,
	[SYS_DUP] = sys_dup,
	[SYS_GETPID] = sys_getpid,
	[SYS_SBRK] = sys_sbrk,
	[SYS_USLEEP] = sys_usleep,
	[SYS_STAT] = sys_stat,
	[SYS_OPEN] = sys_open,
	[SYS_CLOSE] = sys_close,
	[SYS_READ] = sys_read,
	[SYS_WRITE] = sys_write,
	[SYS_LSEEK] = sys_lseek,
	[SYS_CHDIR] = sys_chdir,
	[SYS_MKNOD] = sys_mknod,
	[SYS_LINK] = sys_link,
	[SYS_UNLINK] = sys_unlink,
	/* UCX-OS syscalls */
	[SYS_TADD] = sys_tadd,
	[SYS_TREMOVE] = sys_tremove,
	[SYS_TYIELD] = sys_tyield,
	[SYS_TDELAY] = sys_tdelay,
	[SYS_TSUSPEND] = sys_tsuspend,
	[SYS_TRESUME] = sys_tresume,
	[SYS_TPRIORITY] = sys_tpriority,
	[SYS_TID] = sys_tid,
	[SYS_TWFI] = sys_twfi,
	[SYS_TCOUNT] = sys_tcount
};

int syscall(int num, int arg0, int arg1, int arg2)
{
	return syscalls[num](arg0, arg1, arg2);
}


char *_env[1] = { 0 };
char **environ = _env;
int errno = 0;


/* UNIX syscalls */

int sys_fork(void)
{
	errno = EAGAIN;
	return -1;
}

int sys_exit(int status)
{
	sys_kill(status, -1);
	while (1);
	
	return -1;
}

int sys_wait(int *status)
{
	errno = ECHILD;
	return -1;
}

int sys_pipe(int fildes[2])
{
	errno = EFAULT;
	return -1;
}

int sys_kill(int pid, int sig)
{
	errno = EINVAL;
	return -1;
}

int sys_execve(char *name, char **argv, char **env)
{
	errno = ENOMEM;
	return -1;
}

int sys_dup(int oldfd)
{
	errno = EBADF;
	return -1;
}

int sys_getpid(void)
{
	return 1;
}

int sys_sbrk(int incr)
{
	extern uint32_t _end, _stack;
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0)
		heap_end = (char *)&_end;

	prev_heap_end = heap_end;

	if (heap_end + incr > (char *)_stack) {
		errno = ENOMEM;
		return -1;
	}

	heap_end += incr;

	return (int)prev_heap_end;
}

int sys_usleep(int usec)
{
	errno = EINTR;
	return 0;
}

int sys_stat(char *file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int sys_open(char *path, int flags)
{
	return -1;
}

int sys_close(int file)
{
	return -1;
}

int sys_read(int file, char *ptr, int len)
{
	int idx;

	for (idx = 0; idx < len; idx++)
		*ptr++ = _getchar();

	return len;
}

int sys_write(int file, char *ptr, int len)
{
	int idx;

	for (idx = 0; idx < len; idx++)
		_putchar(*ptr++);

	return len;
}

int sys_lseek(int file, int ptr, int dir)
{
	return 0;
}

int sys_chdir(const char *path)
{
	errno = EFAULT;
	return -1;
}

int sys_mknod(const char *path, int mode, int dev)
{
	errno = EFAULT;
	return -1;
}

int sys_unlink(char *name)
{
	errno = ENOENT;
	return -1;
}

int sys_link(char *old, char *new)
{
	errno = EMLINK;
	return -1;
}


/* UCX-OS syscalls */

int sys_tadd(void *task, int stack_size)
{
	return 1;
}

int sys_tremove(int id)
{
	return 1;
}

int sys_tyield(void)
{
	return 1;
}

int sys_tdelay(int ticks)
{
	return 1;
}

int sys_tsuspend(int id)
{
	return 1;
}

int sys_tresume(int id)
{
	return 1;
}

int sys_tpriority(int id, int priority)
{
	return 1;
}

int sys_tid(void)
{
	return 1;
}

int sys_twfi(void)
{
	return 1;
}

int sys_tcount(void)
{
	return 1;
}
