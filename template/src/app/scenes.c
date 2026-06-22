#include "include/app/scenes.h"

// Handler tables: indexed by the AppScene enum. To add a scene, add an enum
// entry in scenes.h and a row to each of the three tables below.

static void (*const app_scene_on_enter_handlers[])(void*) = {
    scene_main_on_enter,
};

static bool (*const app_scene_on_event_handlers[])(void*, SceneManagerEvent) = {
    scene_main_on_event,
};

static void (*const app_scene_on_exit_handlers[])(void*) = {
    scene_main_on_exit,
};

const SceneManagerHandlers app_scene_handlers = {
    .on_enter_handlers = app_scene_on_enter_handlers,
    .on_event_handlers = app_scene_on_event_handlers,
    .on_exit_handlers = app_scene_on_exit_handlers,
    .scene_num = SceneCount,
};
