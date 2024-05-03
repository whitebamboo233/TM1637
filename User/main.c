#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "TM1637.h"
uint8_t keyNum;
//CLK-PB6 DIO-PB7
unsigned char t3=0;//4
unsigned char t2=0;//3
unsigned char t1=0;//2
unsigned char t0=1;//1
int main(void)
{
//	unsigned char i;
	Led_Init();
	TM1637_Init();
//	TM1637_Display_INC();

//	delay_ms(500);
//	TM1637_CLD();
//	delay_ms(500);

	TM1637_show_num(8025);
	delay_ms(500);
//	TM1637_SetBrightness(2);
//	TM1637_CLD();
//	TM1637_show_num(19);
//	delay_ms(500);
//	TM1637_Display_NoINC(1,0);
	while(1)
	{
//		LED3_Turn();
//		delay_ms(500);
//		show_time(1,2,5,5);
		show_second(t0,&t1,&t2,&t3);
//		dp_flicker();
		
//		TM1637_show_num(1105);
//		for(i=0;i<8;i++)
//		{
//			TM1637_SetBrightness(0);
//			delay_ms(500);
//		}
//		LED3_Turn();
//		delay_ms(500);
	}
	
}

