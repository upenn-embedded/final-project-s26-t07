#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "tft.h"
#include "touchscreen.h"
#include "ui.h"
#include "recipe.h"

// Motor
#define TURNTABLE_MOTOR_STEP   PD5
#define TURNTABLE_MOTOR_DIR    PD4
#define TURNTABLE_MOTOR_SLEEP  PD3

// Pumps
#define PUMP2                  PB0
#define PUMP3                  PD2

// Flow sensors
#define FLOW2                  PD7
#define FLOW3                  PD6

// Hall sensor moved off touchscreen pins
#define HALL_CHANNEL           4   // PC4 / ADC4
#define HALL_THRESHOLD         100

#define NUM_PUMPS              4
#define STEPS_PER_PUMP         417
#define ML_PER_PULSE           0.66

int current_position = 0;

volatile uint32_t flow_pulses_2 = 0;
volatile uint32_t flow_pulses_3 = 0;

uint16_t adc_read_main(uint8_t channel) {
    ADMUX = (1 << REFS0) | (channel & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

ISR(PCINT2_vect) {
    static uint8_t last_flow2 = 1;
    static uint8_t last_flow3 = 1;

    uint8_t current_flow2 = (PIND & (1 << FLOW2)) ? 1 : 0;
    uint8_t current_flow3 = (PIND & (1 << FLOW3)) ? 1 : 0;

    if (last_flow2 == 1 && current_flow2 == 0) flow_pulses_2++;
    if (last_flow3 == 1 && current_flow3 == 0) flow_pulses_3++;

    last_flow2 = current_flow2;
    last_flow3 = current_flow3;
}

void bartender_init(void) {
    DDRD |= (1 << TURNTABLE_MOTOR_STEP) |
            (1 << TURNTABLE_MOTOR_DIR) |
            (1 << TURNTABLE_MOTOR_SLEEP) |
            (1 << PUMP3);

    DDRB |= (1 << PUMP2);

    DDRD &= ~((1 << FLOW2) | (1 << FLOW3));
    PORTD |= (1 << FLOW2) | (1 << FLOW3);

    DDRC &= ~(1 << PC4);
    PORTC |= (1 << PC4);

    PORTD |= (1 << TURNTABLE_MOTOR_SLEEP);
    PORTD |= (1 << TURNTABLE_MOTOR_DIR);
    PORTD &= ~(1 << TURNTABLE_MOTOR_STEP);

    PORTB &= ~(1 << PUMP2);
    PORTD &= ~(1 << PUMP3);

    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT22) | (1 << PCINT23);

    sei();
}

void set_direction_clockwise(void) {
    PORTD |= (1 << TURNTABLE_MOTOR_DIR);
}

void set_direction_counterclockwise(void) {
    PORTD &= ~(1 << TURNTABLE_MOTOR_DIR);
}

void step_motor(void) {
    PORTD |= (1 << TURNTABLE_MOTOR_STEP);
    _delay_us(1900);
    PORTD &= ~(1 << TURNTABLE_MOTOR_STEP);
    _delay_us(1900);
}

void move_steps(int steps) {
    if (steps >= 0) {
        set_direction_clockwise();
    } else {
        set_direction_counterclockwise();
        steps = -steps;
    }

    for (int i = 0; i < steps; i++) {
        step_motor();
    }
}

void home_turntable(void) {
    set_direction_clockwise();

    while (1) {
        uint16_t hall_value = adc_read_main(HALL_CHANNEL);

        if (hall_value < HALL_THRESHOLD) {
            current_position = 0;
            break;
        }

        step_motor();
        _delay_ms(5);
    }
}

void move_to_pump(int pump_number) {
    int target_position = pump_number - 1;
    int step_difference = target_position - current_position;

    if (step_difference > NUM_PUMPS / 2) {
        step_difference -= NUM_PUMPS;
    } else if (step_difference < -NUM_PUMPS / 2) {
        step_difference += NUM_PUMPS;
    }

    move_steps(step_difference * STEPS_PER_PUMP);
    current_position = target_position;
}

void pump_on(int pump_number) {
    if (pump_number == 2) {
        PORTB |= (1 << PUMP2);
    } else if (pump_number == 3) {
        PORTD |= (1 << PUMP3);
    }
}

void pump_off(int pump_number) {
    if (pump_number == 2) {
        PORTB &= ~(1 << PUMP2);
    } else if (pump_number == 3) {
        PORTD &= ~(1 << PUMP3);
    }
}

void dispense_ml(int pump_number, float amount_ml) {
    uint32_t target_pulses = (uint32_t)(amount_ml / ML_PER_PULSE);

    if (pump_number == 2) {
        flow_pulses_2 = 0;
    } else if (pump_number == 3) {
        flow_pulses_3 = 0;
    } else {
        return;
    }

    pump_on(pump_number);

    if (pump_number == 2) {
        while (flow_pulses_2 < target_pulses) {
        }
    } else if (pump_number == 3) {
        while (flow_pulses_3 < target_pulses) {
        }
    }

    pump_off(pump_number);
}

void make_shirley_temple(void) {
    move_to_pump(2);
    dispense_ml(2, 100);

    _delay_ms(500);

    move_to_pump(3);
    dispense_ml(3, 200);
}

int main(void) {
    spi_init();
    tft_init();
    adc_init();

    bartender_init();

    ui_draw_homing();
    _delay_ms(200);

    home_turntable();
    
    _delay_ms(1000);
    
    move_to_pump(3);
    

    ui_set_drink_names("SHIRLEY", "WATER", "MILK");
    ui_init();
    
    while (1) {
        ui_update();

        if (ui_start_requested()) {
            ui_clear_start_request();
            ui_draw_dispense();

            if (ui_get_selected_drink() == 1) {
                make_shirley_temple();
            }

            _delay_ms(1000);
            ui_init();
        }

    _delay_ms(200);
    }
    
    
}