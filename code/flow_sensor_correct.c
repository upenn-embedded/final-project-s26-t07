#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

#define TURNTABLE_MOTOR_STEP PD4
#define TURNTABLE_MOTOR_DIR  PD2
#define PUMP                 PD3
#define WATER_FLOW_SENSOR    PD6

volatile uint32_t pulse_count = 0;

ISR(PCINT2_vect) {
    static uint8_t last_state = 0;
    uint8_t current_state = (PIND & (1 << WATER_FLOW_SENSOR)) ? 1 : 0;

    if (current_state && !last_state) {
        pulse_count++;
    }

    last_state = current_state;
}

void initialize(void) {
    DDRD |= (1 << TURNTABLE_MOTOR_STEP) | (1 << TURNTABLE_MOTOR_DIR) | (1 << PUMP);
    DDRD &= ~(1 << WATER_FLOW_SENSOR);

    PORTD |= (1 << TURNTABLE_MOTOR_DIR);

    // enable pull-up on PD1 if your flow sensor output needs it
    PORTD |= (1 << WATER_FLOW_SENSOR);

    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT17);

    sei();
}

void pump_on(void) {
    PORTD |= (1 << PUMP);
}

void pump_off(void) {
    PORTD &= ~(1 << PUMP);
}

int main(void) {
    initialize();
    uart_init();
    pump_off();

    while (1) {
        uint32_t count;

        cli();
        count = pulse_count;
        sei();

        printf("Pulse count: %lu\r\n", (unsigned long)count);
        _delay_ms(500);
    }
}