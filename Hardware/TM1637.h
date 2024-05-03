#ifndef _TM1637_H
#define _TM1637_H
#include "stm32f10x.h"                  // Device header

extern unsigned char tab[];


/* 定义IIC连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define TM1637_CLK_GPIO_PORT    	GPIOB		                /* GPIO端口 */
#define TM1637_CLK_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define TM1637_CLK_GPIO_PIN			  GPIO_Pin_6

#define TM1637_DIO_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define TM1637_DIO_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define TM1637_DIO_GPIO_PIN		    GPIO_Pin_7
void TM1637_Init(void);
void TM1637_Init( void );
void TM1637_Start( void );
void TM1637_Ack( void );
void TM1637_Stop( void );
void TM1637_WriteByte( unsigned char oneByte );
unsigned char TM1637_ScanKey( void );
void TM1637_NixieTubeDisplay( void );

void TM1637_SetBrightness( unsigned char level );
void TM1637_Display_INC( void );
void TM1637_Display_NoINC( unsigned char add, unsigned char value );
unsigned char TM1637_KeyProcess( void );
void TM1637_CLD( void );
void dp_flicker(void);
void show_time(unsigned char p1,unsigned char p2,unsigned char p3,unsigned char p4);
void TM1637_show_num(uint16_t num);
void show_second(unsigned char o,unsigned char *p,unsigned char *u,unsigned char *l);

#endif
