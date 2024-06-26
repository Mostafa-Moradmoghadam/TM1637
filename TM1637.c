
#include "TM1637.h"
#include "STC12C5052AD.h"
#include <reg51.h>
const unsigned char segmentMap[2][17] = {
	  {
    0xED, 0x48, 0xE6, 0x6E, 0x4B, 0x2F, 0xAF, 0x68, // 0-7
    0xEF, 0x6f, 0xEB, 0x8F, 0xA5, 0xCE, 0xA7, 0x2B, // 8-9, A-F
		0X00
		},
	  {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, // 0-7
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, // 8-9, A-F	
		0x38 
		}
};
/***********************************/
void delay_TM1637(unsigned int delay)
{
	int i=0;
while(delay--)for( i=0;i<1;i++);
}
/***********************************/
void TM1637_dio_input(void)
{
P1M1&=~(1<<7);	
P1M0|=(1<<7);	
// data_dir &= ~(1<<sda);
}

/***********************************/
void TM1637_dio_output(void)
{
P1M1|=(1<<7);
P1M0&=~(1<<7);
//data_dir |= (1<<sda);
}
/***********************************/

unsigned char TM1637_dio_value(void)
{
delay_TM1637(100);
//return HAL_GPIO_ReadPin(GPIO_DIO[num],PIN_DIO[num]);
return	(P1&(1<<7))?1:0;	
 
}
/***********************************/
void TM1637_clk_high (void)
{
//HAL_GPIO_WritePin(GPIO_CLK[num],PIN_CLK[num],GPIO_PIN_SET);
P1|=(1<<6);	
delay_TM1637(100);
}

/***********************************/
void TM1637_clk_low (void)
{
P1&=~(1<<6);	
//HAL_GPIO_WritePin(GPIO_CLK[num],PIN_CLK[num],GPIO_PIN_RESET);
	
delay_TM1637(100);
}

/***********************************/
void TM1637_dio_high(void)
{
//HAL_GPIO_WritePin(GPIO_DIO[num],PIN_DIO[num],GPIO_PIN_SET);
P1|=(1<<7);	
delay_TM1637(100);
}
/***********************************/
void TM1637_dio_low(void)
{
//HAL_GPIO_WritePin(GPIO_DIO[num],PIN_DIO[num],GPIO_PIN_RESET);
P1&=~(1<<7);		
delay_TM1637(100);
}
/***********************************/

void TM1637_i2c_tx_bit_1 (void)
{
	TM1637_dio_output();
  TM1637_clk_low();
  TM1637_dio_high();
  TM1637_clk_high();
  TM1637_clk_low();
}

/***********************************/
void TM1637_i2c_tx_bit_0 (void)
{
	TM1637_dio_output();
  TM1637_clk_low();
  TM1637_dio_low();
  TM1637_clk_high();
  TM1637_clk_low();
}

/***********************************/
unsigned char TM1637_i2c_rx_bit(void)
{
  unsigned char i;
  TM1637_dio_input();	
  TM1637_clk_high();
	i=TM1637_dio_value();

	TM1637_clk_low(); 
  return i;
}
/***********************************/

//--------------------------------------------------------------------------------


void TM1637_i2c_start(void)
{
	TM1637_dio_output();
	TM1637_clk_high();
  TM1637_dio_high();
	TM1637_dio_low();
	delay_TM1637(100);
  TM1637_clk_low();  
}



void TM1637_i2c_stop(void)
{
	TM1637_dio_output();	
  TM1637_dio_low();
	TM1637_clk_low();
	TM1637_clk_high();
  TM1637_dio_high();  
}
/***********************************/

unsigned char TM1637_i2c_write(unsigned char Data)
{
  unsigned char i;


  for(i=0;i<8;i++)
    {

      if(Data & 0x01)
        TM1637_i2c_tx_bit_1();
      else
        TM1637_i2c_tx_bit_0();

      Data >>=1;
    }

//		i2c_tx_bit_0(); 
//		  sda_high();
//		delay();
	//	 
 return TM1637_i2c_rx_bit();
}

/***********************************/
unsigned char TM1637_i2c_read(void)
{
  unsigned char i;
  unsigned char Data=0;
	//delay();
  for(i=0;i<8;i++)
    {
      if(TM1637_i2c_rx_bit())
        Data |= 1 << i;
    }
TM1637_i2c_rx_bit();

  return Data;
}
/***********************************/
unsigned char TM1637_init(void)
{
//TM1637_i2c_start();
//TM1637_i2c_write(0x8f);
//TM1637_i2c_stop();	
//delay(100);
TM1637_i2c_start();	
TM1637_i2c_write(0x40);
TM1637_i2c_stop();	
delay_TM1637(100);
TM1637_i2c_start();	
TM1637_i2c_write(0xC0);	
TM1637_i2c_write(segmentMap[0][0]);	
TM1637_i2c_write(segmentMap[0][0]);	
TM1637_i2c_write(segmentMap[0][0]);	
TM1637_i2c_write(segmentMap[1][0]);	
TM1637_i2c_write(segmentMap[1][0]);
TM1637_i2c_write(segmentMap[1][0]);
TM1637_i2c_stop();	
delay_TM1637(100);
TM1637_i2c_start();
TM1637_i2c_write(0x8f);
TM1637_i2c_stop();
return 0;
}
/***********************************/
unsigned char TM1637_DisplayDecimal_Small(unsigned int number)
{
	unsigned char i[3]={0},j=0;
if( number > 999 )
	return 0;

i[0]=number%10;number/=10;
i[1]=number%10;number/=10;
i[2]=number%10;

TM1637_i2c_start();	
TM1637_i2c_write(0xC3);	
TM1637_i2c_write(segmentMap[1][i[2]]);
TM1637_i2c_write(segmentMap[1][i[1]]);
TM1637_i2c_write(segmentMap[1][i[0]]);
TM1637_i2c_stop();
return 1;	
}
/***********************************/
unsigned char TM1637_DisplayChar_Small(unsigned char number0,unsigned char number1,unsigned char number2)
{

TM1637_i2c_start();	
TM1637_i2c_write(0xC3);	
TM1637_i2c_write(segmentMap[1][number0]);
TM1637_i2c_write(segmentMap[1][number1]);
TM1637_i2c_write(segmentMap[1][number2]);
TM1637_i2c_stop();
return 1;	
}
/***********************************/
unsigned char TM1637_DisplayDecimal_Big(unsigned int number)
{
	unsigned char i[3]={0},j=0;
if( number >999 )
	return 0;
i[0]=number%10;number/=10;
i[1]=number%10;number/=10;
i[2]=number%10;



TM1637_i2c_start();	
TM1637_i2c_write(0xC0);	

TM1637_i2c_write(segmentMap[0][i[0]]);
TM1637_i2c_write(segmentMap[0][i[1]]);
TM1637_i2c_write(segmentMap[0][i[2]]);

TM1637_i2c_stop();
return 1;	
}
/***********************************/
unsigned char TM1637_Read_Key(void)
{
unsigned char key_value=0;
TM1637_i2c_start();	
TM1637_i2c_write(0X42);	
key_value = TM1637_i2c_read();
TM1637_i2c_stop();
return key_value;	
}
/***********************************/
void TM1637_DisplayDecimal_Small_Off(void)
{

TM1637_i2c_start();	
TM1637_i2c_write(0xC3);	
TM1637_i2c_write(0);
TM1637_i2c_write(0);
TM1637_i2c_write(0);
TM1637_i2c_stop();
}
/***********************************/
void TM1637_DisplayDecimal_Big_Off(void)
{

TM1637_i2c_start();	
TM1637_i2c_write(0xC0);	
TM1637_i2c_write(0);
TM1637_i2c_write(0);
TM1637_i2c_write(0);
TM1637_i2c_stop();
}