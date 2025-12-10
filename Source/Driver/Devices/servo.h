#ifndef SERVO_H
#define SERVO_H

/* * Hàm kh?i t?o chân ?i?u khi?n Servo 
 * (C?u hình Output cho chân n?i Servo)
 */
void Servo_Init(void);

/* * Hàm ?i?u khi?n góc quay Servo
 * @param ang: Góc quay mong mu?n (0 - 180 ??)
 * L?u ý: Hàm này có delay bên trong ?? ??m b?o servo quay ??n v? trí
 */
void Servo_Write_Angle(int ang);

#endif