#define F_CPU 8000000UL		/* Define CPU Frequency 8MHz */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

#define TURNTABLE_MOTOR_STEP   PD5
#define TURNTABLE_MOTOR_DIR    PD4
#define TURNTABLE_MOTOR_SLEEP  PD2

#define PUMP1                  PD3
#define PUMP2                  PD7
#define PUMP3                  PC4
#define PUMP4                  PC5

#define FLOW_SENSOR1           PD6
#define FLOW_SENSOR2           PB4
#define FLOW_SENSOR3           PE0
#define FLOW_SENSOR4           PE1
#define HALL_EFFECT_SENSOR     PE2


#define FLOW_RATE              0.66 // mL / # ticks

void initialize() {
    // set pin direction to output
    DDRD |= (1 << TURNTABLE_MOTOR_STEP);
    DDRD |= (1 << TURNTABLE_MOTOR_DIR);
    DDRD |= (1 << TURNTABLE_MOTOR_SLEEP);

    // preload output LOW first
    //    PORTD &= ~((1 << PUMP1) | (1 << PUMP2));
    //    PORTC &= ~((1 << PUMP3) | (1 << PUMP4));

    // then make pins outputs
    //    DDRD |= (1 << PUMP1) | (1 << PUMP2);
    //    DDRC |= (1 << PUMP3) | (1 << PUMP4);

    //    // set motor dir
    PORTD |= (1 << TURNTABLE_MOTOR_DIR);
    PORTD |= (1 << TURNTABLE_MOTOR_SLEEP);

    // set sleep to 1 always
    //    PORTD |= (1 << TURNTABLE_MOTOR_SLEEP);

}

void run_motor(int iters) {
    //    PORTD |= (1 << TURNTABLE_MOTOR_SLEEP);
    for (int i = 0; i < iters; i++) {
        PORTD |= (1 << TURNTABLE_MOTOR_STEP);
        _delay_us(20); // short pulse
        PORTD &= ~(1 << TURNTABLE_MOTOR_STEP);
        _delay_ms(5); // faster stepping
    }
    //    PORTD &= ~(1 << TURNTABLE_MOTOR_SLEEP);
}

int main(void) {
    initialize();
    //    while (1`
    //    run_motor(2000);
    while (1) {
        PORTD |= (1 << TURNTABLE_MOTOR_STEP);
        _delay_us(5);
        PORTD &= ~(1 << TURNTABLE_MOTOR_STEP);
        _delay_ms(10);

    }
}