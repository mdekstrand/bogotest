/*****************************************************************************
 * data.c - data structures in BogoTest
 ****************************************************************************/

#include <setjmp.h>

#include <glib.h>

#include "bogotest.h"
#include "internal.h"

GList *_bt_suites;

void
free_test(Test *test)
{
    g_free(test);
}

void
free_test_fixture(TestFixture *fixture)
{
    g_list_foreach(fixture->tests, free_test, NULL);
    g_list_free(fixture->tests);
    g_free(fixture);
}

void
free_test_suite(TestSuite *suite)
{
    g_list_foreach(suite->fixtures, free_test_fixture, NULL);
    g_list_free(suite->fixtures);
    g_free(suite);
}

GList*
initialize_tests(BTTestInfo *tests)
{
    BTTestInfo *ctest;
    GList *tlist = NULL;
    for (ctest = tests; ctest->name; ++ctest) {
        Test *test = g_new0(Test, 1);
        test->info = ctest;
        tlist = g_list_append(tlist, test);
    }
    return tlist;
}

GList*
initialize_fixtures(BTFixtureInfo *fixtures)
{
    BTFixtureInfo *cfix;
    GList *flist = NULL;
    for (cfix = fixtures; cfix->setup; ++cfix) {
        TestFixture *fix = g_new0(TestFixture, 1);
        fix->info = cfix;
        fix->tests = initialize_tests(fixtures->tests);
        flist = g_list_append(flist, fix);
    }
    return flist;
}

void
initialize_test_suites(void)
{
    BTSuiteInfo *si;
    for (si = bogotest_suites; si->name; ++si) {
        TestSuite *suite = g_new0(TestSuite, 1);
        suite->info = si;
        _bt_suites = g_list_append(_bt_suites, suite);
        suite->fixtures = initialize_fixtures(si->fixtures);
    }
}

void
free_test_suites(void)
{
    g_list_foreach(_bt_suites, free_test_suite, NULL);
    g_list_free(_bt_suites);
    _bt_suites = NULL;
}
