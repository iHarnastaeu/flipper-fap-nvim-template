#pragma once

#include <stdint.h>

// Domain layer: pure business logic. NO furi.h, NO Flipper SDK includes here.
// This is what lets the domain be unit-tested on the host (see tests/).

typedef struct {
    int32_t value;
    int32_t min;
    int32_t max;
} Counter;

void counter_init(Counter* c, int32_t min, int32_t max);
void counter_increment(Counter* c);
void counter_decrement(Counter* c);
void counter_set(Counter* c, int32_t value); // clamped into [min, max]
int32_t counter_value(const Counter* c);
