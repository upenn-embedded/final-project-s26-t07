#define F_CPU 16000000UL		/* Define CPU Frequency 8MHz */
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>		/* Include delay header file */

#define TURNTABLE_MOTOR_STEP PD0
#define TURNTABLE_MOTOR_DIR PD2
#define PUMP PD3

void initialize() {
    // set pin direction to output
    DDRD |= (1 << DDD0);
    DDRD |= (1 << DDD2);

    // set motor dir
    PORTD |= (1 << PD2);
    
    // set pump direction
    DDRD |= (1 << DDD3);

}

void move_motor() {
    PORTD |= (1 << TURNTABLE_MOTOR_STEP);
    _delay_us(20); // short pulse
    PORTD &= ~(1 << TURNTABLE_MOTOR_STEP);
    _delay_ms(5); // faster stepping

}

int main(void) {
    initialize();
    PORTD |= (1 << DDD3);
    while (1) {
    }
}
