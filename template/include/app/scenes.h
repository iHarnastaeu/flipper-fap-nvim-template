#pragma once

#include <gui/scene_manager.h>

// One enum entry per scene. SceneCount must stay last — it sizes the
// handler tables in scenes.c.
typedef enum {
    SceneMain,
    SceneCount,
} AppScene;

extern const SceneManagerHandlers app_scene_handlers;

// Main scene handlers (one trio per scene; add more as you add scenes).
void scene_main_on_enter(void* context);
bool scene_main_on_event(void* context, SceneManagerEvent event);
void scene_main_on_exit(void* context);
