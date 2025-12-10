#include "uart.h"
#include "../../BSP/board.h"
#include <avr/interrupt.h>
#include <stdlib.h>

volatile char g_received_cmd = 0;

void UART_Init(void) {
	UBRR0H = (unsigned char)(UBRR_VAL >> 8);
	UBRR0L = (unsigned char)UBRR_VAL;
	UCSR0A |= (1 << U2X0);
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	sei();
}

void UART_Transmit(char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void UART_Print_String(char *str) {
	while (*str) UART_Transmit(*str++);
}

void UART_Send_Radar_Data(int ang, int dist) {
	char buffer[10];
	itoa(ang, buffer, 10);
	UART_Print_String(buffer);
	UART_Transmit(',');
	itoa(dist, buffer, 10);
	UART_Print_String(buffer);
	UART_Transmit('.');
}

char UART_Get_Cmd(void) { return g_received_cmd; }
void UART_Clear_Cmd(void) { g_received_cmd = 0; }

ISR(USART_RX_vect) {
	g_received_cmd = UDR0;
}