enum UCX_SYSCALLS {
	SYS_FORK = 1, SYS_EXIT, SYS_WAIT, SYS_PIPE,
	SYS_KILL, SYS_EXEC, SYS_DUP, SYS_GETPID,
	SYS_SBRK, SYS_USLEEP, SYS_STAT, SYS_OPEN,
	SYS_CLOSE, SYS_READ, SYS_WRITE, SYS_LSEEK,
	SYS_CHDIR, SYS_MKNOD, SYS_LINK, SYS_UNLINK,
	SYS_TADD = 32, SYS_TCANCEL, SYS_TYIELD, SYS_TDELAY,
	SYS_TSUSPEND, SYS_TRESUME, SYS_TPRIORITY, SYS_TID,
	SYS_TWFI, SYS_TCOUNT, SYS_TICKS, SYS_UPTIME
};

int _syscall(int num, void *arg1, void *arg2, void *arg3);

int sys_fork(void);
int sys_exit(int status);
int sys_wait(int *status);
int sys_pipe(int fildes[2]);
int sys_kill(int pid, int sig);
int sys_execve(char *name, char **argv, char **env);
int sys_dup(int oldfd);
int sys_getpid(void);
int sys_sbrk(int incr);
int sys_usleep(int usec);
int sys_stat(char *file, struct stat *st);
int sys_open(char *path, int flags);
int sys_close(int file);
int sys_read(int file, char *ptr, int len);
int sys_write(int file, char *ptr, int len);
int sys_lseek(int file, int ptr, int dir);
int sys_chdir(const char *path);
int sys_mknod(const char *path, int mode, int dev);
int sys_unlink(char *name);
int sys_link(char *old, char *new);

int sys_tadd(void *task, int stack_size);
int sys_tcancel(int id);
int sys_tyield(void);
int sys_tdelay(int ticks);
int sys_tsuspend(int id);
int sys_tresume(int id);
int sys_tpriority(int id, int priority);
int sys_tid(void);
int sys_twfi(void);
int sys_tcount(void);
int sys_ticks(void);
int sys_uptime(void);
