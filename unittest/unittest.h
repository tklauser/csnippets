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
