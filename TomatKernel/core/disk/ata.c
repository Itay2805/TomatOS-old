#include "ata.h"

#include <stdbool.h>

#include <string.h>

#include <core/graphics/term.h>
#include <kernel.h>

#define MASTER	0xE0 
#define SLAVE	0xF0

#define MASTER_IDENTIFIER	0xA0
#define SLAVE_IDENTIFIER	0xB0

#define CURRENT_IDENTIFIER	MASTER_IDENTIFIER
#define CURRENT				MASTER

#define PRIMARY_ATA_PORT_BASE	0x1F0
#define DATA_PORT				PRIMARY_ATA_PORT_BASE
#define ERROR_PORT				PRIMARY_ATA_PORT_BASE + 1
#define SECTOR_COUNT_PORT		PRIMARY_ATA_PORT_BASE + 2
#define LBA_LOW_PORT			PRIMARY_ATA_PORT_BASE + 3
#define LBA_MID_PORT			PRIMARY_ATA_PORT_BASE + 4
#define LBA_HI_PORT				PRIMARY_ATA_PORT_BASE + 5
#define DEVICE_PORT				PRIMARY_ATA_PORT_BASE + 6
#define COMMAND_PORT			PRIMARY_ATA_PORT_BASE + 7
#define CONTROL_PORT			PRIMARY_ATA_PORT_BASE + 518

static uint32_t max_sectors;

static bool wait_for_drive_or_error(bool flush) {
	while (true) {
		uint8_t status = inb(COMMAND_PORT);
		if (status & 1 || status & (1 << 5)) {
			return true;
		}
		if (flush) {
			if (!(status & (1 << 7))) {
				return false;
			}
		}
		else {
			if (!(status & (1 << 7)) && status & (1 << 3)) {
				return false;
			}
		}
	}
}

void disk_init(void) {
	term_write("[disk/ata] Initializing...\n");

	outb(DEVICE_PORT, CURRENT_IDENTIFIER);
	outb(CONTROL_PORT, 0);
	outb(DEVICE_PORT, MASTER_IDENTIFIER);
	uint8_t status = inb(COMMAND_PORT);
	if (status == 0xFF) {
		kpanic("[disk/ata] No HDD Found!\n");
		return;
	}

	outb(DEVICE_PORT, CURRENT_IDENTIFIER);
	outb(SECTOR_COUNT_PORT, 0);
	outb(LBA_LOW_PORT, 0);
	outb(LBA_MID_PORT, 0);
	outb(LBA_HI_PORT, 0);
	outb(COMMAND_PORT, 0xEC);

	status = inb(COMMAND_PORT);

	if (wait_for_drive_or_error(false)) {
		kpanic("[disk/ata] No HDD Found!\n");
		return;
	}

	uint16_t sectcount[2];

	term_write("[disk/ata] \tIdentifying HDD: ");
	for (int i = 0; i < 256; i++) {
		uint16_t data = inw(DATA_PORT);
		if (i == 60) {
			sectcount[0] = data;
		}
		else if (i == 61) {
			sectcount[1] = data;
		}
		char text[3];
		text[2] = 0;
		text[0] = (data >> 8) & 0xFF;
		text[1] = data & 0xFF;
		term_write(text);
	}
	term_write("\n");

	max_sectors = *(uint32_t*)sectcount;
	char buf[11];
	itoa(max_sectors, buf, 10);
	term_write("[disk/ata] \tMax sector count: ");
	term_write(buf);
	term_write("\n");

	term_write("[disk/ata] Done!");
}

uint32_t disk_sector_count(void) {
	return max_sectors;
}

void disk_write(void* vpbuffer, size_t sector) {
	uintptr_t buffer = (uintptr_t)vpbuffer;

	outb(DEVICE_PORT, CURRENT | ((sector & 0x0F000000) >> 24));
	outb(ERROR_PORT, 0);
	outb(SECTOR_COUNT_PORT, 1);
	outb(LBA_LOW_PORT, sector & 0xFF);
	outb(LBA_MID_PORT, (sector & 0xFF) >> 8);
	outb(LBA_HI_PORT, (sector & 0xFF) >> 16);
	outb(COMMAND_PORT, 0x30);
	
	if (wait_for_drive_or_error(false)) {
		kpanic("[disk/ata] disk_write: error");
		return;
	}

	for (int i = 0; i < 512; i += 2) {
		uint16_t wdata = buffer[i];
		if (i + 1 < 512) {
			wdata |= ((uint16_t)buffer[i + 1]) << 8;
		}
		outw(DATA_PORT, wdata);
	}
}

void disk_read(void* vpbuffer, size_t sector) {
	uintptr_t buffer = (uintptr_t)vpbuffer;

	outb(DEVICE_PORT, CURRENT | ((sector & 0x0F000000) >> 24));
	outb(ERROR_PORT, 0);
	outb(SECTOR_COUNT_PORT, 1);
	outb(LBA_LOW_PORT, sector & 0xFF);
	outb(LBA_MID_PORT, (sector & 0xFF) >> 8);
	outb(LBA_HI_PORT, (sector & 0xFF) >> 16);
	outb(COMMAND_PORT, 0x20);

	if (wait_for_drive_or_error(false)) {
		term_write("[disk/ata] (sector: ");
		char buf[11];
		itoa(sector, buf, 10);
		term_write(buf);
		term_write(")\n");
		kpanic("[disk/ata] disk_read: error");
		return;
	}

	for (int i = 0; i < 512; i += 2) {
		uint16_t wdata = inw(DATA_PORT);
		buffer[i] = wdata & 0xFF;
		if (i + 1 < 512) {
			buffer[i + 1] = (wdata >> 8) & 0xFF;
		}
	}
}

void disk_flush(void) {
	outb(DEVICE_PORT, CURRENT);
	outb(COMMAND_PORT, 0xE7);
	if (wait_for_drive_or_error(true)) {
		kpanic("[disk/ata] disk_flush: error\n");
	}
}
