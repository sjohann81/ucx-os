#include <ucx.h>

/* 
 * non-irq implementation of spinlocks
 * 
 * worth reading: https://www.kernel.org/doc/Documentation/locking/spinlocks.txt
 */

void ucx_lock_init(struct spinlock_s *lock)
{
	lock->locked = 0;
	lock->cpu = -1;
}

/*
 * check whether this cpu is holding the lock.
 */
static int holding(struct spinlock_s *lock)
{
	int r;
	r = (lock->locked && lock->cpu == _cpu_id());
	
	return r;
}

/* 
 * acquire the lock - spins until the lock is acquired.
 */
void ucx_lock_acquire(struct spinlock_s *lock)
{
	if (holding(lock))
		return;

	while (__sync_lock_test_and_set(&lock->locked, 1));
	__sync_synchronize();
	
	lock->cpu = _cpu_id();
}

/* 
 * Release the lock.
 */
void ucx_lock_release(struct spinlock_s *lock)
{
	if (!holding(lock))
		return;

	lock->cpu = -1;

	__sync_synchronize();
	__sync_lock_release(&lock->locked);
}
