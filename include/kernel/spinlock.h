struct spinlock_s {
	uint8_t locked;
	uint16_t cpu;
};

void ucx_lock_init(struct spinlock_s *lock);
void ucx_lock_acquire(struct spinlock_s *lock);
void ucx_lock_release(struct spinlock_s *lock);
