#include "ata.h"

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

#define ATA_DEBUG 0

#include "../port.h"

#include "../syscalls/term.h"

#include <string.h>

static uint32_t max_sectors;

uint32_t driver_ata_sector_count(void) {
	return max_sectors;
}

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

void driver_ata_init(void) {
	term_kwrite("driver_ata_init: initing ata driver.\n");

	outb(DEVICE_PORT, CURRENT_IDENTIFIER);
	outb(CONTROL_PORT, 0);
	outb(DEVICE_PORT, MASTER_IDENTIFIER);
	uint8_t status = inb(COMMAND_PORT);
	if (status == 0xFF) {
		term_kwrite("No HDD Found!\n");
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
		term_kwrite("No HDD Found!\n");
		return;
	}

	uint16_t sectcount[2];

	term_kwrite("Identifying HDD: ");
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
		term_kwrite(text);
	}
	term_kwrite("\n");

	max_sectors = *(uint32_t*)sectcount;
	char buf[11];
	itoa(max_sectors, buf, 10);
	term_kwrite("Max sector count: ");
	term_kwrite(buf);
	term_kwrite("\n");
}

void driver_ata_write(size_t sector, void* vpbuffer) {
	uintptr_t buffer = (uintptr_t)vpbuffer;

	outb(DEVICE_PORT, CURRENT | ((sector & 0x0F000000) >> 24));
	outb(ERROR_PORT, 0);
	outb(SECTOR_COUNT_PORT, 1);
	outb(LBA_LOW_PORT, sector & 0xFF);
	outb(LBA_MID_PORT, (sector & 0xFF) >> 8);
	outb(LBA_HI_PORT, (sector & 0xFF) >> 16);
	outb(COMMAND_PORT, 0x30);

#if ATA_DEBUG
	term_kwrite("driver_ata_write: waiting for disk...\n");
#endif
	if (wait_for_drive_or_error(false)) {
		term_kwrite("driver_ata_write: error");
		return;
	}
#if ATA_DEBUG
	term_kwrite("driver_ata_write: writing\n");
#endif

	for (int i = 0; i < 512; i += 2) {
		uint16_t wdata = buffer[i];
		if (i + 1 < 512) {
			wdata |= ((uint16_t)buffer[i + 1]) << 8;
		}
		outw(DATA_PORT, wdata);
	}
}

void driver_ata_read(size_t sector, void* vpbuffer) {
	uintptr_t buffer = (uintptr_t)vpbuffer;

	outb(DEVICE_PORT, CURRENT | ((sector & 0x0F000000) >> 24));
	outb(ERROR_PORT, 0);
	outb(SECTOR_COUNT_PORT, 1);
	outb(LBA_LOW_PORT, sector & 0xFF);
	outb(LBA_MID_PORT, (sector & 0xFF) >> 8);
	outb(LBA_HI_PORT, (sector & 0xFF) >> 16);
	outb(COMMAND_PORT, 0x20);

#if ATA_DEBUG
	term_kwrite("driver_ata_read: waiting for disk...\n");
#endif
	if (wait_for_drive_or_error(false)) {
		term_kwrite("driver_ata_read: error (sector: ");
		char buf[11];
		itoa(sector, buf, 10);
		term_kwrite(buf);
		term_kwrite(")\n");
		return;
	}
#if ATA_DEBUG
	term_kwrite("driver_ata_read: reading\n");
#endif

	for (int i = 0; i < 512; i += 2) {
		uint16_t wdata = inw(DATA_PORT);
		buffer[i] = wdata & 0xFF;
		if (i + 1 < 512) {
			buffer[i + 1] = (wdata >> 8) & 0xFF;
		}
	}
}

void driver_ata_flush() {
	outb(DEVICE_PORT, CURRENT);
	outb(COMMAND_PORT, 0xE7);

#if ATA_DEBUG
	term_kwrite("driver_ata_flush: waiting for disk...\n");
#endif
	if (wait_for_drive_or_error(true)) {
		term_kwrite("driver_ata_flush: error\n");
	}
#if ATA_DEBUG
	term_kwrite("driver_ata_flush: flushed!\n");
#endif

}