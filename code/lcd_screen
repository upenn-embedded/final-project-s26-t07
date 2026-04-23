#define F_CPU 8000000UL
#include "tft.h"
#include <util/delay.h>

#define BLACK 0x0000
#define RED   0xF800
#define GREEN 0x07E0
#define BLUE  0x001F
#define WHITE 0xFFFF

int main(void) {
    _delay_ms(200);
    spi_init();
    tft_init();
    _delay_ms(200);

    tft_fill_rect(0, 0, 480, 320, BLACK);   
    _delay_ms(50);                    
    
    tft_fill_rect(10, 10, 50, 50, GREEN);   
    _delay_ms(50); 
    
    tft_fill_rect(130,20,80,50,GREEN);
    _delay_ms(50);
    
    tft_fill_rect(240,20,80,50,BLUE);
    _delay_ms(50);

    // Horizontal line
    tft_draw_fast_hline(20, 190,250,GREEN);
    _delay_ms(50);

    // Vertical line
    tft_draw_fast_vline(150,180,80,BLUE);
    _delay_ms(50);
    
    // Single character
    tft_draw_char(20,220,'A',WHITE,BLACK,3);

    // String
    tft_draw_string(80,250,"HELLO",WHITE,BLACK,2);

    while (1) {
    }
}
