#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "tft.h"
#include "touchscreen.h"
#include "ui.h"
#include "recipe.h"


#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define CYAN    0x07FF
#define YELLOW  0xFFE0
#define GRAY    0x8410
#define DARK    0x4208

Recipe menu_drinks[3];


int main(void)
{
    spi_init();
    tft_init();
    adc_init();

    menu_drinks[0] = recipe_1;
    menu_drinks[1] = recipe_2;
    menu_drinks[2] = recipe_3;

    ui_set_drink_names(
        menu_drinks[0].name,
        menu_drinks[1].name,
        menu_drinks[2].name
    );
    
    ui_init();

    while (1) {
        ui_update();
        _delay_ms(100);
    }

}




