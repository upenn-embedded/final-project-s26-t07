#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "touchscreen.h"


/*
Wiring

Y+ -> PC0  ADC0
X+ -> PC1  ADC1
Y- -> PC2  ADC2
X- -> PC3  ADC3
*/

#define YP_PIN  PC0
#define XP_PIN  PC1
#define YM_PIN  PC2
#define XM_PIN  PC3

#define YP_CH   0
#define XP_CH   1
#define YM_CH   2
#define XM_CH   3


/* Adjust after calibration */
#define TS_X_MIN 120
#define TS_X_MAX 900

#define TS_Y_MIN 120
#define TS_Y_MAX 900


#define TFT_WIDTH  480
#define TFT_HEIGHT 320



#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED   0xF800
#define GREEN 0x07E0
#define BLUE  0x001F
#define YELLOW 0xFFE0
#define CYAN  0x07FF
#define GRAY  0x8410


static void pins_all_input(void)
{
    DDRC &= ~(
        (1<<YP_PIN) |
        (1<<XP_PIN) |
        (1<<YM_PIN) |
        (1<<XM_PIN)
    );

    PORTC &= ~(
        (1<<YP_PIN) |
        (1<<XP_PIN) |
        (1<<YM_PIN) |
        (1<<XM_PIN)
    );
}


static uint16_t map_value(
    uint16_t v,
    uint16_t in_min,
    uint16_t in_max,
    uint16_t out_min,
    uint16_t out_max
){
    if(v < in_min) v = in_min;
    if(v > in_max) v = in_max;

    return
      (uint32_t)(v-in_min)*
      (out_max-out_min)/
      (in_max-in_min)
      + out_min;
}


void adc_init(void)
{
    ADMUX = (1<<REFS0);

    ADCSRA =
        (1<<ADEN) |
        (1<<ADPS2) |
        (1<<ADPS1) |
        (1<<ADPS0);
}


uint16_t adc_read(uint8_t channel)
{
    ADMUX =
      (ADMUX & 0xF0) |
      (channel & 0x0F);

    ADCSRA |= (1<<ADSC);

    while(ADCSRA & (1<<ADSC));

    return ADC;
}



/* Read X position */
uint16_t touch_read_x_raw(void)
{
    pins_all_input();

    /*
    Drive Y layer
    Y+ high
    Y- low
    Measure X+
    */

    DDRC |= (1<<YP_PIN) | (1<<YM_PIN);

    PORTC |= (1<<YP_PIN);
    PORTC &= ~(1<<YM_PIN);

    _delay_ms(2);

    return adc_read(XP_CH);
}



/* Read Y position */
uint16_t touch_read_y_raw(void)
{
    pins_all_input();

    /*
    Drive X layer
    X+ high
    X- low
    Measure Y+
    */

    DDRC |= (1<<XP_PIN) | (1<<XM_PIN);

    PORTC |= (1<<XP_PIN);
    PORTC &= ~(1<<XM_PIN);

    _delay_ms(2);

    return adc_read(YP_CH);
}



uint8_t touch_is_pressed(void)
{
    uint16_t x[4];
    uint16_t y[4];

    for (uint8_t i = 0; i < 4; i++) {
        x[i] = touch_read_x_raw();
        y[i] = touch_read_y_raw();
        _delay_ms(2);
    }

    for (uint8_t i = 0; i < 4; i++) {
        if (x[i] < 150 || x[i] > 900 || y[i] < 150 || y[i] > 900) {
            return 0;
        }
    }

    for (uint8_t i = 1; i < 4; i++) {
        if ((x[0] > x[i] ? x[0] - x[i] : x[i] - x[0]) > 20) {
            return 0;
        }
        if ((y[0] > y[i] ? y[0] - y[i] : y[i] - y[0]) > 20) {
            return 0;
        }
    }

    return 1;
}


//
//TouchPoint touch_get_point(void)
//{
//    TouchPoint p;
//
//    uint16_t xr = touch_read_x_raw();
//    uint16_t yr = touch_read_y_raw();
//
//    p.touched = 0;
//    p.x = 0;
//    p.y = 0;
//
//    if(
//       xr < 50 || xr > 1000 ||
//       yr < 50 || yr > 1000
//    ){
//        return p;
//    }
//
//    p.touched = 1;
//
//    p.x =
//      map_value(
//        xr,
//        TS_X_MIN,
//        TS_X_MAX,
//        0,
//        TFT_WIDTH-1
//      );
//
//    p.y =
//      map_value(
//        yr,
//        TS_Y_MIN,
//        TS_Y_MAX,
//        0,
//        TFT_HEIGHT-1
//      );
//
//    /* because your display origin is top-right */
//    p.x = map_value(xr, TS_X_MIN, TS_X_MAX, 0, TFT_WIDTH-1);
//    p.y = map_value(yr, TS_Y_MIN, TS_Y_MAX, 0, TFT_HEIGHT-1);
//  
//    p.y = TFT_HEIGHT-1-p.y;
//
//
//    return p;
//}

TouchPoint touch_get_point(void)
{
    TouchPoint p = {0, 0, 0};

    if (!touch_is_pressed()) {
        return p;
    }

    uint16_t xr = touch_read_x_raw();
    uint16_t yr = touch_read_y_raw();

    p.touched = 1;

    p.x = map_value(xr, TS_X_MIN, TS_X_MAX, 0, TFT_WIDTH - 1);
    p.y = map_value(yr, TS_Y_MIN, TS_Y_MAX, 0, TFT_HEIGHT - 1);

    p.y = TFT_HEIGHT - 1 - p.y;

    return p;
}

