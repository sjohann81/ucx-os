struct dynarraychar_s {
	char *items;
	size_t count;
	size_t capacity;
};

struct dynarrayint_s {
	int *items;
	size_t count;
	size_t capacity;
};

struct dynarraylong_s {
	long *items;
	size_t count;
	size_t capacity;
};

struct dynarraylonglong_s {
	long long *items;
	size_t count;
	size_t capacity;
};

struct dynarrayfloat_s {
	float *items;
	size_t count;
	size_t capacity;
};

struct dynarrayptr_s {
	void **items;
	size_t count;
	size_t capacity;
};

#define dyn_append(arr, x) \
	do { \
		if (arr.count >= arr.capacity) { \
			if (arr.capacity == 0) arr.capacity = 16; \
			else arr.capacity <<= 1; \
			arr.items = realloc(arr.items, arr.capacity * sizeof(*arr.items)); \
		} \
		arr.items[arr.count++] = x; \
	} while (0)

#define dyn_destroy(arr) \
	do { \
		if (arr.count > 0) { \
			arr.count = 0; \
			arr.capacity = 0; \
			free(arr.items); \
		} \
	} while (0)
