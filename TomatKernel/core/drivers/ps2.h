#ifndef KERNEL_CORE_DRIVER_PS2_H
#define KERNEL_CORE_DRIVER_PS2_H

#include <stdint.h>

#include <kernel.h>

#define PS2_DATA    ((uint8_t)0x60)
#define PS2_COMMAND ((uint8_t)0x64)

#define PS2_ACK     ((uint8_t)0xFA)

typedef struct ps2_status {
    // Output buffer status (0 = empty, 1 = full)
    uint8_t output_buffer_status : 1;
    // Input buffer status (0 = empty, 1 = full)
    uint8_t input_buffer_status : 1;
    // Meant to be cleared on reset and set by firmware (via. PS/2 Controller Configuration Byte) if the system passes self tests (POST)
    uint8_t system_flag : 1;
    // Command/data (0 = data written to input buffer is data for PS/2 device, 1 = data written to input buffer is data for PS/2 controller command)
    uint8_t command_or_data : 1;
    uint8_t unknown_1 : 1;
    uint8_t unknown_2 : 1;
    // Time-out error (0 = no error, 1 = time-out error)
    uint8_t timeout_error : 1;
    // Parity error (0 = no error, 1 = parity error)
    uint8_t parity_error : 1;    
} PACKED ps2_status_t;

#ifdef __cplusplus
extern "C" {
#endif

    ps2_status_t ps2_status(void);

    void ps2_command(uint8_t command);
    void ps2_write(uint8_t value);
    uint8_t ps2_read(void);

#ifdef __cplusplus
}
#endif
#endif
