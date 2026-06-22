#include "include/app/scenes.h"
#include "include/app/app.h"
#include "include/app/views.h"
#include "include/domain/counter.h"
#include "include/persistence/counter_store.h"

#include <furi.h>

void scene_main_on_enter(void* context) {
    App* app = context;
    main_view_set_count(app->main_view, counter_value(&app->counter));
    view_dispatcher_switch_to_view(app->view_dispatcher, AppViewMain);
}

bool scene_main_on_event(void* context, SceneManagerEvent event) {
    App* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case MainViewEventIncrement:
            counter_increment(&app->counter);
            consumed = true;
            break;
        case MainViewEventDecrement:
            counter_decrement(&app->counter);
            consumed = true;
            break;
        default:
            break;
        }

        if(consumed) {
            int32_t value = counter_value(&app->counter);
            main_view_set_count(app->main_view, value);
            counter_store_save(&app->storage, value);
        }
    }

    return consumed;
}

void scene_main_on_exit(void* context) {
    UNUSED(context);
}
