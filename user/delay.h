#ifndef  __DELAY_H__
#define  __DELAY_H__

//#include "stm8l15x.h"

void delay_init(unsigned char clk);	//延时函数初始化
void delay_us(unsigned short nus);       //us级延时函数,最大65536us.
void delay_ms(unsigned long nms);	//ms级延时函数
#endif
