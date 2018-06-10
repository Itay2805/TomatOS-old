#include "disk.h"
#include "ata.h"

#include <string.h>

#define SECTOR(a) ((a) / (uint64_t)512)
#define OFFSET(a) ((a) % (uint64_t)512)

void driver_disk_init(void) {
	driver_ata_init();
}

void driver_disk_write(uint64_t address, void* vpbuffer, size_t size) {
	int sector = SECTOR(address);
	int offset = OFFSET(address);
	int sector_count = (offset + size) / 512 + 1;
	uintptr_t buffer = (uintptr_t)vpbuffer;
	uint8_t tempbuffer[512];
	
	driver_ata_read(sector, tempbuffer);
	if (sector_count == 1) {
		
		// write the sector taking into account both offset and size
		memcpy(tempbuffer + offset, buffer, size);
		driver_ata_write(sector, tempbuffer);
		driver_ata_flush();

		return;
	}
	else {
		// write to first sector
		memcpy(tempbuffer + offset, buffer, 512 - offset);
		buffer += 512 - offset;
		driver_ata_write(sector, tempbuffer);

	}

	sector++;

	// write to all middle sectors
	for (int i = 1; i < sector_count - 1; i++, sector++) {
		driver_ata_read(sector, tempbuffer);

		memcpy(tempbuffer, buffer, 512);
		buffer += 512;

		driver_ata_write(sector, tempbuffer);
	}

	// write to last sector
	driver_ata_read(sector, tempbuffer);
	memcpy(tempbuffer, buffer, size % 512);
	driver_ata_write(sector, tempbuffer);

	driver_ata_flush();
}

void driver_disk_read(uint64_t address, void* vpbuffer, size_t size) {
	int sector = SECTOR(address);
	int offset = OFFSET(address);
	int sector_count = (offset + size) / 512 + 1;
	uintptr_t buffer = (uintptr_t)vpbuffer;
	uint8_t tempbuffer[512];

	driver_ata_read(sector, tempbuffer);
	if (sector_count == 1) {
		// read the sector taking into account both offset and size
		memcpy(buffer, tempbuffer + offset, size);
		return;
	}
	else {
		// read the first sector, taking into account offset
		memcpy(buffer, tempbuffer + offset, 512 - offset);
		buffer += 512 - offset;
	}

	sector++;

	// read to all middle sectors
	for (int i = 1; i < sector_count - 1; i++, sector++) {
		driver_ata_read(sector, tempbuffer);

		memcpy(buffer, tempbuffer, 512);
		buffer += 512;
	}

	// read last sector, taking into account size
	driver_ata_read(sector, tempbuffer);
	memcpy(buffer, tempbuffer, size % 512);
}

uint64_t driver_disk_size(void) {
	return ((uint64_t)driver_ata_sector_count()) * 512;
}
