#include "include/domain/counter.h"

void counter_init(Counter* c, int32_t min, int32_t max) {
    c->min = min;
    c->max = max;
    c->value = min;
}

void counter_set(Counter* c, int32_t value) {
    if(value < c->min) value = c->min;
    if(value > c->max) value = c->max;
    c->value = value;
}

void counter_increment(Counter* c) {
    if(c->value < c->max) c->value++;
}

void counter_decrement(Counter* c) {
    if(c->value > c->min) c->value--;
}

int32_t counter_value(const Counter* c) {
    return c->value;
}
