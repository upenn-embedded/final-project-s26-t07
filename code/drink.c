#define F_CPU 8000000UL		/* Define CPU Frequency 8MHz */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

#define PUMP2                  PD7
#define WATER_FLOW_SENSOR      PD6
#define TURNTABLE_MOTOR_STEP   PD5
#define TURNTABLE_MOTOR_DIR    PD4
#define PUMP1                  PD3
#define TURNTABLE_MOTOR_SLEEP  PD2
#define FLOW_RATE              0.66 // mL / # ticks

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
    DDRD |= (1 << PUMP1);
    DDRD |= (1 << PUMP2);
    DDRD &= ~(1 << WATER_FLOW_SENSOR);

    // set motor dir
    PORTD |= (1 << TURNTABLE_MOTOR_DIR);

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

void pump1_on(void) {
    PORTD |= (1 << PUMP1);
}

void pump1_off(void) {
    PORTD &= ~(1 << PUMP1);
}

// run pump for mL amt of mL

void run_pump1(double mL) {
    uint32_t count;
    pump1_on();
    printf("PUMP ON\n");
    uint32_t target_pulses = (uint32_t) (mL / FLOW_RATE);


    cli();
    count = pulse_count;
    sei();

    while (1) {
        cli();
        count = pulse_count;
        sei();
        printf("Pulse count: %lu\r\n", (unsigned long) count);
        
        if (count >= target_pulses) {
            break;
        }
    }
    pump1_off();
    printf("PUMP OFF\n");
}

int main(void) {
    initialize();
    uart_init();
    run_motor(2000);
    run_pump1(FLOW_RATE * 600);
    
    while (1) {
        uint32_t count;

        cli();
        count = pulse_count;
        sei();

        printf("Pulse count: %lu\r\n", (unsigned long)count);
        _delay_ms(500);
        
    }
    /**
     FOR PUMP
        uint32_t count;

        cli();
        count = pulse_count;
        sei();

        printf("Pulse count: %lu\r\n", (unsigned long)count);
        _delay_ms(500);     
     */
}
