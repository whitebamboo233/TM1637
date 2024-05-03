#include "stm32f10x.h"                  // Device header
void buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能时钟
	
	GPIO_InitTypeDef GPIO_InitStructre;
	GPIO_InitStructre.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructre.GPIO_Pin =GPIO_Pin_12;
	GPIO_InitStructre.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructre);//配置端口模式
	//GPIO配置好后默认低电平
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
}

void  buzzer_ON(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}
void buzzer_OFF(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);	
}
void buzzer_Turn(void)
{
	if(GPIO_ReadOutputDataBit (GPIOB,GPIO_Pin_12)==0)
	{
		
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	}
}
