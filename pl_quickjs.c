#include <stdio.h>
#include <quickjs/quickjs.h>
#include <quickjs/quickjs-libc.h>
#include "pl_quickjs.h"

#define NEED_sv_2pv_flags
#include "ppport.h"

static int eval_buf(JSContext* ctx, const void* buf, int buf_len,
                    const char* filename, int eval_flags) {
    int ret = 0;
    fprintf(stderr, "EVAL [%*.*s]\n", buf_len, buf_len, buf);
    JSValue val = JS_Eval(ctx, buf, buf_len, filename, eval_flags);
    fprintf(stderr, "EVAL => %d\n", ret);
    if (JS_IsException(val)) {
        js_std_dump_error(ctx);
        ret = -1;
    }
    JS_FreeValue(ctx, val);
    return ret;
}

#if 0
static int eval_file(JSContext* ctx, const char* filename, int eval_flags) {
    int ret = 0;
    size_t buf_len = 0;
    uint8_t* buf = js_load_file(ctx, &buf_len, filename);
    if (!buf) {
        perror(filename);
        ret = -1;
    } else {
        ret = eval_buf(ctx, buf, buf_len, filename, eval_flags);
        js_free(ctx, buf);
    }
    return ret;
}
#endif

SV* pl_eval(pTHX_ QuickJS* quickjs, const char* js, const char* file) {
    SV* ret = &PL_sv_undef; /* return undef by default */
    int rc = 0;
    rc = eval_buf(quickjs->context, js, strlen(js), "<str>", 0);
    fprintf(stderr, "EVAL DONE => %d\n", rc);
    return ret;
}

int pl_run_gc(QuickJS* quickjs) {
    return 0;
}
