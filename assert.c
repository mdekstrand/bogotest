/*****************************************************************************
 * assert.c - assertion framework for BogoTest
 *
 * Copyright (c) 2007 Michael Ekstrand
 *
 * This file is freely distributable, so long as the above copyright notice
 * remains intact; see the file COPYING for details.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <string.h>
#include <math.h>

#include <glib.h>
#include <glib-object.h>

#include "bogotest.h"
#include "internal.h"

void _bt_assert(int expr, const char *file, int line, const char *msg, ...)
{
    va_list ap;

    if (!expr) {
        gchar *lead, *fmsg;
        lead = g_strdup_printf("%s:%d: test failed", file, line);
        va_start(ap, msg);
        fmsg = g_strdup_vprintf(msg, ap);
        va_end(ap);
        fprintf(stderr, "%s: %s\n", lead, fmsg);
        g_free(lead);
        g_free(fmsg);
        if (_bt_fatal_failures)
            abort();
        else
            longjmp(_bt_abort_buf, 1);
    }
}

void _bt_assert_strings_equal(const char *act, const char *exp,
        const char *file, int line, const char *expr)
{
    _bt_assert(!strcmp(act, exp), file, line,
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

void _bt_assert_gvalue_is_type(GValue *val, GType type,
        const char *file, int line, const char *expr)
{
    _bt_assert(G_VALUE_TYPE(val) == type,
            file, line,
            "type of value %s is incorrect\n  Actual: %s\n  Expected: %s",
            expr, G_VALUE_TYPE_NAME(val), g_type_name(type));
}
