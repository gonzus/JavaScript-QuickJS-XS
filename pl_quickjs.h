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

int eval_buf(JSContext* ctx, const void* buf, int buf_len,
             const char* filename, int eval_flags);

SV* pl_eval(pTHX_ QuickJS* quickjs, const char* js, const char* file);
int pl_run_gc(QuickJS* duk);

#endif
