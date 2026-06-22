#include "include/app/app.h"
#include <furi.h>

// Entry point declared in application.fam. Keep this minimal: the whole
// application lives under src/, wired together by app_alloc/app_run/app_free.
int32_t __APPID___app(void* p) {
    UNUSED(p);
    App* app = app_alloc();
    app_run(app);
    app_free(app);
    return 0;
}
