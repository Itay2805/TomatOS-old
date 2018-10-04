#ifndef KERNEL_CORE_DRIVER_ATA_H
#define KERNEL_CORE_DRIVER_ATA_H

#include <stdint.h>
#include <stdbool.h>
#include <kernel.h>

#define ATA_IO_BASE         0x1F0
#define ATA_DATA            (ATA_IO_BASE + 0)
#define ATA_ERROR           (ATA_IO_BASE + 1)
#define ATA_FEATURES        (ATA_IO_BASE + 1)
#define ATA_SECTOR_COUNT    (ATA_IO_BASE + 2)
#define ATA_SECTOR_NUMBER   (ATA_IO_BASE + 3)
#define ATA_CYLINDER_LOW    (ATA_IO_BASE + 4)
#define ATA_CYLINDER_HIGH   (ATA_IO_BASE + 5)
#define ATA_DRIVE_HEAD      (ATA_IO_BASE + 6)
#define ATA_STATUS          (ATA_IO_BASE + 7)
#define ATA_COMMAND         (ATA_IO_BASE + 7)

#define ATA_CONTROL_BASE        0x3F6
#define ATA_ALTERNATE_STATUS    (ATA_CONTROL_BASE + 0)
#define ATA_DEVICE_CONTROL      (ATA_CONTROL_BASE + 0)
#define ATA_DRIVE_ADDRESS       (ATA_CONTROL_BASE + 1)

#define ATA_COMMAND_NOP (0x00)
#define ATA_COMMAND_READ_SECTORS_WITH_RETRY (0x20)
#define ATA_COMMAND_READ_SECTORS_WITHOUT_RETRY (0x21)
#define ATA_COMMAND_READ_LONG_WITH_RETRY (0x22)
#define ATA_COMMAND_READ_LONG_WITHOUT_RETRY (0x23)
#define ATA_COMMAND_READ_SECTORS_EXT (0x24)
#define ATA_COMMAND_READ_DMA_EXT (0x25)
#define ATA_COMMAND_READ_DMA_QUEUED_EXT (0x26)
#define ATA_COMMAND_READ_NATIVE_MAX_ADDRESS_EXT (0x27)
#define ATA_COMMAND_READ_MULTIPLE_EXT (0x29)
#define ATA_COMMAND_READ_STREAM_DMA (0x2A)
#define ATA_COMMAND_READ_STREAM (0x2B)
#define ATA_COMMAND_READ_LOG_EXT (0x2F)
#define ATA_COMMAND_WRITE_SECTORS_WITH_RETRY (0x30)
#define ATA_COMMAND_WRITE_SECTORS_WITHOUT_RETRY (0x31)
#define ATA_COMMAND_WRITE_LONG_WITH_RETRY (0x32)
#define ATA_COMMAND_WRITE_LONG_WITHOUT_RETRY (0x33)
#define ATA_COMMAND_WRITE_SECTORS_EXT (0x34)
#define ATA_COMMAND_WRITE_DMA_EXT (0x35)
#define ATA_COMMAND_WRITE_DMA_QUEUED_EXT (0x36)
#define ATA_COMMAND_SET_NATIVE_MAX_ADDRESS_EXT (0x37)
#define ATA_COMMAND_WRITE_MULTIPLE_EXT (0x39)
#define ATA_COMMAND_WRITE_STREAM_DMA (0x3A)
#define ATA_COMMAND_WRITE_STREAM (0x3B)
#define ATA_COMMAND_WRITE_VERIFY (0x3C)
#define ATA_COMMAND_WRITE_DMA_FUA_EXT (0x3D)
#define ATA_COMMAND_WRITE_DMA_QUEUED_FUA_EXT (0x3E)
#define ATA_COMMAND_WRITE_LOG_EXT (0x3F)
#define ATA_COMMAND_READ_VERIFY_SECTORS_WITH_RETRY (0x40)
#define ATA_COMMAND_READ_VERIFY_SECTORS_WITHOUT_RETRY (0x41)
#define ATA_COMMAND_READ_VERIFY_SECTORS_EXT (0x42)
#define ATA_COMMAND_WRITE_UNCORRECTABLE_EXT (0x45)
#define ATA_COMMAND_READ_LOG_DMA_EXT (0x47)
#define ATA_COMMAND_FORMAT_TRACK (0x50)
#define ATA_COMMAND_CONFIGURE_STREAM (0x51)
#define ATA_COMMAND_WRITE_LOG_DMA (0x57)
#define ATA_COMMAND_SET_DATE_AND_TIME_EXT (0x77)
#define ATA_COMMAND_ACCESSIBLE_MAX_ADDRESS_CONFIGURATION (0x78)
#define ATA_COMMAND_EXECUTE_DEVICE_DIAGNOSTICS (0x90)
#define ATA_COMMAND_INITIALIZE_DRIVE_PARAMETER (0x90)
#define ATA_COMMAND_PACKET (0xA0)
#define ATA_COMMAND_IDENTIFY_PACKET_DEVICE (0xA1)
#define ATA_COMMAND_SERVICE (0xA2)
#define ATA_COMMAND_SMART (0xB0)
#define ATA_COMMAND_DEVICE_CONFIGURATION (0xB1)
#define ATA_COMMAND_SANITIZE_DEVICE (0xB4)
#define ATA_COMMAND_NV_CACHE (0xB6)
#define ATA_COMMAND_READ_MULTIPLE (0xC4)
#define ATA_COMMAND_WRITE_MULTIPLE (0xC5)
#define ATA_COMMAND_SET_MULTIPLE_MODE (0xC6)
#define ATA_COMMAND_READ_DMA_QUEUED (0xC7)
#define ATA_COMMAND_READ_DMA_WITH_RETRY (0xC8)
#define ATA_COMMAND_READ_DMA_WITHOUT_RETRY (0xC9)
#define ATA_COMMAND_WRITE_DMA_WITH_RETRY (0xCA)
#define ATA_COMMAND_WRITE_DMA_QUEUED (0xCC)
#define ATA_COMMAND_WRITE_MULTIPLE_FUA_EXT (0xCE)
#define ATA_COMMAND_CHECK_MEDIA_CARD_TYPE (0xD1)
#define ATA_COMMAND_GET_MEDIA_STATUS (0xDA)
#define ATA_COMMAND_BOOT_POST_BOOT (0xDC)
#define ATA_COMMAND_BOOT_PRE_BOOT (0xDD)
#define ATA_COMMAND_MEDIA_LOCK (0xDE)
#define ATA_COMMAND_MEDIA_UNLOCK (0xDF)
#define ATA_COMMAND_FLUSH_CACHE (0xEA)
#define ATA_COMMAND_WRITE_BUFFER (0xE8)
#define ATA_COMMAND_READ_BUFFER_DMA (0xE9)
#define ATA_COMMAND_FLUSH_CACHE_EXT (0xEA)
#define ATA_COMMAND_WRITE_BUFFER_DMA (0xEB)
#define ATA_COMMAND_IDENTIFY_DEVICE (0xEC)
#define ATA_COMMAND_MEDIA_EJECT (0xED)
#define ATA_COMMAND_IDENTIFY_DEVICE_DMA (0xEE)
#define ATA_COMMAND_SET_FEATURES (0xEF)
#define ATA_COMMAND_SECURITY_SET_PASSWORD (0xF1)
#define ATA_COMMAND_SECURITY_UNLOCK (0xF2)
#define ATA_COMMAND_SECURITY_EARASE_PREPARE (0xF3)
#define ATA_COMMAND_SECURITY_EARASE_UNIT (0xF4)
#define ATA_COMMAND_SECURITY_FREEZE_LOCK (0xF5)
#define ATA_COMMAND_SECURITY_DISABLE_PASSWORD (0xF6)
#define ATA_COMMAND_READ_NATIVE_MAX_ADDRESS (0xF8)
#define ATA_COMMAND_SET_MAX_ADDRESS (0xF8)

typedef enum ata_bus_kind {
    PRIMARY,
    SECONDARY
} ata_bus_kind_t;

typedef enum ata_drive_kind {
    MASTER,
    SLAVE
} ata_drive_kind_t;

typedef struct ata_error {
    uint8_t address_mark_not_found : 1;
    uint8_t track_zero_not_found : 1;
    uint8_t aborted_command : 1;
    uint8_t media_change_request : 1;
    uint8_t id_not_found : 1;
    uint8_t media_changed : 1;
    uint8_t uncorrectable_data_error  : 1;
    uint8_t bad_block_detected : 1;
} PACKED ata_error_t;

typedef struct ata_drive_head {
    // In CHS addressing, bits 0 to 3 of the head. In LBA addressing, bits 24 to 27 of the block number.
    uint8_t address : 4;
    // Selects the drive number.
    uint8_t drive_number : 1;
    // Always set.
    uint8_t always_1 : 1;
    // Uses CHS addressing if clear or LBA addressing if set.
    uint8_t lba : 1;
    // Always set.
    uint8_t always_1_ : 1;
} PACKED ata_drive_head_t;

typedef struct ata_status {
    // Indicates an error occurred. Send a new command to clear it (or nuke it with a Software Reset).
    uint8_t error : 1;
    // Index. Always set to zero.
    uint8_t index : 1;
    // Corrected data. Always set to zero.
    uint8_t corrected_data : 1;
    // Set when the drive has PIO data to transfer, or is ready to accept PIO data.
    uint8_t drive_data_ready : 1;
    // Overlapped Mode Service Request.
    uint8_t overlapped_mode_service_request : 1;
    // Drive Fault Error (does not set ERR).
    uint8_t drive_fault_error : 1;
    // Bit is clear when drive is spun down, or after an error. Set otherwise.
    uint8_t drive_ready : 1;
    // Indicates the drive is preparing to send/receive data (wait for it to clear). In case of 'hang' (it never clears), do a software reset.
    uint8_t drive_busy : 1;
} PACKED ata_status_t;

typedef struct ata_device_control {
    // Always set to zero.
    uint8_t always_0 : 1;
    // Set this to stop the current device from sending interrupts.
    uint8_t disable_interrupts : 1;
    // Set this to do a "Software Reset" on all ATA drives on a bus, if one is misbehaving.
    uint8_t software_reset : 1;
    // Reserved
    uint8_t reserved : 4;
    // Set this to read back the High Order Byte of the last LBA48 value sent to an IO port.
    uint8_t hob : 1;
} PACKED ata_device_control_t;

// this is inverted from the original value since everything is inverted in the original value
typedef struct ata_drive_address {
    uint8_t drive_0_selected : 1;
    uint8_t drive_1_selected : 1;
    uint8_t selected_head : 4;
    uint8_t write_in_progress : 1;
    uint8_t reserved : 1;
} ata_drive_address_t;

typedef struct ata_identify {
    uint16_t always_0 : 1;
    uint16_t hard_sectored : 1;
    uint16_t soft_sectored : 1;
    uint16_t not_mfm_encoded : 1;
    uint16_t low_head_switch_time : 1;
    uint16_t spindle_motor_control_option : 1;
    uint16_t fixed_drive : 1;
    uint16_t removable_cartidge_drive : 1;
    // less than 5Mbit/sec
    uint16_t disc_transfer_rate_low : 1;
    // Higher than 5Mbit/sec but less than 10Mbit/sec
    uint16_t disc_transfer_rate_med : 1;
    // Higher than 10Mbit/sec
    uint16_t disk_transfer_rate_high : 1;
    uint16_t rotational_speed_tolerance : 1;
    uint16_t data_stobe_option_available : 1;
    uint16_t track_offset_option_available : 1;
    uint16_t reserved_for_nonmagnetic_drives : 1;
    uint16_t format_Speed_tolerance_gap_required : 1;
    uint16_t number_of_cylinderss;
    uint16_t reserved_1;
    uint16_t number_of_heads;
    uint16_t number_of_unformated_bytes_per_track;
    uint16_t number_of_unformated_bytes_per_sector;
    uint16_t number_of_sectors_per_track;
    uint16_t reserved_2[3];
    char serial_number[20];
    uint16_t buffer_type;
    // 512 increments
    uint16_t buffer_size;
    uint16_t number_of_long_command_bytes;
    char firmware_revision[8];
    char model_number[40];
    uint16_t multiple_command_max : 8;
    uint16_t reserved_3 : 8;    
    uint16_t cam_perform_double_word_io;
    uint16_t reserved_5 : 8;
    uint16_t dma_supported : 1;
    uint16_t lba_supported : 1;
    uint16_t reserved_6 : 6;
    uint16_t reserved_7;
    uint16_t reserved_8 : 8;
    uint16_t pio_data_transfer_cycle_timing : 8;
    uint16_t reserved_9 : 8;
    uint16_t dma_data_transfer_cycle_timing : 8;
    uint16_t fields_54_to_58_valid : 1;
    uint16_t reserved_10 : 15;
    uint16_t number_of_current_cylinders;
    uint16_t number_of_current_heads;
    uint16_t number_of_sectors_per_track_2;
    uint32_t current_capacity_in_sectors;
    uint16_t number_of_sectors_transfered_in_multiple : 8;
    uint16_t multiple_sector_setting_is_valid : 1;
    uint16_t reserved_11 : 7;
    uint32_t user_addressable_sectors;
    uint16_t multiword_dma_words_supported : 8;
    uint16_t multiword_dma_words_active : 8;
    uint16_t singleword_dma_words_supported : 8;
    uint16_t singleword_dma_words_active : 8;
    uint16_t reserved_12[192];
} PACKED ata_identify_t;

#define ATA_IDENTIFY_BUFFER_TYPE_NOT_SPECIFIED 0x0000
#define ATA_IDENTIFY_BUFFER_TYPE_SINGLE_PORT_SINGLE_SECTOR 0x0001
#define ATA_IDENTIFY_BUFFER_TYPE_DUAL_PORTED_MULTISECTOR 0x0002
#define ATA_IDENTIFY_BUFFER_TYPE_DUAL_PORTED_MULTISECTOR_WITH_CACHING 0x0003

#ifdef __cplusplus
extern "C" {
#endif

    void ata_comand(uint8_t command);
    ata_error_t ata_error();
    ata_status_t ata_status();
    bool ata_wait();
    void ata_identify(ata_identify_t* identify);

#ifdef __cplusplus
}
#endif

#endif