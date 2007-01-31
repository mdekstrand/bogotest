/*****************************************************************************
 * runner.c - running tests
 ****************************************************************************/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <stdio.h>

#include <glib.h>

#include "bogotest.h"
#include "internal.h"

jmp_buf _bt_abort_buf;

static gboolean
run_test(BTTestSetupFunc setup, BTTestTeardownFunc teardown, Test *test)
{
    pid_t pid;
    if (_bt_verbose)
        printf("  test: %s\n", test->info->name);
    if (_bt_fork)
        pid = fork();
    if (_bt_fork && (pid > 0)) {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("run_test:waitpid");
            return FALSE;
        }
        if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS) {
            fprintf(stderr, "Test %s failed\n", test->info->name);
            return FALSE;
        } else if (!WIFEXITED(pid)) {
            fprintf(stderr, "Error occured in test %s\n", test->info->name);
            return FALSE;
        }
        return TRUE;
    } else if (_bt_fork && pid) {
        perror("run_test:fork");
        abort();
    } else {
        gpointer fdata = NULL;
        gboolean success = TRUE;
        if (setup && !(fdata = (setup)())) {
            fputs("Error in setup for test fixture", stderr);
            abort();
        }
        if (setjmp(_bt_abort_buf)) {
            /* error! */
            success = FALSE;
        } else {
            (test->info->test)(fdata);
        }
        if (teardown)
            (teardown)(fdata);
        if (_bt_fork)
            exit(success ? EXIT_SUCCESS : EXIT_FAILURE);
        else
            return success;
    }
}

static int
run_test_fixture(TestFixture *fixture)
{
    int failures = 0;
    GList *cur = NULL;

    cur = fixture->tests;
    while (cur) {
        if (!run_test(fixture->info->setup, fixture->info->teardown,
                    (Test*) (cur->data)))
            ++failures;
        cur = cur->next;
    }
}

static void
run_test_suite(TestSuite* suite)
{
    GList *cur;
    if (_bt_verbose)
        printf("Suite: %s\n", suite->info->name);
    cur = suite->fixtures;
    while (cur) {
        suite->failures += run_test_fixture((TestFixture*) (cur->data));
        cur = cur->next;
    }
}

gboolean
summarize_results(void)
{
    gboolean success = TRUE;
    GList *cur;
    cur = _bt_suites;
    while (cur) {
        TestSuite *suite = (TestSuite*) (cur->data);
        if (suite->failures) {
            success = FALSE;
            fprintf(stderr, "Suite %s had %d failures",
                    suite->info->name, suite->failures);
        }
        cur = cur->next;
    }
    if (success)
        puts("No errors.");
    return success;
}

gboolean
run_all_tests(void)
{
    GList *cur = _bt_suites;
    while (cur) {
        run_test_suite((TestSuite*) (cur->data));
        cur = cur->next;
    }
    return summarize_results();
}
