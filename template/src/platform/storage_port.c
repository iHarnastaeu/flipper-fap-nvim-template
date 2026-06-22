#include "include/ports/storage_port.h"

#include <furi.h>
#include <storage/storage.h>

// Platform layer: the concrete adapter that implements the storage port
// using Flipper's furi storage API. This is the ONLY place that knows about
// furi storage; everything above it talks to the StoragePort interface.

#define STORE_PATH APP_DATA_PATH("state.bin")

static bool plat_load_i32(void* context, int32_t* out_value) {
    UNUSED(context);
    Storage* storage = furi_record_open(RECORD_STORAGE);
    File* file = storage_file_alloc(storage);
    bool ok = false;
    if(storage_file_open(file, STORE_PATH, FSAM_READ, FSOM_OPEN_EXISTING)) {
        ok = storage_file_read(file, out_value, sizeof(int32_t)) == sizeof(int32_t);
    }
    storage_file_close(file);
    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);
    return ok;
}

static bool plat_save_i32(void* context, int32_t value) {
    UNUSED(context);
    Storage* storage = furi_record_open(RECORD_STORAGE);
    storage_common_mkdir(storage, APP_DATA_PATH(""));
    File* file = storage_file_alloc(storage);
    bool ok = false;
    if(storage_file_open(file, STORE_PATH, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        ok = storage_file_write(file, &value, sizeof(int32_t)) == sizeof(int32_t);
    }
    storage_file_close(file);
    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);
    return ok;
}

StoragePort storage_port_furi(void) {
    return (StoragePort){
        .context = NULL,
        .load_i32 = plat_load_i32,
        .save_i32 = plat_save_i32,
    };
}
