/*
 * memdebug - A simple kmalloc/kfree debugger
 *
 * This can essentially be useful on embedded systems platforms where tools such
 * as valgrind are not available.
 *
 * Copyright (c) 2010 Zurich University of Applied Sciences
 * Copyright (c) 2010 Tobias Klauser <tklauser@distanz.ch>
 * 
 * HISTORY
 * 12.11.2010	gram	Original sources modified for use in kernel modules.
 * 						Sections (comment blocks) added.
 * 						Bug in function 'memdebug_cleanup' corrected (access
 * 						just freed struct)
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

/*******************************************************************************
   Includes
*******************************************************************************/

#include <linux/types.h>
#include <linux/slab.h>

/*******************************************************************************
   Definitions
*******************************************************************************/

/* define/undef to enable tracing of errors */
//#define MEMDEBUG_TRACE
#include "memdebug.h"

/*******************************************************************************
   Structs, Types and Enums
*******************************************************************************/

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

/*******************************************************************************
   Module Global Variable
*******************************************************************************/

static struct memdebug_heap_item *heap_head = NULL;
static struct memdebug_heap_item *heap_tail = NULL;

/*******************************************************************************
   Public Functions
*******************************************************************************/

void memdebug_start(void)
{
	/* Empty for now */
}

void memdebug_cleanup(void)
{
	struct memdebug_heap_item *hi, *next;

	for (hi = heap_head; hi != NULL; hi = next) {
		if (hi->addr != NULL){
			kfree(hi->addr);
		}
		next = hi->next;
		kfree(hi);
	}

	heap_head = NULL;
	heap_tail = NULL;
}

void *__memdebug_malloc(size_t size, int priority, const char *func,
                        const char *file, const unsigned int line)
{
	void *ret;
	struct memdebug_heap_item *hi = kmalloc(sizeof(struct memdebug_heap_item), priority);

	if (!hi)
		goto out_err;

	ret = kmalloc(size, priority);
	if (!ret) {
		kfree(hi);
		goto out_err;
	}

	hi->next = NULL;
	hi->addr = ret;
	hi->size = size;
	hi->func = func;
	hi->file = file;
	hi->line = line;
	hi->freed_func = NULL;
	hi->freed_file = NULL;
	hi->line = 0;

	if (!heap_head)
		heap_head = hi;
	if (heap_tail)
		heap_tail->next = hi;
	heap_tail = hi;

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
		if (hi->addr == ptr)
			break;

	if (!hi) {
		memdebug_trace("no memory allocated at %p (%s:%d:%s)", ptr, file, line, func);
		return;
	}

	if (hi->freed_func) {
		memdebug_trace("double free at %p (%s:%d:%s)\nalready freed by %s:%d:%s",
				ptr, file, line, func, hi->freed_file, hi->freed_line, hi->freed_func);
		return;
	}

	hi->freed_func = func;
	hi->freed_file = file;
	hi->freed_line = line;

	/* Free the memory but keep the memdebug information */
	kfree(hi->addr);
	hi->addr = NULL;
}

void memdebug_report(bool report_free)
{
	struct memdebug_heap_item *hi;
	size_t total_size = 0;
	size_t still_allocated = 0;

	printk(KERN_DEBUG "\n\n*** Reporting allocated/freed memory ***\n");
	for (hi = heap_head; hi; hi = hi->next) {
		if (hi->addr) {
			printk(KERN_DEBUG "  IN USE  %zu bytes at %p (%s:%d:%s)\n",
					hi->size, hi->addr, hi->file, hi->line, hi->func);
			still_allocated += hi->size;
		} else if (report_free) {
			printk(KERN_DEBUG "  FREE    %zu bytes at %p (%s:%d:%s)\n",
					hi->size, hi->addr, hi->file, hi->line, hi->func);
		}
		total_size += hi->size;
	}
	printk(KERN_DEBUG "Total size: %zu bytes, %zu bytes still allocated\n\n",
			total_size, still_allocated);
}

void memdebug_report_brief(void)
{
	struct memdebug_heap_item *hi;
	size_t total_size = 0;
	size_t still_allocated = 0;

	printk(KERN_DEBUG "\n\n*** Reporting allocated/freed memory ***\n");
	for (hi = heap_head; hi; hi = hi->next) {
		if (hi->addr) {
			still_allocated += hi->size;
		}
		total_size += hi->size;
	}
	printk(KERN_DEBUG "Total size: %zu bytes, %zu bytes still allocated\n\n",
			total_size, still_allocated);
}
