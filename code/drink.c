#define F_CPU 8000000UL		/* Define CPU Frequency 8MHz */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

#define WATER_FLOW_SENSOR      PD6
#define TURNTABLE_MOTOR_STEP   PD5
#define TURNTABLE_MOTOR_DIR    PD4
#define PUMP                   PD3
#define TURNTABLE_MOTOR_SLEEP  PD2
#define FLOW_RATE               0.66 // mL / # ticks
volatile uint32_t pulse_count = 0;

ISR(PCINT2_vect) {
    static uint8_t last_state = 0;
    uint8_t current_state = (PIND & (1 << WATER_FLOW_SENSOR)) ? 1 : 0;

    if (current_state && !last_state) {
        pulse_count++;
    }

    last_state = current_state;
}

void initialize() {
    // set pin direction to output
    DDRD |= (1 << TURNTABLE_MOTOR_STEP);
    DDRD |= (1 << TURNTABLE_MOTOR_DIR);
    DDRD |= (1 << TURNTABLE_MOTOR_SLEEP);
    DDRD |= (1 << PUMP);
    DDRD &= ~(1 << WATER_FLOW_SENSOR);

    // set motor dir
    PORTD |= (1 << TURNTABLE_MOTOR_DIR);

    // set pump dir

    // enable pull-up on PD1
    PORTD |= (1 << WATER_FLOW_SENSOR);

    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT22);

    sei();

}

void run_motor(int iters) {
    PORTD |= (1 << TURNTABLE_MOTOR_SLEEP);
    for (int i = 0; i < iters; i++) {
        PORTD |= (1 << TURNTABLE_MOTOR_STEP);
        _delay_us(20); // short pulse
        PORTD &= ~(1 << TURNTABLE_MOTOR_STEP);
        _delay_ms(5); // faster stepping
    }
    PORTD &= ~(1 << TURNTABLE_MOTOR_SLEEP);
}

void pump_on(void) {
    PORTD |= (1 << PUMP);
}

void pump_off(void) {
    PORTD &= ~(1 << PUMP);
}

// run pump for mL amt of mL
void run_pump(double mL) {
    uint32_t count;
    pump_on();
    printf("PUMP ON\n");

    cli();
    count = pulse_count;
    sei();

    while (count < mL * FLOW_RATE) {
        cli();
        count = pulse_count;
        sei();
        printf("Pulse count: %lu\r\n", (unsigned long) count);
    }
    pump_off();
    printf("PUMP OFF\n");
}

int main(void) {
    initialize();
    uart_init();
    run_motor(2000);
    run_pump(FLOW_RATE * 600);
}
