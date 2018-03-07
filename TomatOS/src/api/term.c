#include "include/api/term.h"

#include <string.h>

term_t* __current_term;

void term_write(char* text) {
    __current_term->write(text);
}

void term_clear(void) {
    __current_term->clear();
}

void term_set_text_color(uint8_t color) {
    __current_term->set_text_color(color);
}

void term_set_background_color(uint8_t color) {
    __current_term->set_background_color(color);
}

uint8_t term_get_text_color(void) {
    return __current_term->get_text_color();
}

uint8_t term_get_background_color(void) {
    return __current_term->get_background_color();
}

void term_set_cursor_pos(uint16_t x, uint16_t y) {
    __current_term->set_cursor_pos(x, y);
}

uint16_t term_get_cursor_x(void) {
    return __current_term->get_cursor_x();
}

uint16_t term_get_cursor_y(void) {
    return __current_term->get_cursor_y();
}

uint16_t term_get_width(void) {
    return __current_term->get_width();
}

uint16_t term_get_height(void) {
    return __current_term->get_height();
}

void term_scroll(uint16_t n) {
    __current_term->scroll(n);
}

void term_clear_line(uint16_t n) {
    __current_term->clear_line(n);
}

void term_redirect(term_t* term) {
    __current_term = term;
}

term_t* term_current(void) {
    return __current_term;
}