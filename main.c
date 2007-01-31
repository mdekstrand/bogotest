/*****************************************************************************
 * main.c - main run
 ****************************************************************************/

#include <stdlib.h>
#include <setjmp.h>
#include <getopt.h>

#include <glib.h>

#include "bogotest.h"
#include "internal.h"

gboolean _bt_verbose = FALSE;
gboolean _bt_fork = TRUE;

int
main(int argc, char *argv[])
{
    int c;
    while ((c = getopt(argc, argv, "vF")) >= 0) {
        switch (c) {
        case 'v':
            _bt_verbose = TRUE;
            break;
        case 'F':
            _bt_fork = FALSE;
            break;
        }
    }
    initialize_test_suites();
    if (run_all_tests())
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}
