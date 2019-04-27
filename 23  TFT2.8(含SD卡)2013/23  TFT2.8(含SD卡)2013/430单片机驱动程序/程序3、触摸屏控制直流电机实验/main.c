//��˾��������о�Ƽ��������޹�˾
//��ַ��www.hrbnbc.com
/*===================================================================================================  
�������ƣ�  Ex3 TP_MOT
����������  ��������ֱ���������
Ӳ�����ӣ�  �鿴���޸Ľӿڶ�����NBCTFT.C��TP.C��,����ϸ���ӿ����ߺ��ϵ���ԡ�
      ----------------------------------------
     |     --------TFT���ƽ���----------      |
     |	                                      |
     |	        D10~D17   ��   P4             |
     |          RS        ��   P35	      |
     |		RW   	  ��   P34            |
     |		RD        ��   P33            |
     |		CS        ��   P32	      |
     |		RES       ��   P31            |
     |	                                      |
     |		LE        ��   P30            |
     |                                        |
     |     --------��Դ�������---------      |
     |	        GND       ��   ��Դ����       |
     |	        VIN       ��   ��Դ����(5V)   |
     |----------------------------------------|
     | �����NBC��������ʹ�ã������߲����÷�  |
     | �����ӣ�ֱ�Ӳ���ڿ������ϵ�12864�ӿ�  |
     | ��ֻ���������½���                     |
     |----------------------------------------|
     |     --------�������ƽ���---------      |
     |          PEN       ��   P12	      |
     |		TPDO   	  ��   P13            |
     |		BUSY      ��   P14            |
     |		TPDI      ��   P15	      |
     |		TPCS      ��   P16	      |
     |	        CLK       ��   P17	      |
     |     --------ֱ��������ƽ���---------  |
     |          J18_DCMT  ��   J8_0(P10)      |
      ----------------------------------------
ά����¼��  2012-3-14
====================================================================================================*/

//******************����ͷ�ļ�***************************
#include "io430.h"
#include "nbc430.h"
#include"NBCTFT.h"   //����TFT����ͷ�ļ�
#include "TP.H"
#include <in430.h>    //���ñ��������������ͷ�ļ�

//**************�������ƶ˿ڶ���********************																				   
#define Penirq  P12          //����������ʱ��ADS7843Penirq������δ����ʱ�ĸߵ�ƽ��Ϊ�͵�ƽ
#define Dcout   P10          //����������IO

//******************ȫ�ֱ���***************************
unsigned int Device_code;

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

unsigned char temp,speed,num;



//***********************************************************************************
//��ʱ����ʼ��
//***********************************************************************************
TimerA_init()
{
     //��ʱ��A����
     CCTL0 = CCIE;                  //���ò���/�ȽϿ��ƼĴ�����CCIEλΪ1��CCR0����/�ȽϹ����ж�Ϊ����
     CCR0 = 4;                      //ʵ��4x(1/4096)=0.001S��Ķ�ʱ,����/�ȽϿ��ƼĴ���CCR0��ֵΪ4��
     TACTL = TASSEL_1+ID_3 + MC_1;  //ʱ��Դѡ��ΪACLK(32768HZ)��8��Ƶ��������ģʽ(0-CCR0���),32768/8=4096 HZ.
     _EINT();                       //�����ж�
}
//***********************************************************************************
//�жϷ����ӳ���
//***********************************************************************************
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A0 (void)
{
	_DINT();     	        //�����ж�	
	if(num==speed)Dcout=0;  //��������ж�speed��ʱ��Dcout�õͣ�ռ�ձ�=speed/20,speedԽ��,��ת��Խ��
         num++;                 //������1
         if(num==20)            //��20��Ϊһ���ڣ���T=20��*1ms=20ms
          {
           num=0;               //����numΪ0,��ʼ��һ����
           Dcout=1;             //Dcout�ø�
          }
        _EINT();                //�����ж�
}

//**************************************************************************************************
//TFT�����ʼ��
//**************************************************************************************************
void init_interface()
{
    CLR_Screen(Blue);                                 //�ñ���ɫ����

    Show_RGB(14,46,272,304,Magenta);                  //0
    LCD_PutString(28,280,"0",Black,Magenta);

    Show_RGB(74,166,272,304,Magenta);                 //Enter
    LCD_PutString(100,280,"Enter",Black,Magenta);


    Show_RGB(14,46,208,240,Magenta);                  //1
    LCD_PutString(28,216,"1",Black,Magenta);

    Show_RGB(14,46,144,176,Magenta);                  //4
    LCD_PutString(28,152,"4",Black,Magenta);

    Show_RGB(14,46,80,112,Magenta);                   //7
    LCD_PutString(28,88,"7",Black,Magenta);



    Show_RGB(74,106,208,240,Magenta);                 //2
    LCD_PutString(88,216,"2",Black,Magenta);

    Show_RGB(74,106,144,176,Magenta);                 //5
    LCD_PutString(88,152,"5",Black,Magenta);

    Show_RGB(74,106,80,112,Magenta);                  //8
    LCD_PutString(88,88,"8",Black,Magenta);



    Show_RGB(134,166,208,240,Magenta);                //3
    LCD_PutString(148,216,"3",Black,Magenta);

    Show_RGB(134,166,144,176,Magenta);                //6
    LCD_PutString(148,152,"6",Black,Magenta);

    Show_RGB(134,166,80,112,Magenta);                 //9
    LCD_PutString(148,88,"9",Black,Magenta);



    Show_RGB(194,226,80,176,Magenta);                 //+
    LCD_PutString(206,120,"+",Black,Magenta);

    Show_RGB(194,226,208,304,Magenta);                //-
    LCD_PutString(206,248,"-",Black,Magenta);



    LCD_PutString(44,16,"NBC���������ʾϵͳ",White,Blue);
    LCD_PutString(32,40,"The speed now is : ___",White,Blue);

                             
    LCD_PutChar(192, 38, 0x30+speed, Red,Blue);       //��ʾ�����ٶȵȼ�ֵ
} 

//**************************************************************************************************
//��������ɨ����ƺ���
//**************************************************************************************************
void key_scan()
{
    if((lx>14)&&(lx<46)&&(ly>272)&&(ly<304))           //����0
	{
	  Show_RGB(14,46,272,304,Red);                     //���²��ֱ�ɫͻ����ʾ
	  LCD_PutString(28,280,"0",Black,Red);      
  
	  temp=0;                                          //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);  //��ʾ�ٶȵȼ�ֵ

	  while((P1IN&0x04)==0x00);                                //�ȴ������ͷ�
	  Show_RGB(14,46,272,304,Magenta);                 //�ͷź�ָ���ʾ��ɫ
          LCD_PutString(28,280,"0",Black,Magenta);

	}



	if((lx>14)&&(lx<46)&&(ly>208)&&(ly<240))           //����1
	{
	  Show_RGB(14,46,208,240,Red);                     //���²��ֱ�ɫͻ����ʾ
          LCD_PutString(28,216,"1",Black,Red);
  
	  temp=1;                                          //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);  //��ʾ�ٶȵȼ�ֵ

	  while((P1IN&0x04)==0x00);                        //�ȴ������ͷ�
	  Show_RGB(14,46,208,240,Magenta);                 //�ͷź�ָ���ʾ��ɫ
          LCD_PutString(28,216,"1",Black,Magenta); 

	}


      if((lx>74)&&(lx<106)&&(ly>208)&&(ly<240))            //����2
	{
	  Show_RGB(74,106,208,240,Red);                    //���²��ֱ�ɫͻ����ʾ
          LCD_PutString(88,216,"2",Black,Red);
  
	  temp=2;                                          //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);  //��ʾ�ٶȵȼ�ֵ

	  while((P1IN&0x04)==0x00);                        //�ȴ������ͷ�
	  Show_RGB(74,106,208,240,Magenta);                //�ͷź�ָ���ʾ��ɫ
          LCD_PutString(88,216,"2",Black,Magenta); 

	}



    if((lx>134)&&(lx<166)&&(ly>208)&&(ly<240))              //����3
	{
	  Show_RGB(134,166,208,240,Red);                    //���²��ֱ�ɫͻ����ʾ
      LCD_PutString(148,216,"3",Black,Red);
  
	  temp=3;                                          //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutString(192,38,"3",Black,Magenta);         //��ʾ�ٶȵȼ�ֵ

	  while((P1IN&0x04)==0x00);                        //�ȴ������ͷ�
	  Show_RGB(134,166,208,240,Magenta);               //�ͷź�ָ���ʾ��ɫ
          LCD_PutString(148,216,"3",Black,Magenta);

	}



	if((lx>14)&&(lx<46)&&(ly>144)&&(ly<176))           //����4
	{
	  Show_RGB(14,46,144,176,Red);                     //���²��ֱ�ɫͻ����ʾ
      LCD_PutString(28,152,"4",Black,Red);
  
	  temp=4;                                          //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta); //��ʾ�ٶȵȼ�ֵ

	  while((P1IN&0x04)==0x00);                       //�ȴ������ͷ�
	  Show_RGB(14,46,144,176,Magenta);                //�ͷź�ָ���ʾ��ɫ
          LCD_PutString(28,152,"4",Black,Magenta);

	}



	if((lx>74)&&(lx<106)&&(ly>144)&&(ly<176))           //����5
	{
	  Show_RGB(74,106,144,176,Red);                     //���²��ֱ�ɫͻ����ʾ    
          LCD_PutString(88,152,"5",Black,Red);

	  temp=5;                                           //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //��ʾ�ٶȵȼ�ֵ

	  while((P1IN&0x04)==0x00);                         //�ȴ������ͷ�
	  Show_RGB(74,106,144,176,Magenta);                 //�ͷź�ָ���ʾ��ɫ                
          LCD_PutString(88,152,"5",Black,Magenta);

	}




	if((lx>134)&&(lx<166)&&(ly>144)&&(ly<176))          //����6
	{
	  Show_RGB(134,166,144,176,Red);                    //���²��ֱ�ɫͻ����ʾ    
          LCD_PutString(148,152,"6",Black,Red);

	  temp=6;                                           //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //��ʾ�ٶȵȼ�ֵ

	  while((P1IN&0x04)==0x00);                         //�ȴ������ͷ�
	  Show_RGB(134,166,144,176,Magenta);                //�ͷź�ָ���ʾ��ɫ 
          LCD_PutString(148,152,"6",Black,Magenta);

	}



	if((lx>14)&&(lx<46)&&(ly>80)&&(ly<112))             //����7
	{
	  Show_RGB(14,46,80,112,Red);                       //���²��ֱ�ɫͻ����ʾ    
          LCD_PutString(28,88,"7",Black,Red);

	  temp=7;                                           //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //��ʾ�ٶȵȼ�ֵ

	  while((P1IN&0x04)==0x00);                         //�ȴ������ͷ�
	  Show_RGB(14,46,80,112,Magenta);                   //�ͷź�ָ���ʾ��ɫ 
          LCD_PutString(28,88,"7",Black,Magenta);

	}



	if((lx>74)&&(lx<106)&&(ly>80)&&(ly<112))            //����8
	{
	  Show_RGB(74,106,80,112,Red);                      //���²��ֱ�ɫͻ����ʾ    
          LCD_PutString(88,88,"8",Black,Red);

	  temp=8;                                           //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //��ʾ�ٶȵȼ�ֵ

	  while((P1IN&0x04)==0x00);                         //�ȴ������ͷ�
	  Show_RGB(74,106,80,112,Magenta);                  //�ͷź�ָ���ʾ��ɫ 
          LCD_PutString(88,88,"8",Black,Magenta);

	}



	if((lx>134)&&(lx<166)&&(ly>80)&&(ly<112))           //����9
	{
	  Show_RGB(134,166,80,112,Red);                     //���²��ֱ�ɫͻ����ʾ    
          LCD_PutString(148,88,"9",Black,Red);

	  temp=9;                                           //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //��ʾ�ٶȵȼ�ֵ

	  while((P1IN&0x04)==0x00);                         //�ȴ������ͷ�
	  Show_RGB(134,166,80,112,Magenta);                 //�ͷź�ָ���ʾ��ɫ
          LCD_PutString(148,88,"9",Black,Magenta);

	}





	if((lx>74)&&(lx<166)&&(ly>272)&&(ly<304))           //����Enter
	{                  
	  Show_RGB(74,166,272,304,Red);                     //���²��ֱ�ɫͻ����ʾ 
          LCD_PutString(100,280,"Enter",Black,Red);

	  speed=temp;                                       //�ٶȵȼ�����speed
	  LCD_PutChar(192, 38, 0x30+speed, Red,Blue);       //��ʾ�ٶȵȼ�ֵ

	  while((P1IN&0x04)==0x00);                         //�ȴ������ͷ�
	  Show_RGB(74,166,272,304,Magenta);                 //�ͷź�ָ���ʾ��ɫ
          LCD_PutString(100,280,"Enter",Black,Magenta);

	}



	
	if((lx>194)&&(lx<226)&&(ly>80)&&(ly<176))           //����+
	{
	  Show_RGB(194,226,80,176,Red);                     //���²��ֱ�ɫͻ����ʾ    
          LCD_PutString(206,120,"+",Black,Red);
     
	  if(speed<9)speed++;                               //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+speed, Red,Blue);       //��ʾ�ٶȵȼ�ֵ

	  while((P1IN&0x04)==0x00);                         //�ȴ������ͷ�
	  Show_RGB(194,226,80,176,Magenta);                 //�ͷź�ָ���ʾ��ɫ
          LCD_PutString(206,120,"+",Black,Magenta);

	}



	if((lx>194)&&(lx<226)&&(ly>208)&&(ly<304))          //����-
	{
	  Show_RGB(194,226,208,304,Red);                    //���²��ֱ�ɫͻ����ʾ    
          LCD_PutString(206,248,"-",Black,Red);
     
	  if(speed>0)speed--;                               //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+speed, Red,Blue);       //��ʾ�ٶȵȼ�ֵ

	  while((P1IN&0x04)==0x00);                         //�ȴ������ͷ�
	  Show_RGB(194,226,208,304,Magenta);                //�ͷź�ָ���ʾ��ɫ
          LCD_PutString(206,248,"-",Black,Magenta);

	}
} 

//**************************************************************************************************
//ʱ�ӳ�ʼ��
//**************************************************************************************************
/*3��ʱ��Դ:1��DCO���ڲ�RC���� 2��XTAL1��LFXTAL1��:Ƭ��ʱ��ȷ��Ƶʱ32768Hz,����ACLK.
          3��XTAL2��Ƭ��ʱ��2����ȷ��Ƶʱ��8MHz,����MCLK��SMCLK��
���ʱ��: 1��MCLK��ϵͳ��ʱ�� 2��SMCLK����ϵͳʱ�� 3��ACLK������ʱ��
ϵͳ�ϵ�ʱ��Ĭ��ʹ��DCOʱ����ΪMCLK��SMCLK��ʹ��XTAL1��ΪACLK��XTAL2û������û��
��ʹ�á�ϵͳʱ�ӿ���ѡ��ʹ��MCK��SMCLK��ACLK���е�һ�֡���Ҫϵͳ��������������á�*/
void init_clock()
{
    uchar i;
    BCSCTL1 &= ~XT2OFF;  //����XT2��������(XT2OFF=0)
    BCSCTL2 |=SELM1+SELS;//ѡ����Ҫʱ�� MCLK:XTAL2, SMCLK:XTAL2,����Ƶ
    //������������жϱ�־��ϵͳ����ʱ���ȶ���ɵģ�����Ҫ��λ
    do
    {
        IFG1 &= ~OFIFG; //�������ʧЧ��־
        i = 255;        
        while(i--);     //��ʱ�ȴ�����50us
    }while(IFG1&OFIFG); //�ж�XT2�Ƿ�����
}

//***********************************************************************************
//IO��ʼ������
//***********************************************************************************
void IO_init(void)
{
	P4DIR=0xff;		  //����P4Ϊ���
	P4=0xff;                  //����P4��ʼ��Ϊ�ߵ�ƽ
	
	P3DIR=0xff;		  //����P3Ϊ���
	P3=0xff;                  //����P3��ʼ��Ϊ�ߵ�ƽ

        P1DIR=0xff;		  //����P1Ϊ���
	P1=0xff;                  //����P1��ʼ��Ϊ�ߵ�ƽ
}

//================================================================================================
//	�������ƣ�	������
//	ʵ�ֹ��ܣ�	��������ֱ���������
//	������		��
//	����ֵ��	��
//================================================================================================
main()
{	
        Device_code=0x9320;
        WDTCTL = WDTPW + WDTHOLD;// Stop watchdog timer to prevent time out reset  
        init_clock();
        IO_init();               //IO��ʼ�� 
        
	speed=5;

	TFT_Initial();      //tft��ʼ��
	start_7843();       //��ʼ����������IC
	TimerA_init();	    //��ʱ����ʼ��
	init_interface();   //��ʼ���˻����ƽ���
    
   	while(1)
	{
    	 if(Getpix()==1)key_scan();    //ɨ�败������ʵ�ֵ������
        }
}



