#ifndef KERNEL_TERM_H
#define KERNEL_TERM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define COLOR_BLACK				(0x0)
#define COLOR_BLUE				(0x1)
#define COLOR_GREEN				(0x2)
#define COLOR_CYAN				(0x3)
#define COLOR_RED				(0x4)
#define COLOR_MAGENTA			(0x5)
#define COLOR_BROWN				(0x6)
#define COLOR_GRAY				(0x7)
#define COLOR_DARK_GRAY			(0x8)
#define COLOR_LIGHT_BLUE		(0x9)
#define COLOR_LIGHT_GREEN		(0xa)
#define COLOR_LIGHT_CYAN		(0xb)
#define COLOR_LIGHT_RED			(0xc)
#define COLOR_LIGHT_MAGENTA		(0xd)
#define COLOR_YELLOW			(0xe)
#define COLOR_WHITE				(0xf)


    void term_init();
	void term_init_syscalls();

    void term_write(const char* str);
    void term_clear();
    void term_set_text_color(uint8_t color);
    void term_set_background_color(uint8_t color);
    uint8_t term_get_text_color();
    uint8_t term_get_background_color();
    void term_set_cursor_pos(uint8_t x, uint8_t y);
    uint8_t term_get_cursor_x();
    uint8_t term_get_cursor_y();
    void term_scroll(uint8_t n);
    uint8_t term_get_height();
    uint8_t term_get_width();
    void term_clear_line(uint8_t n);

#ifdef __cplusplus
}
#endif

#endif