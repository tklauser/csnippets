/*
 * memdebug - A simple malloc/free debugger
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
 * 						Prototype for function 'memdebug_cleanup' added
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

#ifndef MEMDEBUG_H_
#define MEMDEBUG_H_

/*******************************************************************************
   Includes
*******************************************************************************/


/*******************************************************************************
   Definitions
*******************************************************************************/


/*******************************************************************************
   Structs, Types and Enums
*******************************************************************************/


/*******************************************************************************
   Macros
*******************************************************************************/

#ifdef MEMDEBUG_TRACE
# define memdebug_trace(fmt, args...) \
	({ \
	 printk(KERN_DEBUG "*** " fmt " ***\n", ##args); \
	 })
#else
# define memdebug_trace(fmt, args...)
#endif

#define memdebug_malloc(size, priority) \
	__memdebug_malloc(size, priority, __func__, __FILE__, __LINE__)
#define memdebug_free(ptr) \
	__memdebug_free(ptr, __func__, __FILE__, __LINE__)

/*******************************************************************************
   Prototypes for external functions
*******************************************************************************/

void memdebug_cleanup(void);
void *__memdebug_malloc(size_t size, int priority, const char *func,
                        const char *file, const unsigned int line);
void __memdebug_free(void *ptr, const char *func,
                   const char *file, const unsigned int line);
void memdebug_report(bool report_free);
void memdebug_report_brief(void);

#endif /* MEMDEBUG_H_ */
