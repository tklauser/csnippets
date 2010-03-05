#ifndef _MEMDEBUG_H_
#define _MEMDEBUG_H_

#include <stdlib.h>

#ifdef MEMDEBUG_TRACE
# define memdebug_trace(fmt, args...) \
	({ \
	 fprintf(stderr, "\n\n*** " fmt " ***\n\n", ##args); \
	 fflush(stderr); \
	 })
#else
# define memdebug_trace(fmt, args...)
#endif

struct memdebug_heap_item {
	struct memdebug_heap_item *next;

	void *addr;
	size_t size;

	const char *func;
	const char *file;
	unsigned int line;

#ifdef MEMDEBUG_TRACE_DOUBLE_FREE
	const char *freed_func;
	const char *freed_file;
	unsigned int freed_line;
#endif
};

#define memdebug_malloc(size) \
	__memdebug_malloc(size, __func__, __FILE__, __LINE__)
#define memdebug_free(ptr) \
	__memdebug_free(ptr, __func__, __FILE__, __LINE__)

void *__memdebug_malloc(size_t size, const char *func,
                        const char *file, const unsigned int line);
void __memdebug_free(void *ptr, const char *func,
                   const char *file, const unsigned int line);
void memdebug_report(void);

#endif /* _MEMDEBUG_H_ */
