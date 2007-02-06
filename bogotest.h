/*****************************************************************************
 * bogotest.h - master include file for BogoTest
 *
 * BogoTest (The Bogus Testing Framework for C) is a unit-testing framework
 * for C programmers.
 *
 * Copyright (c) 2007 Michael Ekstrand
 *
 * This file is freely distributable, so long as the above copyright notice
 * remains intact; see the file COPYING for details.
 ****************************************************************************/

#ifndef BOGOTEST_H
#define BOGOTEST_H

#ifndef BOGOTEST_DOUBLE_DELTA
#define BOGOTEST_DOUBLE_DELTA 0.0001
#endif

typedef void (*BTTestFunc)(gpointer);
#define BT_TEST_FUNC(func) ((BTTestFunc) (func))
typedef gpointer (*BTTestSetupFunc)(void);
#define BT_TEST_SETUP_FUNC(func) ((BTTestSetupFunc) (func))
typedef void (*BTTestTeardownFunc)(gpointer);
#define BT_TEST_TEARDOWN_FUNC(func) ((BTTestTeardownFunc) (func))

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
#define BT_ASSERT_PTR_NULL_MESSAGE(ptr, msg, ...) _bt_assert((ptr) == NULL, __FILE__, __LINE__, \
        msg, __VA_ARGS__)
#define BT_ASSERT_PTR_NOT_NULL(ptr) _bt_assert((ptr) != NULL, __FILE__, __LINE__, \
        "pointer %s is NULL", #ptr)
#define BT_ASSERT_PTR_NOT_NULL_MESSAGE(ptr, msg, ...) _bt_assert((ptr) != NULL, __FILE__, __LINE__, \
        msg, __VA_ARGS__)

#define BT_ASSERT_EQUAL(act, exp) BT_ASSERT((act) == (exp))
#define BT_ASSERT_NOT_EQUAL(act, rej) BT_ASSERT((act) != (rej))

void _bt_assert_strings_equal(const char *act, const char *exp,
        const char *file, int line, const char *expr);
#define BT_ASSERT_STRINGS_EQUAL(actual, expected) _bt_assert_strings_equal( \
        (actual), (expected), __FILE__, __LINE__, #actual)

int _bt_doubles_equal(double d1, double d2, double delta);
void _bt_assert_doubles_equal(double act, double exp, double delta,
        const char *file, int line, const char *expr);
#define BT_ASSERT_DOUBLES_EQUAL(actual, expected) _bt_assert_doubles_equal( \
        (actual), (expected), BOGOTEST_DOUBLE_DELTA, __FILE__, __LINE__, #actual)
#define BT_ASSERT_DOUBLES_EQUAL_MSG(act, exp, msg, ...) BT_ASSERT_MESSAGE( \
        _bt_doubles_equal((act), (exp), BOGOTEST_DOUBLE_DELTA), \
        msg "\n  Actual: %f\n  Expected: %f", __VA_ARGS__, (act), (exp))
#ifdef G_VALUE_TYPE
void _bt_assert_gvalue_is_type(GValue *val, GType type,
        const char *file, int line, const char *expr);
#define BT_ASSERT_GVALUE_IS_TYPE(val, type) _bt_assert_gvalue_is_type( \
        (val), (type), __FILE__, __LINE__, #val)
#endif

#endif
