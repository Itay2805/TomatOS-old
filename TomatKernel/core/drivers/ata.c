#include "ata.h"

// TODO: Check error?
void ata_comand(uint8_t command, uint8_t sectorCount, uint32_t sector) {
    outb(ATA_SECTOR_COUNT, sectorCount);
    outb(ATA_SECTOR_NUMBER, sector & 0xFF);
    outb(ATA_CYLINDER_LOW, (sector & 0xFF) >> 8);
    outb(ATA_CYLINDER_HIGH, (sector & 0xFF) >> 16);
    outb(ATA_COMMAND, command);
    ata_wait();
}

ata_error_t ata_error() {
    uint8_t error = inb(ATA_ERROR);
    return *(ata_error_t*)&error;
}

ata_status_t ata_status() {
    uint8_t error = inb(ATA_STATUS);
    return *(ata_status_t*)&error;
}

bool ata_wait() {
    ata_status_t status;
    do {
        status = ata_status();
        if(status.error || status.drive_fault_error) {
            return false;
        }
    } while(!status.drive_ready);
    return true;
}

void ata_bus(ata_bus_kind_t kind) {
    outb(ATA_DRIVE_HEAD, (uint8_t)kind);
}

void ata_drive(ata_drive_kind_t kind) {
    outb(ATA_DEVICE_CONTROL, (uint8_t)kind);    
}

bool ata_connected() {
    ata_status_t status = ata_status();
    return *(uint8_t*)&status != 0xFF && *(uint8_t*)&status != 0x00;
}

void ata_identify(ata_identify_t* identify) {
    ata_comand(ATA_COMMAND_IDENTIFY_DEVICE, 0, 0);
    for(uint16_t* it = (uint16_t*)identify; it < (uint16_t*)((uintptr_t)identify + sizeof(ata_identify_t)); it++) {
        *it = inw(ATA_DATA);
    }
}

