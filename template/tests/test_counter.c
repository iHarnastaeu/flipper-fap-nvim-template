// Host-side unit test for the pure domain layer. Compiled with the system
// compiler (see Makefile) — no Flipper SDK, no device required.
#include "include/domain/counter.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    Counter c;
    counter_init(&c, 0, 3);
    assert(counter_value(&c) == 0);

    counter_decrement(&c); // clamped at min
    assert(counter_value(&c) == 0);

    counter_increment(&c);
    counter_increment(&c);
    assert(counter_value(&c) == 2);

    counter_increment(&c);
    counter_increment(&c); // clamped at max
    assert(counter_value(&c) == 3);

    counter_set(&c, 99); // clamped at max
    assert(counter_value(&c) == 3);

    counter_set(&c, -5); // clamped at min
    assert(counter_value(&c) == 0);

    printf("All counter domain tests passed.\n");
    return 0;
}
