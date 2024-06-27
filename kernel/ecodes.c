#include <ucx.h>

static struct ecode_s edatadesc[] = {
	{ERR_OK,			"no error"},
	{ERR_FAIL,			"failure"},
	{ERR_NO_TASKS,			"no tasks to run"},
	{ERR_KCB_ALLOC,			"kcb alloc failed"},
	{ERR_TCB_ALLOC,			"tcb alloc failed"},
	{ERR_STACK_ALLOC,		"stack alloc failed"},
	{ERR_TASK_NOT_FOUND,		"task not found"},
	{ERR_TASK_CANT_SUSPEND,		"task suspend failed"},
	{ERR_TASK_CANT_RESUME,		"task resume failed"},
	{ERR_TASK_INVALID_PRIO,		"task invalid priority"},
	{ERR_STACK_CHECK,		"stack check failed"},
	{ERR_PIPE_ALLOC,		"pipe alloc failed"},
	{ERR_PIPE_DEALLOC,		"pipe dealloc failed"},
	{ERR_SEM_ALLOC,			"sema alloc failed"},
	{ERR_SEM_DEALLOC,		"sema dealloc failed"},
	{ERR_SEM_OPERATION,		"sema operation failed"},
	{ERR_EQ_NOTEMPTY,		"message queue not empty"},
	{ERR_UNKNOWN,			"unknown reason"}
};

struct ecode_s *perror = edatadesc;
