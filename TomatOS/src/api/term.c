#include "include/api/term.h"

#include <string.h>

static term_t* current_term;

void term_write(char* text) {
	current_term->write(text);
}

void term_clear(void) {
	current_term->clear();
}

void term_set_text_color(uint8_t color) {
	current_term->set_text_color(color);
}

void term_set_background_color(uint8_t color) {
	current_term->set_background_color(color);
}

uint8_t term_get_text_color(void) {
    return current_term->get_text_color();
}

uint8_t term_get_background_color(void) {
    return current_term->get_background_color();
}

void term_set_cursor_pos(uint16_t x, uint16_t y) {
	current_term->set_cursor_pos(x, y);
}

uint16_t term_get_cursor_x(void) {
    return current_term->get_cursor_x();
}

uint16_t term_get_cursor_y(void) {
    return current_term->get_cursor_y();
}

uint16_t term_get_width(void) {
    return current_term->get_width();
}

uint16_t term_get_height(void) {
    return current_term->get_height();
}

void term_scroll(uint16_t n) {
	current_term->scroll(n);
}

void term_clear_line(uint16_t n) {
	current_term->clear_line(n);
}

void term_redirect(term_t* term) {
	current_term = term;
}

term_t* term_current(void) {
    return current_term;
}