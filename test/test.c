#include <stdlib.h>
#include <glib.h>
#include <glib-object.h>

#include "../bogotest.h"

void run_test(gpointer dat)
{
#include "test.text"
}

BTTestInfo tests[] = {
    { "master", run_test },
    BT_TEST_INFO_NULL
};

BTFixtureInfo fixtures[] = {
    { NULL, NULL, tests },
    BT_FIXTURE_INFO_NULL
};

BTSuiteInfo bogotest_suites[] = {
    { "master", fixtures },
    BT_SUITE_INFO_NULL
};
