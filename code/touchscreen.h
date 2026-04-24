#ifndef TOUCH_H
#define TOUCH_H

#include <stdint.h>

typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t touched;
} TouchPoint;

void adc_init(void);
uint16_t adc_read(uint8_t channel);

uint16_t touch_read_x_raw(void);
uint16_t touch_read_y_raw(void);
uint8_t touch_is_pressed(void);
TouchPoint touch_get_point(void);

#endif
