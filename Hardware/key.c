#include "stm32f10x.h"  // Device header
#include "Delay.h"
//K1 B1
//K2 B11
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructre;
	GPIO_InitStructre.GPIO_Mode =GPIO_Mode_IPU ;
	GPIO_InitStructre.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_11;
	GPIO_InitStructre.GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_Init(GPIOB,&GPIO_InitStructre);
}
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum=0;
	if((GPIO_ReadInputDataBit (GPIOB ,GPIO_Pin_1)==0))//0低电平，1高电平
	{
		delay_ms (20);
		while((GPIO_ReadInputDataBit (GPIOB ,GPIO_Pin_1)==0));//一直按下
		delay_ms (20);
		KeyNum=1;		
	}
		if((GPIO_ReadInputDataBit (GPIOB ,GPIO_Pin_11)==0))//0低电平，1高电平
	{
		delay_ms (20);
		while((GPIO_ReadInputDataBit (GPIOB ,GPIO_Pin_11)==0));//一直按下
		delay_ms (20);
		KeyNum=2;		
	}
	return KeyNum;
}
