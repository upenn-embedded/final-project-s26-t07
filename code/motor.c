#define F_CPU 8000000UL		/* Define CPU Frequency 8MHz */
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>		/* Include delay header file */

#define MOTOR_STEP PD0
#define MOTOR_DIR PD2

void initialize() {
    // set pin direction to output
    DDRD |= (1 << DDD0);
    DDRD |= (1 << DDD2);

    // set motor dir
    PORTD |= (1 << PD2);
    
}

void move_motor() {
    PORTD |= (1 << MOTOR_STEP);
    _delay_us(20); // short pulse
    PORTD &= ~(1 << MOTOR_STEP);
    _delay_ms(5); // faster stepping

}

int main(void) {
    initialize();

    while (1) {
        move_motor();
    }