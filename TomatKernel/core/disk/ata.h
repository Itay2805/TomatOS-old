#ifndef CORE_DISK_ATA_H
#define CORE_DISK_ATA_H

#include <stdint.h>

// TODO: needs to do an overall to the disk driver
// make it more flexible like allowing multiple disks 

#ifdef __cplusplus
extern "C" {
#endif

	void disk_init(void);
	uint32_t disk_sector_count(void);

	void disk_read(void* buffer, size_t sector);
	void disk_write(void* buffer, size_t sector);
	void disk_flush(void);

#ifdef __cplusplus
}
#endif

#endif