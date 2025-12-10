#include "hcsr04.h"
#include "../../BSP/board.h"
#include <util/delay.h>

void Ultrasonic_Init(void) {
	SENSOR_DDR |= (1 << TRIG_PIN);
	SENSOR_DDR &= ~(1 << ECHO_PIN);
	SENSOR_PORT &= ~(1 << TRIG_PIN);
	SENSOR_PORT &= ~(1 << ECHO_PIN);
}

int Ultrasonic_Get_Distance(void) {
	long count = 0;
	SENSOR_PORT |= (1 << TRIG_PIN);
	_delay_us(12);
	SENSOR_PORT &= ~(1 << TRIG_PIN);
	
	long timeout = 0;
	while (!(SENSOR_PIN_REG & (1 << ECHO_PIN))) {
		timeout++;
		if (timeout > 3000) return 0;
	}
	while (SENSOR_PIN_REG & (1 << ECHO_PIN)) {
		count++;
		if (count > 25000) return 0;
	}
	int dist_cm = count / 6;
	if (dist_cm > 40 || dist_cm < 2) return 0;
	return dist_cm;
}