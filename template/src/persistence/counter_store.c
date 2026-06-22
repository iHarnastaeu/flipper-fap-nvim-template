#include "include/persistence/counter_store.h"

bool counter_store_load(const StoragePort* port, int32_t* out_value) {
    if(!port || !port->load_i32) return false;
    return port->load_i32(port->context, out_value);
}

bool counter_store_save(const StoragePort* port, int32_t value) {
    if(!port || !port->save_i32) return false;
    return port->save_i32(port->context, value);
}
