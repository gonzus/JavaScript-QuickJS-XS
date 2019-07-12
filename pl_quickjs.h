#ifndef PL_QUICKJS_H
#define PL_QUICKJS_H

#include <quickjs/quickjs.h>

#include "EXTERN.h"
#include "perl.h"
#include "ppport.h"

/*
 * This is our internal data structure.
 */
typedef struct QuickJS {
    int inited;
    JSRuntime* runtime;
    JSContext* context;
} QuickJS;

/* Run the QuickJS GC */
int pl_run_gc(QuickJS* duk);

#endif
