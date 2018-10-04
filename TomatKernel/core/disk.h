#ifndef KERNEL_CORE_DISK_H
#define KERNEL_CORE_DISK_H

#include <stdint.h>
#include <stdbool.h>

typedef enum disk_kind {
    DISK_NONE,
    DISK_ATA,
} disk_kind_t;

typedef struct disk {
    char label;
    bool available;
    disk_kind_t kind;
    void* driver_data;
} disk_t;

#ifdef __cplusplus
extern "C" {
#endif

    void disk_init(void);
    bool disk_check(char label);
    void disk_add(disk_t device, char label);
    void disk_remove(char label);

    void disk_read();

#ifdef __cplusplus
}
#endif

#endif