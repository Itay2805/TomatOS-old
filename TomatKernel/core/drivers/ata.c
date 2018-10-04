#include "ata.h"

// TODO: Check error?
void ata_comand(uint8_t command) {
    ata_wait();
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

void ata_identify(ata_identify_t* identify) {
    for(uint16_t* it = (uint16_t*)identify; it < identify + 1; it++) {
        *it = inw(ATA_DATA);
    }
}
