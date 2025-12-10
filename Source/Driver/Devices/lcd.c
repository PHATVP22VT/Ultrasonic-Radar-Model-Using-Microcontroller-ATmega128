#include "lcd.h"
#include "../../BSP/board.h"
#include <util/delay.h>
#include <stdlib.h>

// --- Helper Functions (Private) ---
static void LCD_Enable_Pulse(void) {
	LCD_CTRL_PORT |= (1 << LCD_E_PIN);
	_delay_us(1);
	LCD_CTRL_PORT &= ~(1 << LCD_E_PIN);
	_delay_us(50);
}

static void LCD_Send_Nibble(uint8 data) {
	// Gi? nguyên 4 bit th?p c?a PORT d? li?u, ghi ?è 4 bit cao
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data & 0xF0);
	LCD_Enable_Pulse();
}

static void LCD_Send_Command(uint8 command) {
	LCD_CTRL_PORT &= ~(1 << LCD_RS_PIN); // RS = 0
	LCD_Send_Nibble(command & 0xF0);
	LCD_Send_Nibble((command << 4) & 0xF0);
	_delay_ms(2);
}

static void LCD_Send_Data(uint8 data) {
	LCD_CTRL_PORT |= (1 << LCD_RS_PIN);  // RS = 1
	LCD_Send_Nibble(data & 0xF0);
	LCD_Send_Nibble((data << 4) & 0xF0);
	_delay_ms(2);
}

// --- Public Functions ---
void LCD_Init(void) {
	LCD_CTRL_DDR |= (1 << LCD_RS_PIN) | (1 << LCD_E_PIN);
	LCD_DATA_DDR |= 0xF0; // Set Output cho 4 bit cao
	
	LCD_CTRL_PORT &= ~((1 << LCD_RS_PIN) | (1 << LCD_E_PIN));
	
	_delay_ms(50);
	LCD_CTRL_PORT &= ~(1 << LCD_RS_PIN);
	
	LCD_Send_Nibble(0x30); _delay_ms(5);
	LCD_Send_Nibble(0x30); _delay_us(150);
	LCD_Send_Nibble(0x30); _delay_ms(2);
	LCD_Send_Nibble(0x20); _delay_ms(2);
	
	LCD_Send_Command(0x28);
	LCD_Send_Command(0x0C);
	LCD_Send_Command(0x01);
	LCD_Send_Command(0x06);
	_delay_ms(2);
}

void LCD_Clear(void) { LCD_Send_Command(0x01); _delay_ms(2); }
void LCD_Home(void)  { LCD_Send_Command(0x02); _delay_ms(2); }

void LCD_Set_Cursor(uint8 row, uint8 col) {
	if (row == 0) LCD_Send_Command(0x80 + col);
	else LCD_Send_Command(0xC0 + col);
}

void LCD_Print_Char(char c) { LCD_Send_Data(c); }

void LCD_Print_String(const char *str) {
	while (*str) LCD_Send_Data(*str++);
}

void LCD_Print_String_At(uint8 row, uint8 col, const char *str) {
	LCD_Set_Cursor(row, col);
	LCD_Print_String(str);
}

void LCD_Print_Number(int number) {
	char buffer[12];
	itoa(number, buffer, 10);
	LCD_Print_String(buffer);
}

void LCD_Update_Smart(int dist, int ang) {
	LCD_Set_Cursor(0, 0);
	if (dist == 0) LCD_Print_String("No Object       ");
	else {
		LCD_Print_String("Dist: ");
		LCD_Print_Number(dist);
		LCD_Print_String("cm      ");
	}
	LCD_Print_String_At(1, 0, "Angle: ");
	LCD_Print_Number(ang);
	LCD_Print_String("    ");
} 