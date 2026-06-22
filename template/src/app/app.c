#include "include/app/app.h"
#include "include/app/scenes.h"
#include "include/app/views.h"
#include "include/domain/counter.h"
#include "include/ports/storage_port.h"
#include "include/persistence/counter_store.h"

#include <furi.h>

// Bridges the custom view's events into the ViewDispatcher's custom-event
// pipeline, which the SceneManager then routes to the active scene.
static void app_view_callback(void* context, MainViewEvent event) {
    App* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, event);
}

static bool app_custom_event_callback(void* context, uint32_t event) {
    App* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool app_back_event_callback(void* context) {
    App* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

App* app_alloc(void) {
    App* app = malloc(sizeof(App));

    app->gui = furi_record_open(RECORD_GUI);
    app->view_dispatcher = view_dispatcher_alloc();
    app->scene_manager = scene_manager_alloc(&app_scene_handlers, app);

    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, app_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, app_back_event_callback);
    view_dispatcher_attach_to_gui(
        app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    app->main_view = main_view_alloc();
    main_view_set_callback(app->main_view, app_view_callback, app);
    view_dispatcher_add_view(
        app->view_dispatcher, AppViewMain, main_view_get_view(app->main_view));

    // Domain + persistence: init the counter, then restore any saved value.
    counter_init(&app->counter, 0, 99);
    app->storage = storage_port_furi();
    int32_t saved = 0;
    if(counter_store_load(&app->storage, &saved)) {
        counter_set(&app->counter, saved);
    }

    return app;
}

void app_free(App* app) {
    view_dispatcher_remove_view(app->view_dispatcher, AppViewMain);
    main_view_free(app->main_view);
    scene_manager_free(app->scene_manager);
    view_dispatcher_free(app->view_dispatcher);
    furi_record_close(RECORD_GUI);
    free(app);
}

void app_run(App* app) {
    scene_manager_next_scene(app->scene_manager, SceneMain);
    view_dispatcher_run(app->view_dispatcher);
}
