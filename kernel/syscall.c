#include <ucx.h>


/* syscall wrappers */

static int _fork(void);
static int _exit(int status);
static int _wait(int *status);
static int _pipe(int fildes[2]);
static int _kill(int pid, int sig);
static int _execve(char *name, char **argv, char **env);
static int _dup(int oldfd);
static int _getpid(void);
static int _sbrk(int incr);
static int _usleep(int usec);
static int _stat(char *file, struct stat *st);
static int _open(char *path, int flags);
static int _close(int file);
static int _read(int file, char *ptr, int len);
static int _write(int file, char *ptr, int len);
static int _lseek(int file, int ptr, int dir);
static int _chdir(const char *path);
static int _mknod(const char *path, int mode, int dev);
static int _unlink(char *name);
static int _link(char *old, char *new);

static int _tadd(void *task, int stack_size);
static int _tcancel(int id);
static int _tyield(void);
static int _tdelay(int ticks);
static int _tsuspend(int id);
static int _tresume(int id);
static int _tpriority(int id, int priority);
static int _tid(void);
static int _twfi(void);
static int _tcount(void);
static int _ticks(void);
static int _uptime(void);


/* syscall table */

static int (*const syscall_tbl[])() = {
	/* UNIX syscalls */
	[SYS_FORK] = _fork,
	[SYS_EXIT] = _exit,
	[SYS_WAIT] = _wait,
	[SYS_PIPE] = _pipe,
	[SYS_KILL] = _kill,
	[SYS_EXEC] = _execve,
	[SYS_DUP] = _dup,
	[SYS_GETPID] = _getpid,
	[SYS_SBRK] = _sbrk,
	[SYS_USLEEP] = _usleep,
	[SYS_STAT] = _stat,
	[SYS_OPEN] = _open,
	[SYS_CLOSE] = _close,
	[SYS_READ] = _read,
	[SYS_WRITE] = _write,
	[SYS_LSEEK] = _lseek,
	[SYS_CHDIR] = _chdir,
	[SYS_MKNOD] = _mknod,
	[SYS_LINK] = _link,
	[SYS_UNLINK] = _unlink,
	/* UCX-OS syscalls */
	[SYS_TADD] = _tadd,
	[SYS_TCANCEL] = _tcancel,
	[SYS_TYIELD] = _tyield,
	[SYS_TDELAY] = _tdelay,
	[SYS_TSUSPEND] = _tsuspend,
	[SYS_TRESUME] = _tresume,
	[SYS_TPRIORITY] = _tpriority,
	[SYS_TID] = _tid,
	[SYS_TWFI] = _twfi,
	[SYS_TCOUNT] = _tcount,
	[SYS_TICKS] = _ticks,
	[SYS_UPTIME] = _uptime
};


/* default syscall implementation - may be redefined according to arch*/

int _syscall(int, void *, void *, void *) __attribute__ ((weak, alias ("syscall_dsp")));

int syscall_dsp(int num, void *arg1, void *arg2, void *arg3)
{
	return syscall_tbl[num](arg1, arg2, arg3);
}


char *_env[1] = { 0 };
char **environ = _env;
int errno = 0;


/* UNIX syscalls (dummy implementation) */

int _fork(void)
{
	errno = EAGAIN;
	return -1;
}

int _exit(int status)
{
	_kill(status, -1);
	while (1);
	
	return -1;
}

int _wait(int *status)
{
	errno = ECHILD;
	return -1;
}

int _pipe(int fildes[2])
{
	errno = EFAULT;
	return -1;
}

int _kill(int pid, int sig)
{
	errno = EINVAL;
	return -1;
}

int _execve(char *name, char **argv, char **env)
{
	errno = ENOMEM;
	return -1;
}

int _dup(int oldfd)
{
	errno = EBADF;
	return -1;
}

int _getpid(void)
{
	return 1;
}

int _sbrk(int incr)
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

int _usleep(int usec)
{
	errno = EINTR;
	return 0;
}

int _stat(char *file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _open(char *path, int flags)
{
	return -1;
}

int _close(int file)
{
	return -1;
}

int _read(int file, char *ptr, int len)
{
	int idx;

	for (idx = 0; idx < len; idx++)
		*ptr++ = _getchar();

	return len;
}

int _write(int file, char *ptr, int len)
{
	int idx;

	for (idx = 0; idx < len; idx++)
		_putchar(*ptr++);

	return len;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int _chdir(const char *path)
{
	errno = EFAULT;
	return -1;
}

int _mknod(const char *path, int mode, int dev)
{
	errno = EFAULT;
	return -1;
}

int _unlink(char *name)
{
	errno = ENOENT;
	return -1;
}

int _link(char *old, char *new)
{
	errno = EMLINK;
	return -1;
}


/* UCX-OS syscalls (wrapper implementation and API) */

int _tadd(void *task, int stack_size)
{
	return ucx_task_spawn(task, stack_size);
}

int sys_task_add(void *task, int stack_size)
{
	return _syscall(SYS_TADD, task, (void *)stack_size, 0);
}


int _tcancel(int id)
{
	return ucx_task_cancel(id);
}

int sys_task_cancel(int id)
{
	return _syscall(SYS_TCANCEL, (void *)id, 0, 0);
}


int _tyield(void)
{
	ucx_task_yield();
	
	return 0;
}

int sys_task_yield(void)
{
	_syscall(SYS_TYIELD, 0, 0, 0);
	
	return 0;
}


int _tdelay(int ticks)
{
	ucx_task_delay(ticks);
	
	return 0;
}

int sys_task_delay(int ticks)
{
	_syscall(SYS_TDELAY, (void *)ticks, 0, 0);
	
	return 0;
}


int _tsuspend(int id)
{
	return ucx_task_suspend(id);
}

int sys_task_suspend(int id)
{
	return _syscall(SYS_TSUSPEND, (void *)id, 0, 0);
}


int _tresume(int id)
{
	return ucx_task_resume(id);
}

int sys_task_resume(int id)
{
	return _syscall(SYS_TRESUME, (void *)id, 0, 0);
}


int _tpriority(int id, int priority)
{
	return ucx_task_priority(id, priority);
}

int sys_task_priority(int id, int priority)
{
	return _syscall(SYS_TPRIORITY, (void *)id, (void *)priority, 0);
}


int _tid(void)
{
	return ucx_task_id();
}

int sys_task_id(void)
{
	return _syscall(SYS_TID, 0, 0, 0);
}


int _twfi(void)
{
	ucx_task_wfi();
	
	return 0;
}

int sys_task_wfi(void)
{
	_syscall(SYS_TWFI, 0, 0, 0);
	
	return 0;
}


int _tcount(void)
{
	return ucx_task_count();
}

int sys_task_count(void)
{
	return _syscall(SYS_TCOUNT, 0, 0, 0);
}


int _ticks(void)
{
	return ucx_ticks();
}

int sys_ticks(void)
{
	return _syscall(SYS_TICKS, 0, 0, 0);
}


int _uptime(void)
{
	return ucx_uptime();
}

int sys_uptime(void)
{
	return _syscall(SYS_UPTIME, 0, 0, 0);
}
