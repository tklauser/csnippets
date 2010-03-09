#ifndef _UNITTEST_H_
#define _UNITTEST_H_

#include <stdlib.h>
#include <stdio.h>
#include "stringify.h"

#define TEST_PASS	0
#define TEST_FAIL	1

#define assert_err(fmt, args...) \
	fprintf(stderr, "%s:%d: Assertion error: " fmt, __FILE__, __LINE__, ##args)

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
		assert_err(__stringify(a) " != " __stringify(b) "\n"); \
		exit(TEST_FAIL);	\
	}})

#endif /* _UNITTEST_H_ */
