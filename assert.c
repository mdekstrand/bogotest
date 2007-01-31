/*****************************************************************************
 * assert.c - assertion framework for BogoTest
 ****************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>

#include <glib.h>

#include "bogotest.h"
#include "internal.h"

void _bt_assert(int expr, const char *file, int line, const char *msg, ...)
{
    va_list ap;

    if (!expr) {
        gchar *lead, *fmsg;
        lead = g_strdup_printf("Failure at %s:%d", file, line);
        va_start(ap, msg);
        fmsg = g_strdup_vprintf(msg, ap);
        va_end(ap);
        fprintf(stderr, "%s: %s\n", lead, fmsg);
        g_free(lead);
        g_free(fmsg);
        longjmp(_bt_abort_buf, 1);
    }
}

void _bt_assert_strings_equal(const char *act, const char *exp,
        const char *file, int line, const char *expr)
{
    _bt_assert(g_string_equal(act, exp), file, line,
            "string %s has incorrect value\n  Actual: '%s'\n  Expected: '%s'",
            expr, act, exp);
}

void _bt_assert_doubles_equal(double act, double exp, double delta,
        const char *file, int line, const char *expr)
{
    _bt_assert(fabs(act - exp) < delta, file, line,
            "double %f has incorrect value\n  Actual: %f\n  Expected: %f",
            expr, act, exp);
}
