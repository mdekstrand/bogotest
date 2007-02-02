/*****************************************************************************
 * main.c - main run
 *
 * Copyright (c) 2007 Michael Ekstrand
 *
 * This file is freely distributable, so long as the above copyright notice
 * remains intact; see the file COPYING for details.
 ****************************************************************************/

#include <stdlib.h>
#include <setjmp.h>
#include <getopt.h>

#include <glib.h>
#include <glib-object.h>

#include "bogotest.h"
#include "internal.h"

gboolean _bt_verbose = FALSE;
gboolean _bt_fork = TRUE;
gboolean _bt_fatal_failures = FALSE;

int
main(int argc, char *argv[])
{
    int c;
    g_type_init();
    while ((c = getopt(argc, argv, "vFk")) >= 0) {
        switch (c) {
        case 'v':
            _bt_verbose = TRUE;
            break;
        case 'F':
            _bt_fork = FALSE;
            break;
        case 'k':
            _bt_fatal_failures = TRUE;
            break;
        }
    }

    if (_bt_fatal_failures)
        _bt_fork = FALSE;

    initialize_test_suites();
    if (run_all_tests())
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}
