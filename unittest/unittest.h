/*
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

#ifndef _UNITTEST_H_
#define _UNITTEST_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stringify.h"

#define TEST_PASS	0
#define TEST_FAIL	1

#define test_print(fmt, args...) \
	fprintf(stderr, "%s:%d:" fmt, __FILE__, __LINE__, ##args)

#define assert_err(fmt, args...) \
	test_print("Assertion error: " fmt, ##args)

/* Assert the expression 'expr' */
#define test_assert(expr) ({if (!(expr)) {		\
		assert_err(__stringify(expr) "\n");	\
		exit(TEST_FAIL);			\
		}})

/* Assert the expression 'expr' and print 'msg' on fail */
#define test_assert_msg(expr, msg) ({if (!(expr)) {	\
		assert_err(msg "\n");			\
		exit(TEST_FAIL);			\
		}})

/* Assert that a == b */
#define test_assert_eq(a, b) ({		\
	if ((a) != (b)) {		\
		assert_err(__stringify(a) " != " __stringify(b) "\n"); \
		exit(TEST_FAIL);	\
	}})

/* Assert that a != b */
#define test_assert_neq(a, b) ({	\
	if ((a) == (b)) {		\
		assert_err(__stringify(a) " == " __stringify(b) "\n"); \
		exit(TEST_FAIL);	\
	}})

/* Assert that a is NULL */
#define test_assert_null(a) ({	\
	if ((a) != NULL) {		\
		assert_err(__stringify(a) " not NULL\n"); \
		exit(TEST_FAIL);	\
	}})

/* Assert that a is not NULL */
#define test_assert_not_null(a) ({	\
	if ((a) == NULL) {		\
		assert_err(__stringify(a) " is NULL\n"); \
		exit(TEST_FAIL);	\
	}})

static inline int __xstrmatch(const char *ctx, const char *arg)
{
	size_t len = strlen(ctx);
	return (strlen(arg) == len && strncmp(arg, ctx, len) == 0);
}

/* Assert that string a matches string b */
#define test_assert_str_eq(a, b) ({	\
	if (!__xstrmatch((a), (b))) {		\
		assert_err(__stringify(a) " does not match " __stringify(b) "\n"); \
		exit(TEST_FAIL);	\
	}})

#endif /* _UNITTEST_H_ */
