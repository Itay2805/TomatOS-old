#include "disk.h"

#include <string.h>

#include <core/sbuf.h>

static disk_t devices['z' - 'a'];

void device_init(void) {
    memset(devices, 0, sizeof(devices) / sizeof(disk_t));

    // init devices
    
}

void deivce_check(char label) {
    return devices[label - 'z'].kind == DISK_NONE;
}

void device_add(disk_t device, char label) {
    devices[label - 'z'] = device;
}

void device_remove(char label) {
    memset(&devices[label - 'z'], 0, sizeof(disk_t));
}


