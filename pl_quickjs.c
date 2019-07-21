#include <stdio.h>
#include <quickjs/quickjs.h>
#include <quickjs/quickjs-libc.h>
#include "pl_quickjs.h"

#define NEED_sv_2pv_flags
#include "ppport.h"

static void show(JSContext* ctx, JSValue val) {
    uint32_t tag = JS_VALUE_GET_NORM_TAG(val);
    switch (tag) {
        case JS_TAG_NULL:
            fprintf(stderr, "TAG null\n");
            break;
        case JS_TAG_UNDEFINED:
            fprintf(stderr, "TAG undefined\n");
            break;
        case JS_TAG_UNINITIALIZED:
            fprintf(stderr, "TAG uninitialized\n");
            break;

        case JS_TAG_INT: {
            int cval = 0;
            int ret = JS_ToInt32(ctx, &cval, val);
            fprintf(stderr, "TAG int => %d (%d)\n", cval, ret);
            break;
        }
        case JS_TAG_BIG_INT: {
            int64_t cval = 0;
            int ret = JS_ToInt64(ctx, &cval, val);
            fprintf(stderr, "TAG big_int => %lld (%d)\n", cval, ret);
            break;
        }
        case JS_TAG_BIG_FLOAT: {
            double cval = 0;
            int ret = JS_ToFloat64(ctx, &cval, val);
            fprintf(stderr, "TAG big_float => %f (%d)\n", cval, ret);
            break;
        }
        case JS_TAG_FLOAT64: {
            fprintf(stderr, "TAG float64 => %f\n", JS_VALUE_GET_FLOAT64(val));
            break;
        }

        case JS_TAG_BOOL: {
            int cval = JS_ToBool(ctx, val); /* return -1 for JS_EXCEPTION */
            fprintf(stderr, "TAG bool => [%s]\n", cval ? "true" : "false");
            break;
        }

        case JS_TAG_STRING: {
            int clen = 0;
            const char* cstr = JS_ToCStringLen(ctx, &clen, val, 0);
            fprintf(stderr, "TAG string [%d:%s]\n", clen, cstr);
            break;
        }
        case JS_TAG_SYMBOL: // TODO
            fprintf(stderr, "TAG symbol\n");
            break;

        case JS_TAG_OBJECT: {
            // could be JS_CLASS_ERROR
            // Maybe we just need to eval Object.getOwnPropertyNames(val) to get all the properties?
            // An array always has a property called 'length'
            // https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/getOwnPropertyNames
            if (JS_IsArray(ctx, val)) {
                // TODO: how to get all elements of array? add one element? delete one?
                JSValue jlen = JS_GetPropertyStr(ctx, val, "length");
                int clen = 0;
                int ret = JS_ToInt32(ctx, &clen, jlen);
                fprintf(stderr, "TAG object => array [%d] (%d)\n", clen, ret);
            } else {
                fprintf(stderr, "TAG object\n");
            }
            break;
        }

        case JS_TAG_SHAPE: // ???
            fprintf(stderr, "TAG shape\n");
            break;
        case JS_TAG_ASYNC_FUNCTION: // ???
            fprintf(stderr, "TAG async_function\n");
            break;
        case JS_TAG_VAR_REF: // ???
            fprintf(stderr, "TAG var_ref\n");
            break;
        case JS_TAG_MODULE: // ???
            fprintf(stderr, "TAG module\n");
            break;
        case JS_TAG_FUNCTION_BYTECODE: // ???
            fprintf(stderr, "TAG function_bytecode\n");
            break;
        case JS_TAG_CATCH_OFFSET: // ???
            fprintf(stderr, "TAG catch_offset\n");
            break;
        case JS_TAG_EXCEPTION: // ???
            fprintf(stderr, "TAG exception\n");
            break;
        default:
            fprintf(stderr, "TAG WTF?\n");
            break;
    }
}

int eval_buf(JSContext* ctx, const void* buf, int buf_len,
             const char* filename, int eval_flags) {
    int ret = 0;
    JSValue val = JS_Eval(ctx, buf, buf_len, filename, eval_flags);
    show(ctx, val);
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
    // TODO: is this necessary?
    // js_std_loop(quickjs->context);
    return ret;
}

int pl_run_gc(QuickJS* quickjs) {
    return 0;
}
