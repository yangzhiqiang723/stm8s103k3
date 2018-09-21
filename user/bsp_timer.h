/**
  **********************************  STM8S  ***********************************
  * @文件名     ： bsp_timer.h
  * @作者       ： strongerHuang
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年04月10日
  * @摘要       ： TIM定时器头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm8s.h"


/* 宏定义 --------------------------------------------------------------------*/

/* 全局变量 ------------------------------------------------------------------*/
extern uint16_t gTIMTiming_Num;                            //定时计数
extern uint8_t  gTIMTiming_Flag;                           //定时标志(0-无效 1-有效)

/* 函数申明 ------------------------------------------------------------------*/
void TIMER_Initializes(void);

void TIMTiming_Nms(uint16_t Times);
void TIMTiming_Off(void);


#endif /* _BSP_TIMER_H */

/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/

