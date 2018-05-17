#ifndef KERNEL_DRIVER_ATA_H
#define KERNEL_DRIVER_ATA_H

#include <stdint.h>

void driver_ata_init(void);
void driver_ata_write(size_t sector, uintptr_t buffer, int count);
void driver_ata_read(size_t sector, uintptr_t buffer, int count);
void driver_ata_flush();

#endif