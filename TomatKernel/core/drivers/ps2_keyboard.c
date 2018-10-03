#include "ps2_keyboard.h"

#include <kernel.h>

#include <string.h>
#include <stdbool.h>

#include <boot/interrupts/pic.h>
#include <boot/interrupts/isr.h>

#include <core/process/process.h>

#include "ps2.h"

#define SCANCODE_MAX 0x80

// keycode mapping
static uint8_t scs_mapping[SCANCODE_MAX];
static uint8_t shift_mapping[SCANCODE_MAX];

static bool key_states[1024];

static bool is_shift_pressed() {
    return key_states[KEYS_LEFT_SHIFT] || key_states[KEYS_RIGHT_SHIFT];
}

// keyboard events are passed to the core process (which happen to be a foreground process)
// it is the core process's job to pass the events to the right process afterwards
static void key_release(uint16_t keycode) {
    process_t* process = process_get_core_process();
    if(process == NULL) {
        kpanic("[TomatKernel/key_press] No core process found!\n");
    }

    // push key_up event
    event_t event;
    event.type = EVENT_KEY_UP;
    event.data[0] = keycode;
    event.data[1] = 0;
    event.data[2] = 0;
    event.data[3] = 0;    
    buf_push(process->events, event);


    key_states[keycode] = false;
}

static void key_press(uint16_t keycode) {
    process_t* process = process_get_core_process();
    if(process == NULL) {
        kpanic("[TomatKernel/key_press] No core process found!\n");
    }

    // push key event
    event_t event;
    event.type = EVENT_KEY;
    event.data[0] = keycode;
    event.data[1] = key_states[keycode];
    event.data[2] = 0;
    event.data[3] = 0;
    buf_push(process->events, event);

    key_states[keycode] = true;

    // because any printable character has shift-mapping this way we can create CHAR events 
    if(shift_mapping[keycode] != 0) {
        // transform if shift is pressed
        if(is_shift_pressed()) {
            keycode = shift_mapping[keycode];
        }
        
        // push char event
        event.type = EVENT_CHAR;
        event.data[0] = keycode;
        event.data[1] = 0;
        event.data[2] = 0;
        event.data[3] = 0;
        buf_push(process->events, event);
    }
}

static void handle_keyboard_irq(registers_t* regs) {
    UNUSED(regs);

    // transform the scancode
    uint8_t scancode = ps2_read();
    
    if(scancode == 0xE0) {
        scancode = ps2_read();
        switch(scancode) {
            default: {
                // unknown scancode (TODO: Log this)
            } break;
        }
    } else if(scancode <= SCANCODE_MAX) {
        if(scs_mapping[scancode] != 0) {
            key_press(scs_mapping[scancode]);
        }
    }else {
        scancode -= SCANCODE_MAX;
        if(scs_mapping[scancode] != 0) {
            key_release(scs_mapping[scancode]);
        }
    }
}

void ps2_keyboard_init(void) {
    register_irq_handler(IRQ_KEYBOARD, handle_keyboard_irq);

    // enable keyboard
    // ps2_command(0xF4);
    // ps2_read();

    // set scancode set to 1
    // ps2_command(0xED);
    // ps2_command(1);
    // ps2_read();

    // create scancode mapping

    // clear mapping
    memset(scs_mapping, 0, sizeof(scs_mapping));
    memset(shift_mapping, 0, sizeof(scs_mapping));

    // normal key mapping (scs 1)

    scs_mapping[0x0b] = '0';
    scs_mapping[0x02] = '1';
    scs_mapping[0x03] = '2';
    scs_mapping[0x04] = '3';
    scs_mapping[0x05] = '4';
    scs_mapping[0x06] = '5';
    scs_mapping[0x07] = '6';
    scs_mapping[0x08] = '7';
    scs_mapping[0x09] = '8';
    scs_mapping[0x0a] = '9';

    scs_mapping[0x1e] = 'a';
    scs_mapping[0x30] = 'b';
    scs_mapping[0x2e] = 'c';
    scs_mapping[0x20] = 'd';
    scs_mapping[0x12] = 'e';
    scs_mapping[0x21] = 'f';
    scs_mapping[0x22] = 'g';
    scs_mapping[0x23] = 'h';
    scs_mapping[0x17] = 'i';
    scs_mapping[0x24] = 'j';
    scs_mapping[0x25] = 'k';
    scs_mapping[0x26] = 'l';
    scs_mapping[0x32] = 'm';
    scs_mapping[0x31] = 'n';
    scs_mapping[0x18] = 'o';
    scs_mapping[0x19] = 'p';
    scs_mapping[0x10] = 'q';
    scs_mapping[0x13] = 'r';
    scs_mapping[0x1f] = 's';
    scs_mapping[0x14] = 't';
    scs_mapping[0x16] = 'u';
    scs_mapping[0x2f] = 'v';
    scs_mapping[0x11] = 'w';
    scs_mapping[0x2d] = 'x';
    scs_mapping[0x15] = 'y';
    scs_mapping[0x2c] = 'z';

    scs_mapping[0x2A] = KEYS_LEFT_SHIFT;
    scs_mapping[0x36] = KEYS_RIGHT_SHIFT;

    // key mapping while pressed SHIFT (scs 1)

    shift_mapping['0'] = ')';
    shift_mapping['1'] = '!';
    shift_mapping['2'] = '@';
    shift_mapping['3'] = '#';
    shift_mapping['4'] = '$';
    shift_mapping['5'] = '%';
    shift_mapping['6'] = '^';
    shift_mapping['7'] = '&';
    shift_mapping['8'] = '*';
    shift_mapping['9'] = '(';

    shift_mapping['a'] = 'A';
    shift_mapping['b'] = 'B';
    shift_mapping['c'] = 'C';
    shift_mapping['d'] = 'D';
    shift_mapping['e'] = 'E';
    shift_mapping['f'] = 'F';
    shift_mapping['g'] = 'G';
    shift_mapping['h'] = 'H';
    shift_mapping['i'] = 'I';
    shift_mapping['j'] = 'J';
    shift_mapping['k'] = 'K';
    shift_mapping['l'] = 'L';
    shift_mapping['m'] = 'M';
    shift_mapping['n'] = 'N';
    shift_mapping['o'] = 'O';
    shift_mapping['p'] = 'P';
    shift_mapping['q'] = 'Q';
    shift_mapping['r'] = 'R';
    shift_mapping['s'] = 'S';
    shift_mapping['t'] = 'T';
    shift_mapping['u'] = 'U';
    shift_mapping['v'] = 'V';
    shift_mapping['w'] = 'W';
    shift_mapping['x'] = 'X';
    shift_mapping['y'] = 'Y';
    shift_mapping['z'] = 'Z';
}
