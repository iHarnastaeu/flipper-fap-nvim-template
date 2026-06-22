#pragma once

#include "include/ports/storage_port.h"
#include <stdbool.h>
#include <stdint.h>

// Persistence layer: knows WHAT to persist (the counter value), but accesses
// storage only through the StoragePort abstraction. No furi.h here.

bool counter_store_load(const StoragePort* port, int32_t* out_value);
bool counter_store_save(const StoragePort* port, int32_t value);
