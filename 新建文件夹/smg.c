#include "smg.h"       //����ͷ�ļ�

uchar code dis[17]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                   //  0    1    2    3    4    5    6    7    
                     0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00};  //0~F�Ķ���
                   //  8    9    A    B    C    D    E    F  
char dspcom,dspbuf[8]={0,1,2,3,4,5,6,7};


void display(void)
{	 
	P0=(~(0x80>>(dspcom)));	
	LE1=1;                   //����λ
	LE1=0;                   //�Ͽ�����,λѡ573��Q7~Q0�Ա��� 

	P0=dis[dspbuf[dspcom]];
	LE2=1;                   //�������
	LE2=0;
	 ++dspcom;
	if(dspcom>=8)dspcom=0; 

}  