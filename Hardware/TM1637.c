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
    0x00,/*21 ����*/
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

//��ʼλ CLKΪ�ߵ�ƽʱ��DIO�ɸ߱��
void TM1637_Start( void )
{
	W_SCL(1);
	W_SDA(1);
	W_SDA(0);
}

//ֹͣλ CLKΪ�ߵ�ƽʱ��DIO�ɵͱ��
void TM1637_Stop( void )
{
	W_SCL(0);
	W_SDA(0);
    W_SCL(1);
	W_SDA(1);
}

//�ȴ�Ӧ�� ����������ȷʱ���ڵڰ˸�ʱ���½��أ�оƬ�ڲ������һ��ACK�źţ���DIO�ܽ����ͣ��ڵھŸ�ʱ�ӽ���֮���ͷ�DIO���ߡ�
void TM1637_Ack( void )
{
	W_SCL(0);//�����½���
    delay_us( 3 );													//�ڵڰ˸�ʱ���½���֮����ʱ 5us����ʼ�ж� ACK �ź�
    while( R_SDA() );								//�ȴ�Ӧ��λ  ��һ�д���Ҳ���Բ�Ҫ ��Ӱ��ʵ��ʹ��Ч�� ��ʹ����������ʱ����Ҫ���������룬�������ͻῨ�����
    W_SCL(1);
	W_SCL(0);
}

//����������CLK�ĵ͵�ƽ�仯����CLK�ĸߵ�ƽ�����䡣
//ÿ����һ���ֽڣ�оƬ�ڲ��ڵڰ˸�ʱ���½��ز���һ��ACK
// 1637дһ���ֽ�
void TM1637_WriteByte( unsigned char Byte )
{
    unsigned char i;
    for( i = 0; i < 8; i++ )
    {
		W_SCL(0);
        if( Byte & 0x01 )									//��λ��ǰ
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
//д��ʾ�Ĵ���  ��ַ����
void TM1637_Display_INC( void )
{
    unsigned char i;
    TM1637_Start();
    TM1637_WriteByte( 0x40 );									//д���ݵ���ʾ�Ĵ��� 40H ��ַ�Զ���1 ģʽ,44H �̶���ַģʽ,����������Լ�1ģʽ
    TM1637_Ack();
    TM1637_Stop();
	
    TM1637_Start();
    TM1637_WriteByte( 0xC0 );									//��ַ�������� ��ʾ��ַ 00H
    TM1637_Ack();

    for( i = 1; i < 6; i++ )									//��ַ�Լӣ�����ÿ�ζ�д��ַ
    {
		if(dpFlag)
		{
			TM1637_WriteByte( tab[i]| 0x80 );			//��������   
			TM1637_Ack();
		}
		else
		{
			TM1637_WriteByte( tab[i] );			//��������   
			TM1637_Ack();
		}
        
    }
    TM1637_Stop();
	
    TM1637_Start();
    TM1637_WriteByte(0x88 | 0x07 );					//����ʾ���������-----���������ȿ���0---7  ������14/16
    TM1637_Ack();
    TM1637_Stop();
}

//void TM1637_writeCammand(unsigned char mData)
//{
//	TM1637_start();
//	TM1637_write1Byte(mData);  //����
//	TM1637_stop();	
//}

//д��ʾ�Ĵ���  ��ַ������
// add ����ܵĵ�ַ 0--5
// value Ҫ��ʾ������
void TM1637_Display_NoINC( unsigned char add, unsigned char value )
{
   
    TM1637_Start();
    TM1637_WriteByte( 0x44 );								//д���ݵ���ʾ�Ĵ��� 40H ��ַ�Զ���1 ģʽ,44H �̶���ַģʽ,����������Լ�1ģʽ
    TM1637_Ack();
    TM1637_Stop();

    TM1637_Start();
    TM1637_WriteByte( 0xC0 | (add-1) );					//��ַ�������� ��ʾ��ַ C0H---C5H
    TM1637_Ack();
	if(dpFlag)
		{
			TM1637_WriteByte( tab[value]| 0x80 );			//��������    value�洢Ҫ��ʾ������
			TM1637_Ack();
		}
		else
		{
			TM1637_WriteByte( tab[value]);			//��������   
			TM1637_Ack();
		}
        
    TM1637_Stop();

    TM1637_Start();
    TM1637_WriteByte( 0x88 | 0x07 );							//����ʾ���������-----���������ȿ���0---7  ������14/16
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
//	a[3]=num%10;//  ��λ        	4
//	a[2]=num/10%10;//ʮλ 		 	3
//	a[1]=num/10/10%10;//��λ 	  	2
//	a[0]=num/10/10/10%10;//ǧλ		1
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
    TM1637_WriteByte( 0x88 | level );							//����ʾ���������-----���������ȿ���0---7  ������14/16
    TM1637_Ack();
    TM1637_Stop();
}

void TM1637_CLD( void )//����ʾ
{
    TM1637_Start();
    TM1637_WriteByte( 0x80 );							//����ʾ���������-----���������ȿ���0---7  ������14/16
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



