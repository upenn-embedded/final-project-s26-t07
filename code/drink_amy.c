#define F_CPU 8000000UL		/* Define CPU Frequency 8MHz */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"
#include <stdint.h>
#define TURNTABLE_MOTOR_STEP   PD5
#define TURNTABLE_MOTOR_DIR    PD4
#define PUMP                   PD3
//#define WATER_FLOW_SENSOR    PD5

//volatile uint32_t pulse_count = 0;

//
//ISR(PCINT2_vect) {
//    static uint8_t last_state = 0;
//    uint8_t current_state = (PIND & (1 << WATER_FLOW_SENSOR)) ? 1 : 0;
//
//    if (current_state && !last_state) {
//        pulse_count++;
//    }
//
//    last_state = current_state;
//}


void initialize() {

    // set pin direction to output
    DDRD |= (1 << TURNTABLE_MOTOR_STEP);
    DDRD |= (1 << TURNTABLE_MOTOR_DIR);
    DDRD |= (1 << PD2);

    // set motor dir
    PORTD |= (1 << TURNTABLE_MOTOR_DIR);

    // set pump dir
    DDRD |= (1 << PUMP);
    
    // enable pull-up on PD1
//    PORTD |= (1 << WATER_FLOW_SENSOR);

//    PCICR |= (1 << PCIE2);
//    PCMSK2 |= (1 << PCINT17);

//    sei();

}

void run_motor(int iters) {
    PORTD |= (1 << PD2);
    for (int i = 0; i < iters; i++) {
        PORTD |= (1 << TURNTABLE_MOTOR_STEP);
        _delay_us(20); // short pulse
        PORTD &= ~(1 << TURNTABLE_MOTOR_STEP);
        _delay_ms(5); // faster stepping
    }
    PORTD &= ~(1 << PD2);
}

void run_pump(uint16_t time_ms) {
    PORTD |= (1 << PUMP);
    while (time_ms--) {
        _delay_ms(1);
    }
    PORTD &= ~(1 << PUMP);
}

void pump_on(void) {
    PORTD |= (1 << PUMP);
}

void pump_off(void) {
    PORTD &= ~(1 << PUMP);
}

// goal: dispense x amt of water
int main(void) {
    initialize();
    run_motor(2000);
    _delay_ms(500);
    run_pump(5000);
//    pump_on();
    while (1) {
    }
}
