void _stdout_install(int (*hook)(int));
void _stdin_install(int (*hook)(void));
void _stdpoll_install(int (*hook)(void));
int _putchar(int c);
int _getchar(void);
int _kbhit(void);
