#include "servo.h"
#include "../../BSP/board.h"
#include <util/delay.h>
#include <util/delay_basic.h>

void Servo_Init(void) {
	SERVO_DDR |= (1 << SERVO_PIN);
	SERVO_PORT &= ~(1 << SERVO_PIN);
}

void Servo_Write_Angle(int ang) {
	if (ang < 0) ang = 0;
	if (ang > 180) ang = 180;

	// Logic tính toán t? code g?c
	double pulse_us = 500.0 + ((double)ang * 10.55);
	uint16_t loop_count = (uint16_t)((pulse_us * (F_CPU / 1000000UL)) / 4);

	SERVO_PORT |= (1 << SERVO_PIN);
	_delay_loop_2(loop_count);
	SERVO_PORT &= ~(1 << SERVO_PIN);
	
	_delay_ms(15);
}  