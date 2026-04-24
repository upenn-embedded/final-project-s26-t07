#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "tft.h"
#include "touchscreen.h"
#include "ui.h"

#define RED 0xF800
#define BLACK 0x0000


void clear_screen() {
    tft_fill_screen(BLACK);
}

void test_touchscreen_red_dots() {
    while(1)
    {
        TouchPoint p = touch_get_point();

        if (p.touched) {
    tft_fill_rect(p.x, p.y, 4, 4, RED);
    _delay_ms(100);
    }       
    }
    
}

int main(void)
{
    spi_init();
    tft_init();
    adc_init();

    clear_screen();
    
    ui_init();

    while (1) {
        ui_update();
        _delay_ms(100);
    }


}
