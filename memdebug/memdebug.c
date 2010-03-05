/*
 * memdebug - A simple malloc/free debugger
 *
 * This can essentially be useful on embedded systems platforms where tools such
 * as valgrind are not available.
 *
 * Copyright (c) 2010, Tobias Klauser <tklauser@distanz.ch>
 */

#include <stdio.h>
#include <stdlib.h>

/* define/undef to enable tracing of errors */
#define MEMDEBUG_TRACE
/* define/undef to enable tracing of double free errors */
#define MEMDEBUG_TRACE_DOUBLE_FREE

#include "memdebug.h"

static struct memdebug_heap_item *heap_head = NULL;
static struct memdebug_heap_item *heap_tail = NULL;

void *__memdebug_malloc(size_t size, const char *func,
                        const char *file, const unsigned int line)
{
	void *ret;
	struct memdebug_heap_item *hi = malloc(sizeof(struct memdebug_heap_item));

	if (!hi)
		goto out_err;

	ret = malloc(size);
	if (!ret) {
		free(hi);
		goto out_err;
	}

	hi->next = NULL;
	hi->addr = ret;
	hi->size = size;
	hi->func = func;
	hi->file = file;
	hi->line = line;
#ifdef MEMDEBUG_TRACE_DOUBLE_FREE
	hi->freed_func = NULL;
	hi->freed_file = NULL;
	hi->line = 0;
#endif

	if (!heap_head)
		heap_head = hi;
	if (!heap_tail)
		heap_tail = hi;
	else {
		heap_tail->next = hi;
		heap_tail = hi;
	}

	return ret;

out_err:
	memdebug_trace("malloc failed (%s:%d:%s)", file, line, func);
	return NULL;
}

void __memdebug_free(void *ptr, const char *func,
                     const char *file, const unsigned int line)
{
	struct memdebug_heap_item *hi, *prev;

	if (!ptr) {
		memdebug_trace("trying to free NULL pointer (%s:%d:%s)", file, line, func);
		return;
	}

	for (hi = heap_head, prev = NULL; hi; prev = hi, hi = hi->next)
		if (hi->addr ==ptr)
			break;

	if (!hi) {
		memdebug_trace("no memory allocated at %p (%s:%d:%s)", ptr, file, line, func);
		return;
	}

#ifdef MEMDEBUG_TRACE_DOUBLE_FREE
	if (hi->freed_func) {
		memdebug_trace("double free at %p (%s:%d:%s)\nalready freed by %s:%d:%s",
				ptr, file, line, func, hi->freed_file, hi->freed_line, hi->freed_func);
		return;
	}

	hi->freed_func = func;
	hi->freed_file = file;
	hi->freed_line = line;
#else
	/* Delete the item from the linked list */
	if (prev)
		prev->next = hi->next;
	else
		heap_head = hi->next;
	if (heap_tail == hi)
		heap_tail = prev;

	free(hi);
#endif

	free(ptr);
}

void memdebug_report(void)
{
	struct memdebug_heap_item *hi;
	size_t total_size = 0;

	fprintf(stderr, "\n\n*** Reporting allocated memory ***\n");
	for (hi = heap_head; hi; hi = hi->next) {
		fprintf(stderr, "  %zu bytes at %p (%s:%d:%s)\n", hi->size, hi->addr, hi->file, hi->line, hi->func);
		total_size += hi->size;
	}
	fprintf(stderr, "Total size: %zu bytes\n\n", total_size);
	fflush(stderr);
}
