#include "reg52.h"  
#define uchar unsigned char
#define uint  unsigned int
sbit LE1=P2^0;           //λѡ573������ʹ��
sbit LE2=P2^1;           //��ѡ573������ʹ��
extern uchar code dis[17];
 void display(void);
extern void delay(uint time);