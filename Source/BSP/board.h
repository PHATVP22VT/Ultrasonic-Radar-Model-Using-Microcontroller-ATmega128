#ifndef BOARD_H
#define BOARD_H

#include <avr/io.h>

#define F_CPU 1000000UL

/* --- LCD CONFIG (Map chân t? lcd.h c? sang ?ây) --- */
#define LCD_CTRL_PORT PORTB
#define LCD_CTRL_DDR  DDRB
#define LCD_RS_PIN    PB0
#define LCD_E_PIN     PB2

#define LCD_DATA_PORT PORTD
#define LCD_DATA_DDR  DDRD
/* L?u ý: Code LCD dùng 4 bit cao (D4-D7) c?a PORTD */

/* --- SERVO CONFIG --- */
#define SERVO_DDR   DDRC
#define SERVO_PORT  PORTC
#define SERVO_PIN   PC0

/* --- ULTRASONIC SENSOR CONFIG --- */
#define SENSOR_DDR     DDRC
#define SENSOR_PORT    PORTC
#define SENSOR_PIN_REG PINC
#define TRIG_PIN       PC3
#define ECHO_PIN       PC4

/* --- BUZZER CONFIG --- */
#define BUZZER_PIN     PC5

/* --- BUTTON CONFIG --- */
#define BTN_PIN_REG    PIND
#define BTN_PORT       PORTD
#define BTN_DDR        DDRD
#define RESET_BTN_BIT  PD3

/* --- LED CONFIG --- */
#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define LED_PIN     PB1

/* --- UART CONFIG --- */
#define BAUD 9600
#define UBRR_VAL ((F_CPU/8/BAUD)-1)

#endif