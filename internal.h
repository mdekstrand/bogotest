/*****************************************************************************
 * internal.h - internal structures and declarations
 *
 * Copyright (c) 2007 Michael Ekstrand
 *
 * This file is freely distributable, so long as the above copyright notice
 * remains intact; see the file COPYING for details.
 ****************************************************************************/

typedef enum {
    RESULT_TYPE_SUCCESS,
    RESULT_TYPE_SIGNAL,
    RESULT_TYPE_EXIT
} resultmode_t;

typedef struct _Test {
    BTTestInfo *info;
    gboolean success;
    resultmode_t result_mode;
    int result_value;
} Test;

typedef struct _TestFixture {
    BTFixtureInfo *info;
    GList *tests;
} TestFixture;

typedef struct _TestSuite {
    BTSuiteInfo *info;
    int failures;
    GList *fixtures;
} TestSuite;

extern const BTSuiteInfo bogotest_suites[];
extern GList *_bt_suites;
extern jmp_buf _bt_abort_buf;
extern gboolean _bt_verbose;
extern gboolean _bt_fork;
extern gboolean _bt_fatal_failures;

void initialize_test_suites(void);
void free_test_suites(void);
gboolean run_all_tests(void);
