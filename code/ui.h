#ifndef UI_H
#define UI_H

#include <stdint.h>
#include "touchscreen.h"

void ui_init(void);
void ui_update(void);

void ui_draw_home(void);
void ui_draw_confirm(void);
void ui_draw_dispense(void);

uint8_t ui_button_pressed(uint16_t x, uint16_t y, uint16_t w, uint16_t h, TouchPoint p);

#endif
