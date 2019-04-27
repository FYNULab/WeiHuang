#include "reg52.h"  
#define uchar unsigned char
#define uint  unsigned int
sbit LE1=P2^0;           //位选573锁存器使能
sbit LE2=P2^1;           //段选573锁存器使能
extern uchar code dis[17];
 void display(void);
extern void delay(uint time);