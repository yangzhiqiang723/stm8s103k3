/**
  **********************************  STM8S  ***********************************
  * @文件名     ： bsp_eeprom.h
  * @作者       ： strongerHuang
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年05月18日
  * @摘要       ： 内部EEPROM头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _BSP_EEPROM_H
#define _BSP_EEPROM_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm8s.h"


/* 宏定义 --------------------------------------------------------------------*/

/* 函数申明 ------------------------------------------------------------------*/
void EEPROM_Initializes(void);
void EEPROM_WriteNByte(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte);
void EEPROM_ReadNByte(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t nByte);


#endif /* _BSP_EEPROM_H */

/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
