#ifndef HCSR04_H
#define HCSR04_H

/* * Hàm kh?i t?o c?m bi?n siêu âm
 * (C?u hình chân TRIG là Output, ECHO là Input)
 */
void Ultrasonic_Init(void);

/* * Hàm ?o kho?ng cách
 * @return: Kho?ng cách tính b?ng cm.
 * Tr? v? 0 n?u timeout ho?c kho?ng cách không h?p l? (<2cm ho?c >40cm)
 */
int Ultrasonic_Get_Distance(void);

#endif