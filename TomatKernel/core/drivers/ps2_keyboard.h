#ifndef KERNEL_CORE_DRIVER_PS2_KEYBOARD_H
#define KERNEL_CORE_DRIVER_PS2_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#define KEYS_RIGHT_SHIFT    0xA0
#define KEYS_LEFT_SHIFT     0xA1

    void ps2_keyboard_init(void);

#ifdef __cplusplus
}
#endif
#endif
