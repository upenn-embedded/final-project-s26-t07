#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "tft.h"
#include "touchscreen.h"
#include "ui.h"
#include "recipe.h"
//#include "uart.h"


#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define CYAN    0x07FF
#define YELLOW  0xFFE0
#define GRAY    0x8410
#define DARK    0x4208

#define PUMP2 PD7
#define PUMP3 PC4

#define TURNTABLE_MOTOR_STEP PD5
#define TURNTABLE_MOTOR_DIR PD4
#define TURNTABLE_MOTOR_SLEEP PD2

#define WATER_FLOW_SENSOR PD6

Recipe menu_drinks[3];

void run_motor(int iters) {
    PORTD |= (1 << TURNTABLE_MOTOR_SLEEP);
    for (int i = 0; i < iters; i++) {
        PORTD |= (1 << TURNTABLE_MOTOR_STEP);
        _delay_us(5);
        PORTD &= ~(1 << TURNTABLE_MOTOR_STEP);
        _delay_ms(10);
    }
    PORTD &= ~(1 << TURNTABLE_MOTOR_SLEEP);
}

int main(void)
{
    spi_init();
    tft_init();
    adc_init();
    DDRD |= (1 << PUMP2);
    DDRC |= (1 << PUMP3);
    
    PORTD |= (1 << PUMP3);
    PORTC |= (1 << PUMP2);

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



