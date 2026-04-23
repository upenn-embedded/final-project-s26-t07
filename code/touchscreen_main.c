#include "tft.h"
#include "touchscreen.h"

#define RED 0xF800
#define BLACK 0x0000

int main(void)
{
    spi_init();
    tft_init();
    adc_init();

    tft_fill_screen(BLACK);

    while(1)
    {
        TouchPoint p = touch_get_point();

        if(p.touched)
        {
            tft_fill_rect(
                p.x,
                p.y,
                4,
                4,
                RED
            );
        }
    }
}
