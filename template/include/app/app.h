#pragma once

#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>

#include "include/domain/counter.h"
#include "include/ports/storage_port.h"

// Forward declaration: the view's internals stay private to main_view.c.
typedef struct MainView MainView;

// View ids registered with the ViewDispatcher.
typedef enum {
    AppViewMain,
} AppViewId;

// The application context — passed everywhere as `void* context`.
typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;
    MainView* main_view;

    Counter counter; // domain state
    StoragePort storage; // platform adapter (port instance)
} App;

App* app_alloc(void);
void app_free(App* app);
void app_run(App* app);
