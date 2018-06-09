#ifndef KERNEL_DRIVER_DISK_H
#define KERNEL_DRIVER_DISK_H

#include <stdint.h>

void driver_disk_init(void);
void driver_disk_write(uint64_t address, void* buffer, size_t size);
void driver_disk_read(uint64_t address, void* buffer, size_t size);
uint64_t driver_disk_size(void);

#endif