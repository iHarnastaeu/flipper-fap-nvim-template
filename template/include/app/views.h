#pragma once

#include <gui/view.h>
#include <stdint.h>

// The custom view module for the main screen. Its struct is private to
// main_view.c; the rest of the app talks to it only through this interface.

typedef struct MainView MainView;

// Custom events the view emits up to the scene (via the ViewDispatcher).
typedef enum {
    MainViewEventIncrement = 100,
    MainViewEventDecrement,
} MainViewEvent;

typedef void (*MainViewCallback)(void* context, MainViewEvent event);

MainView* main_view_alloc(void);
void main_view_free(MainView* view);
View* main_view_get_view(MainView* view);
void main_view_set_count(MainView* view, int32_t count);
void main_view_set_callback(MainView* view, MainViewCallback callback, void* context);
