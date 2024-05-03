
#include "stm32f10x.h"                 
//LED1 A1
//LED2 A2	
void Led_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructre;
	GPIO_InitStructre.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructre.GPIO_Pin =GPIO_Pin_1| GPIO_Pin_2;
	GPIO_InitStructre.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructre);//配置端口模式
	
	GPIO_InitStructre.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructre.GPIO_Pin =GPIO_Pin_13;
	GPIO_InitStructre.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructre);
	
	//GPIO配置好后默认低电平
	GPIO_SetBits(GPIOA,GPIO_Pin_1| GPIO_Pin_2);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
}

void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_1);	
}
void LED1_Turn(void)
{
	if(GPIO_ReadOutputDataBit (GPIOA,GPIO_Pin_1)==0)
	{
		
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	}
}
void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}
void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_2);	
}
void LED2_Turn(void)
{
	if(GPIO_ReadOutputDataBit (GPIOA,GPIO_Pin_2)==0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_2);	
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	}
}

void LED3_Turn(void)
{
	if(GPIO_ReadOutputDataBit (GPIOC,GPIO_Pin_13)==0)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_13);	
	}
	else
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	}
}

