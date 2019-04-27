/***************************************************************************************************    
文件名称：	nbc430.h
功能描述：	实现430单片机IO数据向量、方向向量的位操作
维护记录：      2011-8-22
***************************************************************************************************/
#include "io430.h"
/***************************************************************************************************    
//按位定义P1
***************************************************************************************************/
//P1口输出数据
#define P1                       P1OUT 
#define P10                      P1OUT_bit.P1OUT_0
#define P11                      P1OUT_bit.P1OUT_1
#define P12                      P1OUT_bit.P1OUT_2
#define P13                      P1OUT_bit.P1OUT_3
#define P14                      P1OUT_bit.P1OUT_4
#define P15                      P1OUT_bit.P1OUT_5
#define P16                      P1OUT_bit.P1OUT_6
#define P17                      P1OUT_bit.P1OUT_7
//P1口方向属性
#define P1DIR                    P1DIR
#define P1DIR0                   P1DIR_bit.P1DIR_0
#define P1DIR1                   P1DIR_bit.P1DIR_1
#define P1DIR2                   P1DIR_bit.P1DIR_2
#define P1DIR3                   P1DIR_bit.P1DIR_3
#define P1DIR4                   P1DIR_bit.P1DIR_4
#define P1DIR5                   P1DIR_bit.P1DIR_5
#define P1DIR6                   P1DIR_bit.P1DIR_6
#define P1DIR7                   P1DIR_bit.P1DIR_7

/***************************************************************************************************    
//按位定义P2
***************************************************************************************************/
//P2口输出数据
#define P2                       P2OUT 
#define P20                      P2OUT_bit.P2OUT_0
#define P21                      P2OUT_bit.P2OUT_1
#define P22                      P2OUT_bit.P2OUT_2
#define P23                      P2OUT_bit.P2OUT_3
#define P24                      P2OUT_bit.P2OUT_4
#define P25                      P2OUT_bit.P2OUT_5
#define P26                      P2OUT_bit.P2OUT_6
#define P27                      P2OUT_bit.P2OUT_7
//P2口方向属性
#define P2DIR                    P2DIR
#define P2DIR0                   P2DIR_bit.P2DIR_0
#define P2DIR1                   P2DIR_bit.P2DIR_1
#define P2DIR2                   P2DIR_bit.P2DIR_2
#define P2DIR3                   P2DIR_bit.P2DIR_3
#define P2DIR4                   P2DIR_bit.P2DIR_4
#define P2DIR5                   P2DIR_bit.P2DIR_5
#define P2DIR6                   P2DIR_bit.P2DIR_6
#define P2DIR7                   P2DIR_bit.P2DIR_7

/***************************************************************************************************    
//按位定义P3
***************************************************************************************************/
//P3口输出数据
#define P3                       P3OUT 
#define P30                      P3OUT_bit.P3OUT_0
#define P31                      P3OUT_bit.P3OUT_1
#define P32                      P3OUT_bit.P3OUT_2
#define P33                      P3OUT_bit.P3OUT_3
#define P34                      P3OUT_bit.P3OUT_4
#define P35                      P3OUT_bit.P3OUT_5
#define P36                      P3OUT_bit.P3OUT_6
#define P37                      P3OUT_bit.P3OUT_7
//P3口方向属性
#define P3DIR                    P3DIR
#define P3DIR0                   P3DIR_bit.P3DIR_0
#define P3DIR1                   P3DIR_bit.P3DIR_1
#define P3DIR2                   P3DIR_bit.P3DIR_2
#define P3DIR3                   P3DIR_bit.P3DIR_3
#define P3DIR4                   P3DIR_bit.P3DIR_4
#define P3DIR5                   P3DIR_bit.P3DIR_5
#define P3DIR6                   P3DIR_bit.P3DIR_6
#define P3DIR7                   P3DIR_bit.P3DIR_7

/***************************************************************************************************    
//按位定义P4
***************************************************************************************************/
//P4口输出数据
#define P4                       P4OUT 
#define P40                      P4OUT_bit.P4OUT_0
#define P41                      P4OUT_bit.P4OUT_1
#define P42                      P4OUT_bit.P4OUT_2
#define P43                      P4OUT_bit.P4OUT_3
#define P44                      P4OUT_bit.P4OUT_4
#define P45                      P4OUT_bit.P4OUT_5
#define P46                      P4OUT_bit.P4OUT_6
#define P47                      P4OUT_bit.P4OUT_7
//P4口方向属性
#define P4DIR                    P4DIR
#define P4DIR0                   P4DIR_bit.P4DIR_0
#define P4DIR1                   P4DIR_bit.P4DIR_1
#define P4DIR2                   P4DIR_bit.P4DIR_2
#define P4DIR3                   P4DIR_bit.P4DIR_3
#define P4DIR4                   P4DIR_bit.P4DIR_4
#define P4DIR5                   P4DIR_bit.P4DIR_5
#define P4DIR6                   P4DIR_bit.P4DIR_6
#define P4DIR7                   P4DIR_bit.P4DIR_7

/***************************************************************************************************    
//按位定义P5
***************************************************************************************************/
//P5口输出数据
#define P5                       P5OUT 
#define P50                      P5OUT_bit.P5OUT_0
#define P51                      P5OUT_bit.P5OUT_1
#define P52                      P5OUT_bit.P5OUT_2
#define P53                      P5OUT_bit.P5OUT_3
#define P54                      P5OUT_bit.P5OUT_4
#define P55                      P5OUT_bit.P5OUT_5
#define P56                      P5OUT_bit.P5OUT_6
#define P57                      P5OUT_bit.P5OUT_7
//P5口方向属性
#define P5DIR                    P5DIR
#define P5DIR0                   P5DIR_bit.P5DIR_0
#define P5DIR1                   P5DIR_bit.P5DIR_1
#define P5DIR2                   P5DIR_bit.P5DIR_2
#define P5DIR3                   P5DIR_bit.P5DIR_3
#define P5DIR4                   P5DIR_bit.P5DIR_4
#define P5DIR5                   P5DIR_bit.P5DIR_5
#define P5DIR6                   P5DIR_bit.P5DIR_6
#define P5DIR7                   P5DIR_bit.P5DIR_7

/***************************************************************************************************    
//按位定义P6
***************************************************************************************************/
//P6口输出数据
#define P6                       P6OUT 
#define P60                      P6OUT_bit.P6OUT_0
#define P61                      P6OUT_bit.P6OUT_1
#define P62                      P6OUT_bit.P6OUT_2
#define P63                      P6OUT_bit.P6OUT_3
#define P64                      P6OUT_bit.P6OUT_4
#define P65                      P6OUT_bit.P6OUT_5
#define P66                      P6OUT_bit.P6OUT_6
#define P67                      P6OUT_bit.P6OUT_7
//P6口方向属性
#define P6DIR                    P6DIR
#define P6DIR0                   P6DIR_bit.P6DIR_0
#define P6DIR1                   P6DIR_bit.P6DIR_1
#define P6DIR2                   P6DIR_bit.P6DIR_2
#define P6DIR3                   P6DIR_bit.P6DIR_3
#define P6DIR4                   P6DIR_bit.P6DIR_4
#define P6DIR5                   P6DIR_bit.P6DIR_5
#define P6DIR6                   P6DIR_bit.P6DIR_6
#define P6DIR7                   P6DIR_bit.P6DIR_7