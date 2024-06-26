#ifndef _TM1637_H
#define _TM1637_H

//#include "STC12C5052AD.h"

unsigned char TM1637_init(void);
unsigned char TM1637_DisplayDecimal_Big(unsigned int number);
unsigned char TM1637_DisplayDecimal_Small(unsigned int number);
void TM1637_DisplayDecimal_Small_Off(void);
void TM1637_DisplayDecimal_Big_Off(void);
unsigned char TM1637_DisplayChar_Small(unsigned char number0,unsigned char number1,unsigned char number2);
unsigned char TM1637_Read_Key(void);
#endif
