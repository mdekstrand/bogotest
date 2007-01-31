/*****************************************************************************
 * bogotest.h - master include file for BogoTest
 *
 * BogoTest (The Bogus Testing Framework for C) is a unit-testing framework
 * for C programmers.
 ****************************************************************************/

#ifndef BOGOTEST_H
#define BOGOTEST_H

#ifndef BOGOTEST_DOUBLE_DELTA
#define BOGOTEST_DOUBLE_DELTA 0.0001
#endif

typedef void (*BTTestFunc)(gpointer);
typedef gpointer (*BTTestSetupFunc)(void);
typedef void (*BTTestTeardownFunc)(gpointer);
typedef gboolean (*BTSuiteFunc)(void);

typedef struct _BTTestInfo {
    const char *name;
    BTTestFunc test;
} BTTestInfo;
#define BT_TEST_INFO_NULL { NULL, NULL }

typedef struct _BTFixtureInfo {
    BTTestSetupFunc setup;
    BTTestTeardownFunc teardown;
    BTTestInfo *tests;
} BTFixtureInfo;
#define BT_FIXTURE_INFO_NULL { NULL, NULL, NULL }

typedef struct _BTSuiteInfo {
    const char *name;
    BTFixtureInfo *fixtures;
} BTSuiteInfo;
#define BT_SUITE_INFO_NULL { NULL, NULL }

void _bt_assert(int expr, const char *file, int line, const char *msg, ...);
#define BT_ASSERT(expr) _bt_assert((expr), __FILE__, __LINE__, \
        "assertion failed: `%s`", #expr)
#define BT_ASSERT_MESSAGE(expr, msg, ...) _bt_assert((expr), __FILE__, __LINE__, msg, __VA_ARGS__)

#define BT_ASSERT_PTR_NULL(ptr) _bt_assert((ptr) == NULL, __FILE__, __LINE__, \
        "pointer %s isn't NULL", #ptr)
#define BT_ASSERT_PTR_NULL_MESSAGE(ptr, msg, ...) _bt_assert(ptr == NULL, __FILE__, __LINE__, \
        msg, __VA_ARGS__)
#define BT_ASSERT_PTR_NOT_NULL(ptr) _bt_assert(ptr != NULL, __FILE__, __LINE__, \
        "pointer %s is NULL", #ptr)
#define BT_ASSERT_PTR_NOT_NULL_MESSAGE(ptr, msg, ...) _bt_assert(ptr != NULL, __FILE__, __LINE__, \
        msg, __VA_ARGS__)
#define BT_ASSERT_EQUAL(act, exp) BT_ASSERT(exp == act)

void _bt_assert_strings_equal(const char *act, const char *exp,
        const char *file, int line, const char *expr);
#define BT_ASSERT_STRINGS_EQUAL(actual, expected) _bt_assert_strings_equal( \
        (actual), (expected), __FILE__, __LINE__, #actual)

void _bt_assert_doubles_equal(double act, double exp, double delta,
        const char *file, int line, const char *expr);
#define BT_ASSERT_DOUBLES_EQUAL(actual, expected) _bt_assert_doubles_equal( \
        (actual), (expected), BOGOTEST_DOUBLE_DELTA, __FILE__, __LINE__, #actual)

#endif
