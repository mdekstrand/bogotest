/*****************************************************************************
 * internal.h - internal structures and declarations
 ****************************************************************************/

typedef struct _Test {
    BTTestInfo *info;
    gboolean success;
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

void initialize_test_suites(void);
void free_test_suites(void);
gboolean run_all_tests(void);
