#ifndef API_TERM_H
#define API_TERM_H

#include <stdint.h>
#include <stdbool.h>

typedef struct term_t {
    void (*write)(char* text);
    void (*clear)(void);
    void (*set_text_color)(uint8_t color);
    void (*set_background_color)(uint8_t color);
    uint8_t (*get_text_color)(void);
    uint8_t (*get_background_color)(void);
    void (*set_cursor_pos)(uint16_t x, uint16_t y);
    uint16_t (*get_cursor_x)(void);
    uint16_t (*get_cursor_y)(void);
    uint16_t (*get_width)(void);
    uint16_t (*get_height)(void);
    void (*scroll)(uint16_t n);
    void (*clear_line)(uint16_t n);
} term_t;

void term_write(char* text);
void term_clear(void);
void term_set_text_color(uint8_t color);
void term_set_background_color(uint8_t color);
uint8_t term_get_text_color(void);
uint8_t term_get_background_color(void);
void term_set_cursor_pos(uint16_t x, uint16_t y);
uint16_t term_get_cursor_x(void);
uint16_t term_get_cursor_y(void);
uint16_t term_get_width(void);
uint16_t term_get_height(void);
void term_scroll(uint16_t n);
void term_clear_line(uint16_t n);

void term_cursor_blink(bool enabled);
void term_redirect(term_t* term);
term_t* term_current(void);
term_t* term_native(void);

void kernel_init_native_term(void);

#endif