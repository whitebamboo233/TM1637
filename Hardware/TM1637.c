#include "TM1637.h"
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
//SCL PB6
//SDA PB7
unsigned char dpFlag=0;

unsigned char tab[] =
{
    0x3F,/*0*/
    0x06,/*1*/
    0x5B,/*2*/
    0x4F,/*3*/
    0x66,/*4*/
    0x6D,/*5*/
    0x7D,/*6*/
    0x07,/*7*/
    0x7F,/*8*/
    0x6F,/*9*/
    0x77,/*10 A*/
    0x7C,/*11 b*/
    0x58,/*12 c*/
    0x5E,/*13 d*/
    0x79,/*14 E*/
    0x71,/*15 F*/
    0x76,/*16 H*/
    0x38,/*17 L*/
    0x54,/*18 n*/
    0x73,/*19 P*/
    0x3E,/*20 U*/
    0x00,/*21 黑屏*/
};


void TM1637_Init( void )
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( TM1637_CLK_GPIO_CLK | TM1637_DIO_GPIO_CLK, ENABLE );

    GPIO_InitStructure.GPIO_Pin = TM1637_CLK_GPIO_PIN | TM1637_DIO_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( TM1637_CLK_GPIO_PORT, &GPIO_InitStructure );
}


void W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_6,(BitAction)BitValue);
	delay_us(2);
}

void W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_7,(BitAction)BitValue);
	delay_us(2);
}

uint8_t R_SDA(void)
{
	uint8_t BitValue;
	BitValue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
	delay_us(2);
	return BitValue;
}

//起始位 CLK为高电平时，DIO由高变低
void TM1637_Start( void )
{
	W_SCL(1);
	W_SDA(1);
	W_SDA(0);
}

//停止位 CLK为高电平时，DIO由低变高
void TM1637_Stop( void )
{
	W_SCL(0);
	W_SDA(0);
    W_SCL(1);
	W_SDA(1);
}

//等待应答 传输数据正确时，在第八个时钟下降沿，芯片内部会产生一个ACK信号，将DIO管脚拉低，在第九个时钟结束之后释放DIO总线。
void TM1637_Ack( void )
{
	W_SCL(0);//产生下降沿
    delay_us( 3 );													//在第八个时钟下降沿之后延时 5us，开始判断 ACK 信号
    while( R_SDA() );								//等待应答位  这一行代码也可以不要 不影响实际使用效果 在使用软件仿真的时候需要屏蔽这句代码，否则程序就会卡在这里。
    W_SCL(1);
	W_SCL(0);
}

//输入数据在CLK的低电平变化，在CLK的高电平被传输。
//每传输一个字节，芯片内部在第八个时钟下降沿产生一个ACK
// 1637写一个字节
void TM1637_WriteByte( unsigned char Byte )
{
    unsigned char i;
    for( i = 0; i < 8; i++ )
    {
		W_SCL(0);
        if( Byte & 0x01 )									//低位在前
        {
            W_SDA(1);
        }
        else
        {
           W_SDA(0);
        }
        delay_us( 3 );
        Byte = Byte >> 1;
        W_SCL(1);
        delay_us( 3 );
    }
}
//写显示寄存器  地址自增
void TM1637_Display_INC( void )
{
    unsigned char i;
    TM1637_Start();
    TM1637_WriteByte( 0x40 );									//写数据到显示寄存器 40H 地址自动加1 模式,44H 固定地址模式,本程序采用自加1模式
    TM1637_Ack();
    TM1637_Stop();
	
    TM1637_Start();
    TM1637_WriteByte( 0xC0 );									//地址命令设置 显示地址 00H
    TM1637_Ack();

    for( i = 1; i < 6; i++ )									//地址自加，不必每次都写地址
    {
		if(dpFlag)
		{
			TM1637_WriteByte( tab[i]| 0x80 );			//发送数据   
			TM1637_Ack();
		}
		else
		{
			TM1637_WriteByte( tab[i] );			//发送数据   
			TM1637_Ack();
		}
        
    }
    TM1637_Stop();
	
    TM1637_Start();
    TM1637_WriteByte(0x88 | 0x07 );					//开显示，最大亮度-----调节脉冲宽度控制0---7  脉冲宽度14/16
    TM1637_Ack();
    TM1637_Stop();
}

//void TM1637_writeCammand(unsigned char mData)
//{
//	TM1637_start();
//	TM1637_write1Byte(mData);  //数据
//	TM1637_stop();	
//}

//写显示寄存器  地址不自增
// add 数码管的地址 0--5
// value 要显示的内容
void TM1637_Display_NoINC( unsigned char add, unsigned char value )
{
   
    TM1637_Start();
    TM1637_WriteByte( 0x44 );								//写数据到显示寄存器 40H 地址自动加1 模式,44H 固定地址模式,本程序采用自加1模式
    TM1637_Ack();
    TM1637_Stop();

    TM1637_Start();
    TM1637_WriteByte( 0xC0 | (add-1) );					//地址命令设置 显示地址 C0H---C5H
    TM1637_Ack();
	if(dpFlag)
		{
			TM1637_WriteByte( tab[value]| 0x80 );			//发送数据    value存储要显示的内容
			TM1637_Ack();
		}
		else
		{
			TM1637_WriteByte( tab[value]);			//发送数据   
			TM1637_Ack();
		}
        
    TM1637_Stop();

    TM1637_Start();
    TM1637_WriteByte( 0x88 | 0x07 );							//开显示，最大亮度-----调节脉冲宽度控制0---7  脉冲宽度14/16
    TM1637_Ack();
    TM1637_Stop();

}

void TM1637_show_num(uint16_t num)
{
	unsigned char a[4]={0};
	unsigned char i=3;
	while(num>0)
	{
		a[i]=num%10;
		i--;
		num/=10;
	}
//	a[3]=num%10;//  个位        	4
//	a[2]=num/10%10;//十位 		 	3
//	a[1]=num/10/10%10;//百位 	  	2
//	a[0]=num/10/10/10%10;//千位		1
	for(i=1;i<5;i++)
	{
		TM1637_Display_NoINC( i,a[i-1]);
	}
}
unsigned char k=0;
void show_time(unsigned char p1,unsigned char p2,unsigned char p3,unsigned char p4)
{
	
	unsigned char i;
//	unsigned char o=p3,p=p2,l=p1;
//	TM1637_Display_NoINC( 1,l );
//	TM1637_Display_NoINC( 2,p );
//	TM1637_Display_NoINC( 3,o );
//	TM1637_Display_NoINC( 4,p4 );
	
	if(k==0)
	{
		for(i=p4;i<=9;i++)
		{	
		TM1637_Display_NoINC( 4,i );
//		delay_s(1);
			delay_ms(300);
		}
	}
	else
	{
		for(i=0;i<=9;i++)
		{	
		TM1637_Display_NoINC( 4,i );
//		delay_s(1);		
		delay_ms(300);	
		}
		
	}
	if(i==10)
	{
		i=0;
		k=1;
		p3++;
		TM1637_Display_NoINC( 3,p3 );
	}
//	if(o==10)	
//	{
//		o=0;
//		p++;
//		TM1637_Display_NoINC( 2,p );
//	}
//	if(p==10)	
//	{
//		p=0;
//		l++;
//		TM1637_Display_NoINC( 2,l );
//	}
//	if(p==10)
//	{
//		l=0;
//	}
	
}

void show_second(unsigned char o,unsigned char *p,unsigned char *u,unsigned char *l)
{
	unsigned char i;

	if(k==0)
	{
		TM1637_show_num((*l)*1000+(*u)*100+(*p)*10+o);
		for(i=o;i<=9;i++)
		{	
			TM1637_Display_NoINC( 4,i );
			delay_s(1);
//			delay_ms(100);
		}
	}
	else
	{
		for(i=0;i<=9;i++)
		{	
			TM1637_Display_NoINC( 4,i );
			delay_s(1);		
//		delay_ms(100);	
		}
	}
	if(i==10)
	{
		i=0;
		TM1637_Display_NoINC( 4,i );
		k=1;
		(*p)++;
		TM1637_Display_NoINC( 3,*p );
	}

	if((*p)==6)
	{
		(*p)=0;
		(*u)++;
		if((*u)!=10)
		{
			TM1637_Display_NoINC( 2,*u );
			
		}
		TM1637_Display_NoINC( 3,*p );
	}
	
	if((*u)==6)
	{
		(*u)=0;
		(*l)++;
		TM1637_Display_NoINC( 2,*u );
		TM1637_Display_NoINC( 1,*l );
	}
	
	if((*l)==6)
	{
		(*l)=0;
		TM1637_Display_NoINC( 1,*l );
	}
}

////void TM1637_countdown(unsigned char o,unsigned char *p,unsigned char *u,unsigned char *l)
////{
////	signed	char i;

////	if(k==0)
////	{
////		TM1637_show_num((*l)*1000+(*u)*100+(*p)*10+o);
////		for(i=o;i>=0;i--)
////		{	
////			TM1637_Display_NoINC( 4,i );
//////			delay_s(1);
////			delay_ms(100);
////		}
////	}
////	else
////	{
////		for(i=0;i<=9;i++)
////		{	
////			TM1637_Display_NoINC( 4,i );
//////			delay_s(1);		
////		delay_ms(100);	
////		}
////	}
////	if(i==-1)
////	{
////		i=9;
////		TM1637_Display_NoINC( 4,i );
////		k=1;
////		(*p)--;
////		TM1637_Display_NoINC( 3,*p );
////	}

////	if((*p)==0)
////	{
////		(*p)=0;
////		(*u)++;
////		if((*u)!=10)
////		{
////			TM1637_Display_NoINC( 2,*u );
////			
////		}
////		TM1637_Display_NoINC( 3,*p );
////	}
////	
////	if((*u)==10)
////	{
////		(*u)=0;
////		(*l)++;
////		TM1637_Display_NoINC( 2,*u );
////		TM1637_Display_NoINC( 1,*l );
////	}
////	
////	if((*l)==10)
////	{
////		(*l)=0;
////		TM1637_Display_NoINC( 1,*l );
////	}
////}
////}
void TM1637_SetBrightness( unsigned char level )
{
    TM1637_Start();
    TM1637_WriteByte( 0x88 | level );							//开显示，最大亮度-----调节脉冲宽度控制0---7  脉冲宽度14/16
    TM1637_Ack();
    TM1637_Stop();
}

void TM1637_CLD( void )//关显示
{
    TM1637_Start();
    TM1637_WriteByte( 0x80 );							//开显示，最大亮度-----调节脉冲宽度控制0---7  脉冲宽度14/16
    TM1637_Ack();
    TM1637_Stop();
}

void dp_flicker(void)
{
//	dpFlag=dpFlag%1;
//	delay_s(1);
	
	if(dpFlag==1)
		dpFlag=0;
	else
		dpFlag=1;

	delay_ms(500);
}



