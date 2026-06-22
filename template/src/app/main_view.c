#include "include/app/views.h"

#include <furi.h>
#include <gui/view.h>
#include <input/input.h>
#include <stdio.h>

// Private view struct + model. Nothing outside this file sees these.
struct MainView {
    View* view;
    MainViewCallback callback;
    void* callback_context;
};

typedef struct {
    int32_t count;
} MainViewModel;

static void main_view_draw(Canvas* canvas, void* model) {
    MainViewModel* m = model;
    canvas_clear(canvas);

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 14, "Hello world");

    canvas_set_font(canvas, FontSecondary);
    char buf[24];
    snprintf(buf, sizeof(buf), "Count: %ld", (long)m->count);
    canvas_draw_str(canvas, 2, 30, buf);
    canvas_draw_str(canvas, 2, 44, "Up / Down: change");
    canvas_draw_str(canvas, 2, 56, "Back: exit");
}

static bool main_view_input(InputEvent* event, void* context) {
    MainView* view = context;
    if(event->type != InputTypeShort) return false;

    if(event->key == InputKeyUp) {
        if(view->callback) view->callback(view->callback_context, MainViewEventIncrement);
        return true;
    }
    if(event->key == InputKeyDown) {
        if(view->callback) view->callback(view->callback_context, MainViewEventDecrement);
        return true;
    }

    // Return false for Back (and anything else) so the ViewDispatcher's
    // navigation handler runs and the SceneManager can pop/exit.
    return false;
}

MainView* main_view_alloc(void) {
    MainView* view = malloc(sizeof(MainView));
    view->callback = NULL;
    view->callback_context = NULL;

    view->view = view_alloc();
    view_set_context(view->view, view);
    view_allocate_model(view->view, ViewModelTypeLocking, sizeof(MainViewModel));
    view_set_draw_callback(view->view, main_view_draw);
    view_set_input_callback(view->view, main_view_input);

    return view;
}

void main_view_free(MainView* view) {
    view_free(view->view);
    free(view);
}

View* main_view_get_view(MainView* view) {
    return view->view;
}

void main_view_set_count(MainView* view, int32_t count) {
    with_view_model(
        view->view, MainViewModel * model, { model->count = count; }, true);
}

void main_view_set_callback(MainView* view, MainViewCallback callback, void* context) {
    view->callback = callback;
    view->callback_context = context;
}
