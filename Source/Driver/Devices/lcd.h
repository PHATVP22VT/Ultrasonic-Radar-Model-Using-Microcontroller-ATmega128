#ifndef LCD_H
#define LCD_H

#include "../../Common/std_types.h"

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Home(void);
void LCD_Set_Cursor(uint8 row, uint8 col);
void LCD_Print_Char(char c);
void LCD_Print_String(const char *str);
void LCD_Print_String_At(uint8 row, uint8 col, const char *str);
void LCD_Print_Number(int number);

/* Hàm h? tr? hi?n th? riêng cho d? án này */
void LCD_Update_Smart(int dist, int ang);

#endif