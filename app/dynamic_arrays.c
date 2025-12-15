#include <ucx.h>
#include <dynarray.h>

void idle(void)
{
	while (1) {
		ucx_task_yield();
	}
}

void task0(void)
{
	struct dynarrayint_s int_array = { 0 };
	unsigned int rnd;

	while (1) {
		rnd = random() % 256;
		
		for (int i = 0; i < rnd; i++) {
			dyn_append(int_array, random());
			printf("heap: %p, at %d added %d, size %d\n", int_array.items,
				i, int_array.items[i], int_array.capacity);
		}
		
		for (int i = 0; i < int_array.count; i++)
			printf("%d ", int_array.items[i]);
		printf("\n");

		dyn_destroy(int_array);			
		ucx_task_delay(100);
	}
}

int app_main(void)
{
	ucx_task_spawn(idle, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	
	return 1;
}
