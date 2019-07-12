#define PERL_NO_GET_CONTEXT      /* we want efficiency */
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"

#include <stdio.h>
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

static QuickJS* create_quickjstape_object(pTHX_ HV* opt)
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
    RETVAL = create_quickjstape_object(aTHX_ opt);
  OUTPUT: RETVAL

void
reset(QuickJS* quickjs)
  PPCODE:
    tear_down(quickjs);
    set_up(quickjs);
