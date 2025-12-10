#include "../BSP/board.h"
#include "../Common/std_types.h"
#include "../Driver/Components/uart.h"
#include "../Driver/Devices/servo.h"
#include "../Driver/Devices/hcsr04.h"
#include "../Driver/Devices/lcd.h"
#include <util/delay.h>
#include <avr/wdt.h>

/* DEFINE TR?NG THÁI */
#define STATE_RUNNING 0
#define STATE_STOPPED 1
#define STATE_MENU    2

/* BI?N TOÀN C?C */
int angle = 0;
int direction = 0; // 0: Tang, 1: Giam
int distance = 0;
uint8 system_state = STATE_RUNNING;
uint8 menu_option = 0;

/* HÀM KH?I T?O T?NG */
void Hardware_Init_All(void) {
	MCUSR = 0;
	wdt_disable();

	Servo_Init();
	Ultrasonic_Init();
	UART_Init();
	LCD_Init();

	// Init LED & Buzzer & Button (Code ng?n nên ?? ?ây ho?c t?o file bsp_io.c)
	SENSOR_DDR |= (1 << BUZZER_PIN);
	SENSOR_PORT &= ~(1 << BUZZER_PIN);
	
	LED_DDR |= (1 << LED_PIN);
	LED_PORT |= (1 << LED_PIN);

	BTN_DDR &= ~(1 << RESET_BTN_BIT);
	BTN_PORT |= (1 << RESET_BTN_BIT);
}

void System_Soft_Reset(void) {
	LCD_Clear();
	LCD_Print_String("RESETTING...");
	SENSOR_PORT |= (1 << BUZZER_PIN);
	_delay_ms(500);
	SENSOR_PORT &= ~(1 << BUZZER_PIN);
	wdt_enable(WDTO_15MS);
	while(1) {}
}

void Draw_Menu_Screen(void) {
	LCD_Clear();
	LCD_Print_String_At(0, 0, "Cur:");
	if (direction == 0) LCD_Print_String("Fwd"); else LCD_Print_String("Rev");
	LCD_Print_String_At(1, 0, "Set:");
	if (menu_option == 0) LCD_Print_String("Fwd"); else LCD_Print_String("Rev");
}

int main(void) {
	Hardware_Init_All();
	
	LCD_Clear();
	LCD_Print_String("System Ready");
	SENSOR_PORT |= (1 << BUZZER_PIN); _delay_ms(100); SENSOR_PORT &= ~(1 << BUZZER_PIN);
	
	Servo_Write_Angle(0);
	_delay_ms(1000);
	LCD_Clear();

	while (1) {
		/* --- CHECK RESET --- */
		if (!(BTN_PIN_REG & (1 << RESET_BTN_BIT))) {
			_delay_ms(20);
			if (!(BTN_PIN_REG & (1 << RESET_BTN_BIT))) System_Soft_Reset();
		}

		/* --- X? LÝ L?NH T? UART --- */
		char cmd = UART_Get_Cmd();
		if (cmd != 0) {
			if (cmd == 's') {
				if (system_state == STATE_RUNNING) {
					system_state = STATE_STOPPED;
					LCD_Clear(); LCD_Print_String("System Stop");
					} else if (system_state == STATE_STOPPED) {
					system_state = STATE_RUNNING;
					LCD_Clear();
				}
			}
			else if (cmd == 'm') {
				if (system_state == STATE_STOPPED) {
					system_state = STATE_MENU;
					menu_option = direction; Draw_Menu_Screen();
					} else if (system_state == STATE_MENU) {
					direction = menu_option;
					LCD_Clear(); LCD_Print_String("Saved OK!"); _delay_ms(1000);
					system_state = STATE_STOPPED;
					LCD_Clear(); LCD_Print_String("System Stop");
				}
			}
			else if (cmd == 'e') {
				if (system_state == STATE_MENU) {
					menu_option = !menu_option; Draw_Menu_Screen();
				}
			}
			UART_Clear_Cmd();
		}

		/* --- LOGIC CHÍNH --- */
		if (system_state == STATE_RUNNING) {
			
			distance = Ultrasonic_Get_Distance();
			
			// Còi báo ??ng
			if (distance >= 2 && distance < 40) SENSOR_PORT ^= (1 << BUZZER_PIN);
			else SENSOR_PORT &= ~(1 << BUZZER_PIN);

			UART_Send_Radar_Data(angle, distance);
			LCD_Update_Smart(distance, angle);

			// Quét Servo
			if (direction == 0) { // T?ng
				angle += 2;
				if (angle >= 180) {
					angle = 180;
					direction = 1;
					Servo_Write_Angle(angle);
					_delay_ms(500);
				}
				} else { // Gi?m
				angle -= 2;
				if (angle <= 0) {
					angle = 0;
					direction = 0;
					Servo_Write_Angle(angle);
					_delay_ms(500);
				}
			}
			
			Servo_Write_Angle(angle);
			_delay_ms(30);

			} else {
			// Khi d?ng thì t?t còi
			SENSOR_PORT &= ~(1 << BUZZER_PIN);
			_delay_ms(100);
		}
	}
}