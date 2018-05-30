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

#include "../port.h"

#include "../syscalls/term.h"

void driver_ata_init(void) {
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

	if (status == 0) {
		term_kwrite("No HDD Found!\n");
		return;
	}

	do {
		status = inb(COMMAND_PORT);
	} while (((status & 0x80) == 0x80) && (status & 0x1) != 0x01);

	if (status & 0x1) {
		term_kwrite("driver_ata_init: exception\n");
		return;
	}

	term_kwrite("Identifying HDD: ");
	for (int i = 0; i < 256; i++) {
		uint16_t data = inw(DATA_PORT);
		char text[3];
		text[2] = 0;
		text[0] = (data >> 8) & 0xFF;
		text[1] = data & 0xFF;
		term_kwrite(text);
	}
	term_kwrite("\n");
}

void driver_ata_write(size_t sector, uintptr_t data, int count) {
	outb(DEVICE_PORT, CURRENT | ((sector & 0x0F000000) >> 24));
	outb(ERROR_PORT, 0);
	outb(SECTOR_COUNT_PORT, 1);
	outb(LBA_LOW_PORT, sector & 0xFF);
	outb(LBA_MID_PORT, (sector & 0xFF) >> 8);
	outb(LBA_HI_PORT, (sector & 0xFF) >> 16);
	outb(COMMAND_PORT, 0x30);

	for (int i = 0; i < count; i += 2) {
		uint16_t wdata = data[i];
		if (i + 1 < count) {
			wdata |= ((uint16_t)data[i + 1]) << 8;
		}
		outw(DATA_PORT, wdata);
	}

	for (int i = count + (count % 2); i < 512; i += 2) {
		outw(DATA_PORT, 0);
	}
}

void driver_ata_read(size_t sector, uintptr_t buffer, int count) {
	outb(DEVICE_PORT, CURRENT | ((sector & 0x0F000000) >> 24));
	outb(ERROR_PORT, 0);
	outb(SECTOR_COUNT_PORT, 1);
	outb(LBA_LOW_PORT, sector & 0xFF);
	outb(LBA_MID_PORT, (sector & 0xFF) >> 8);
	outb(LBA_HI_PORT, (sector & 0xFF) >> 16);
	outb(COMMAND_PORT, 0x20);

	uint8_t status;
	do {
		status = inb(COMMAND_PORT);
	} while (((status & 0x80) == 0x80) && (status & 0x1) != 0x01);

	if (status & 0x1) {
		term_kwrite("driver_ata_read: exception\n");
		return;
	}

	for (int i = 0; i < count; i += 2) {
		uint16_t wdata = inw(DATA_PORT);
		buffer[i] = wdata & 0xFF;
		if (i + 1 < count) {
			buffer[i + 1] = (wdata >> 8) & 0xFF;
		}
	}

	for (int i = count + (count % 2); i < 512; i +=2 ) {
		inw(DATA_PORT);
	}
}

void driver_ata_flush() {
	outb(DEVICE_PORT, CURRENT);
	outb(COMMAND_PORT, 0xE7);

	uint8_t status = inb(COMMAND_PORT);

	if (status == 0) {
		term_kwrite("driver_ata_flush: exception - first\n");
	}

	do {
		status = inb(COMMAND_PORT);
	} while (((status & 0x80) == 0x80) && (status & 0x1) != 0x01);
	
	if (status & 0x1) {
		// add exception
		term_kwrite("driver_ata_flush: exception - second\n");
		return;
	}
}