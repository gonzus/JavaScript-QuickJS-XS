#include "pl_quickjs.h"

#define NEED_sv_2pv_flags
#include "ppport.h"

int pl_run_gc(QuickJS* quickjs)
{
    return 0;
}
