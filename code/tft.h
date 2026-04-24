#ifndef TFT_H
#define TFT_H

#include <avr/io.h>
#include <stdint.h>

void spi_init(void);
void tft_reset(void);
void tft_init(void);

void tft_write_command(uint8_t cmd);
void tft_write_data(uint8_t data);
void tft_write_data16(uint16_t data);

void tft_set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

void tft_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void tft_draw_fast_hline(uint16_t x, uint16_t y, uint16_t w, uint16_t color);
void tft_draw_fast_vline(uint16_t x, uint16_t y, uint16_t h, uint16_t color);
void tft_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

void tft_fill_rect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
void tft_fill_screen(uint16_t color);

void tft_test_block(void);

void tft_draw_char(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
void tft_draw_string(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size);

#endif
