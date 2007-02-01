/*****************************************************************************
 * data.c - data structures in BogoTest
 *
 * Copyright (c) 2007 Michael Ekstrand
 *
 * This file is freely distributable, so long as the above copyright notice
 * remains intact; see the file COPYING for details.
 ****************************************************************************/

#include <setjmp.h>

#include <glib.h>

#include "bogotest.h"
#include "internal.h"

GList *_bt_suites;

static void
free_test(Test *test)
{
    g_free(test);
}

static void
free_test_fixture(TestFixture *fixture)
{
    g_list_foreach(fixture->tests, (GFunc) free_test, NULL);
    g_list_free(fixture->tests);
    g_free(fixture);
}

static void
free_test_suite(TestSuite *suite)
{
    g_list_foreach(suite->fixtures, (GFunc) free_test_fixture, NULL);
    g_list_free(suite->fixtures);
    g_free(suite);
}

static GList*
initialize_tests(BTTestInfo *tests)
{
    BTTestInfo *ctest;
    GList *tlist = NULL;
    for (ctest = tests; ctest->test; ++ctest) {
        Test *test = g_new0(Test, 1);
        test->info = ctest;
        tlist = g_list_append(tlist, test);
    }
    return tlist;
}

static GList*
initialize_fixtures(BTFixtureInfo *fixtures)
{
    BTFixtureInfo *cfix;
    GList *flist = NULL;
    for (cfix = fixtures; cfix->tests; ++cfix) {
        TestFixture *fix = g_new0(TestFixture, 1);
        fix->info = cfix;
        fix->tests = initialize_tests(cfix->tests);
        flist = g_list_append(flist, fix);
    }
    return flist;
}

void
initialize_test_suites(void)
{
    const BTSuiteInfo *si;
    for (si = bogotest_suites; si->fixtures; ++si) {
        TestSuite *suite = g_new0(TestSuite, 1);
        suite->info = si;
        _bt_suites = g_list_append(_bt_suites, suite);
        suite->fixtures = initialize_fixtures(si->fixtures);
    }
}

void
free_test_suites(void)
{
    g_list_foreach(_bt_suites, (GFunc) free_test_suite, NULL);
    g_list_free(_bt_suites);
    _bt_suites = NULL;
}
