#pragma once

#include <stdbool.h>
#include <stdint.h>

// Port: an abstraction over persistent storage of a single int32 value.
// The persistence layer depends on this interface, NOT on furi storage
// directly. The platform layer provides a concrete implementation
// (see src/platform/storage_port.c). This is the "ports & adapters" seam.

typedef struct {
    void* context;
    bool (*load_i32)(void* context, int32_t* out_value);
    bool (*save_i32)(void* context, int32_t value);
} StoragePort;

// Concrete adapter backed by Flipper's furi storage.
StoragePort storage_port_furi(void);
