/*****************************************************************************
 * main.c - main run
 ****************************************************************************/

#include <stdlib.h>
#include <setjmp.h>
#include <glib.h>

#include "bogotest.h"
#include "internal.h"

int
main(int argc, char *argv[])
{
    initialize_all_tests();
    if (run_all_tests())
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}
