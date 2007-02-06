/*****************************************************************************
 * runner.c - running tests
 *
 * Copyright (c) 2007 Michael Ekstrand
 *
 * This file is freely distributable, so long as the above copyright notice
 * remains intact; see the file COPYING for details.
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
            fprintf(stderr, "Test failed: %s\n", test->info->name);
            return FALSE;
        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "Test terminated with signal %d: %s\n",
                    WTERMSIG(status), test->info->name);
            return FALSE;
        } else if (!WIFEXITED(status)) {
            fprintf(stderr, "Test terminated erroneously: %s\n", test->info->name);
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
        if (_bt_fork) {
            exit(success ? EXIT_SUCCESS : EXIT_FAILURE);
        } else {
            if (!_bt_fork && !success)
                fprintf(stderr, "Test failed: %s\n", test->info->name);
            return success;
        }
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
    return failures;
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
    int failures = 0;
    int fixtures = 0;
    int tests = 0;
    int suites = 0;
    GList *cur;
    cur = _bt_suites;
    while (cur) {
        GList *cfix;
        TestSuite *suite = (TestSuite*) (cur->data);
        cfix = suite->fixtures;
        while (cfix) {
            TestFixture *fixture = (TestFixture*) (cfix->data);
            tests += g_list_length(fixture->tests);
            ++fixtures;
            cfix = cfix->next;
        }
        if (suite->failures) {
            failures += suite->failures;
            fprintf(stderr, "Suite %s had %d failures\n",
                    suite->info->name, suite->failures);
        }
        cur = cur->next;
        ++suites;
    }

    if (failures)
        printf("%d failures/errors in %d suites with %d tests in %d fixtures\n",
                failures, suites, tests, fixtures);
    else
        printf("No errors in %d suites with %d tests in %d fixtures\n",
                suites, tests, fixtures);

    return !failures;
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
