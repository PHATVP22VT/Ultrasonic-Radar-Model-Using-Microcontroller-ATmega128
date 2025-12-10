#ifndef UART_H
#define UART_H

void UART_Init(void);
void UART_Transmit(char data);
void UART_Print_String(char *str);
void UART_Send_Radar_Data(int ang, int dist);
char UART_Get_Cmd(void);    // L?y l?nh nh?n ???c
void UART_Clear_Cmd(void);  // Xóa l?nh sau khi x? lý

#endif