#define F_CPU 8000000UL		/* Define CPU Frequency 8MHz */
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>		/* Include delay header file */

#define TURNTABLE_MOTOR_STEP PD5
#define TURNTABLE_MOTOR_DIR  PD4
#define PUMP                 PD3



void initialize() {
    
    // set pin direction to output
    DDRD |= (1 << TURNTABLE_MOTOR_STEP);
    DDRD |= (1 << TURNTABLE_MOTOR_DIR);

    // set motor dir
    PORTD |= (1 << TURNTABLE_MOTOR_DIR);
    
    // set pump dir
    DDRD |= (1 << PUMP);
    
}

void move_motor() {
    PORTD |= (1 << TURNTABLE_MOTOR_STEP);
    _delay_us(20); // short pulse
    PORTD &= ~(1 << TURNTABLE_MOTOR_STEP);
    _delay_ms(5); // faster stepping

}

void motor_off() {
    PORTD &= ~(1 << TURNTABLE_MOTOR_STEP);
}

void pump_on(void) {
    PORTD |= (1 << PUMP);
}

void pump_off(void) {
    PORTD &= ~(1 << PUMP);
}

int main(void) {
    initialize();
    pump_on();
        
    while (1) {
//        move_motor();
    }
}
