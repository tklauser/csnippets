/*
 * memdebug - A simple malloc/free debugger
 *
 * This can essentially be useful on embedded systems platforms where tools such
 * as valgrind are not available.
 *
 * Copyright (c) 2010 Zurich University of Applied Sciences
 * Copyright (c) 2010 Tobias Klauser <tklauser@distanz.ch>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _MEMDEBUG_H_
#define _MEMDEBUG_H_

#include <stdlib.h>

#ifdef MEMDEBUG_TRACE
# define memdebug_trace(fmt, args...) \
	({ \
	 fprintf(stderr, "*** " fmt " ***\n", ##args); \
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

	const char *freed_func;
	const char *freed_file;
	unsigned int freed_line;
};

#define memdebug_malloc(size) \
	__memdebug_malloc(size, __func__, __FILE__, __LINE__)
#define memdebug_free(ptr) \
	__memdebug_free(ptr, __func__, __FILE__, __LINE__)

void *__memdebug_malloc(size_t size, const char *func,
                        const char *file, const unsigned int line);
void __memdebug_free(void *ptr, const char *func,
                   const char *file, const unsigned int line);
void memdebug_report(int report_all);

#endif /* _MEMDEBUG_H_ */
