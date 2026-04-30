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

// TODO: Currently only tracks FLOW_SENSOR1
volatile uint32_t pulse_count = 0;
volatile uint32_t pulse_count2 = 0;
volatile uint32_t pulse_count3 = 0;
volatile uint32_t pulse_count4 = 0;

ISR(PCINT2_vect) {
    static uint8_t last_state = 0;
    uint8_t current_state = (PIND & (1 << FLOW_SENSOR1)) ? 1 : 0;

    if (current_state && !last_state) {
        pulse_count++;
    }

    last_state = current_state;
}

ISR(PCINT0_vect) {
    static uint8_t last_state2 = 0;
    uint8_t current_state2 = (PINB & (1 << FLOW_SENSOR2)) ? 1 : 0;

    if (current_state2 && !last_state2) {
        pulse_count2++;
    }

    last_state2 = current_state2;
}

ISR(PCINT3_vect) {
    static uint8_t last_state3 = 0;
    static uint8_t last_state4 = 0;

    uint8_t current_state3 = (PINE & (1 << FLOW_SENSOR3)) ? 1 : 0;
    uint8_t current_state4 = (PINE & (1 << FLOW_SENSOR4)) ? 1 : 0;

    if (current_state3 && !last_state3) {
        pulse_count3++;
    }

    if (current_state4 && !last_state4) {
        pulse_count4++;
    }

    last_state3 = current_state3;
    last_state4 = current_state4;
}

void flow_sensor_init1() {
    DDRD &= ~(1 << FLOW_SENSOR1);

    // enable pull-up on PD1
    PORTD |= (1 << FLOW_SENSOR1);

    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT22);
}

void flow_sensor_init2() { // PB4
    DDRB &= ~(1 << FLOW_SENSOR2);
    PORTB |= (1 << FLOW_SENSOR2);

    PCICR |= (1 << PCIE0);          // Enable Port B interrupts
    PCMSK0 |= (1 << PCINT4);        // PB4
}

void flow_sensor_init3(void) {
    DDRE &= ~(1 << FLOW_SENSOR3);
    PORTE |= (1 << FLOW_SENSOR3);

    PCICR |= (1 << PCIE3);
    PCMSK3 |= (1 << PCINT24);
}

void flow_sensor_init4(void) {
    DDRE &= ~(1 << FLOW_SENSOR4);
    PORTE |= (1 << FLOW_SENSOR4);

    PCICR |= (1 << PCIE3);
    PCMSK3 |= (1 << PCINT25);
}


void pump_init(int pump) {
    // set to output direction
    DDRD |= (1 << pump);
}

void initialize() {
    // set pin direction to output
    DDRD |= (1 << TURNTABLE_MOTOR_STEP);
    DDRD |= (1 << TURNTABLE_MOTOR_DIR);
    DDRD |= (1 << TURNTABLE_MOTOR_SLEEP);
    pump_init(PUMP1);
    pump_init(PUMP2);
    pump_init(PUMP3);
    pump_init(PUMP4);

    // set motor dir
    PORTD |= (1 << TURNTABLE_MOTOR_DIR);

    // set sleep to 0
    PORTD &= (1 << TURNTABLE_MOTOR_SLEEP);

    flow_sensor_init1();
    flow_sensor_init2();
    flow_sensor_init3();
    flow_sensor_init4();

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

void pump_on(int pump) {
    PORTD |= (1 << pump);
}

void pump_off(int pump) {
    PORTD &= ~(1 << pump);
}

// run pump for mL amt of mL

void run_pump(double mL, int pump) {
    uint32_t count;
    pump_on(pump);
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
    pump_off(pump);
    printf("PUMP OFF\n");
}

int main(void) {
    initialize();
    uart_init();
    run_motor(2000);
//    run_pump(FLOW_RATE * 600, PUMP1);

//    while (1) {
//        uint32_t count;
//
//        cli();
//        count = pulse_count;
//        sei();
//
//        printf("Pulse count: %lu\r\n", (unsigned long)count);
//        _delay_ms(500);
//
//    }
}