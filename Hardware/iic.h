#ifndef _IIC_H_
#define _IIC_H_

#include "stm32f10x.h"
#include "io_bit.h"
#include "delay.h"

#define IIC_GPIO_RCC RCC_APB2Periph_GPIOB
#define IIC_GPIO GPIOB
#define IIC_SCL_PIN GPIO_Pin_8
#define IIC_SDA_PIN GPIO_Pin_9

#define IIC_SCL PBout(8)
#define IIC_SDA PBout(9)
#define IIC_SDA_IN PBin(9)

void IIC_Pin_Init(void);

void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Ack(u8 ack);
u8 IIC_Send_Data(u8 data);
u8 IIC_Read_Data(void);

#endif
