#define PERL_NO_GET_CONTEXT      /* we want efficiency */
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"

#include <stdio.h>
#include <quickjs/quickjs.h>
#include <quickjs/quickjs-libc.h>
#include "pl_quickjs.h"
#include "pl_util.h"

/*
 * QuickJS Javascript Engine
 *
 * https://bellard.org/quickjs/quickjs.html
 */

static void set_up(QuickJS* quickjs)
{
    if (quickjs->inited) {
        return;
    }
    quickjs->runtime = JS_NewRuntime();
    quickjs->context = JS_NewContext(quickjs->runtime);

    // JS_SetModuleLoaderFunc(rt, NULL, js_module_loader, NULL);

    /* system modules */
    js_init_module_std(quickjs->context, "std");
    js_init_module_os(quickjs->context, "os");

    /* make 'std' and 'os' visible to non module code */
    const char *str =
        "import * as std from 'std';\n"
        "import * as os from 'os';\n"
        "std.global.std = std;\n"
        "std.global.os = os;\n";
    eval_buf(quickjs->context, str, strlen(str), "<input>", JS_EVAL_TYPE_MODULE);

    fprintf(stderr, "GONZO: created runtime %p, context %p\n", quickjs->runtime, quickjs->context);
    quickjs->inited = 1;
}

static void tear_down(QuickJS* quickjs)
{
    if (!quickjs->inited) {
        return;
    }
    fprintf(stderr, "GONZO: destroying runtime %p, context %p\n", quickjs->runtime, quickjs->context);
    js_std_free_handlers(quickjs->runtime);
    JS_FreeContext(quickjs->context);
    JS_FreeRuntime(quickjs->runtime);
    quickjs->inited = 0;
}

static QuickJS* create_quickjs_object(pTHX_ HV* opt)
{
    QuickJS* quickjs = (QuickJS*) malloc(sizeof(QuickJS));
    memset(quickjs, 0, sizeof(QuickJS));
    set_up(quickjs);
    return quickjs;
}

static int session_dtor(pTHX_ SV* sv, MAGIC* mg)
{
    QuickJS* quickjs = (QuickJS*) mg->mg_ptr;
    UNUSED_ARG(sv);
    tear_down(quickjs);
    return 0;
}

static MGVTBL session_magic_vtbl = { .svt_free = session_dtor };

MODULE = JavaScript::QuickJS::XS       PACKAGE = JavaScript::QuickJS::XS
PROTOTYPES: DISABLE

#################################################################

QuickJS*
new(char* CLASS, HV* opt = NULL)
  CODE:
    UNUSED_ARG(opt);
    RETVAL = create_quickjs_object(aTHX_ opt);
  OUTPUT: RETVAL

void
reset(QuickJS* quickjs)
  PPCODE:
    tear_down(quickjs);
    set_up(quickjs);

SV*
eval(QuickJS* quickjs, const char* js, const char* file = 0)
  CODE:
    RETVAL = pl_eval(aTHX_ quickjs, js, file);
  OUTPUT: RETVAL
