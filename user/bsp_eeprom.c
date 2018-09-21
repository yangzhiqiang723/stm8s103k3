/**
  **********************************  STM8S  ***********************************
  * @文件名     ： bsp_eeprom.c
  * @作者       ： strongerHuang
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年05月18日
  * @摘要       ： 内部EEPROM源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2017-05-18 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "bsp_eeprom.h"


/************************************************
函数名称 ： EEPROM_Initializes
功    能 ： 内部EEPROM初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void EEPROM_Initializes(void)
{
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
}

/************************************************
函数名称 ： EEPROM_WriteNByte
功    能 ： 写EEPROM
参    数 ： pBuffer ----- 写入数据区首地址
            WriteAddr --- 要写入Flash的地址
            nByte ------- 要写入的字节数
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void EEPROM_WriteNByte(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);

  while(FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);

  while(nByte--)
  {
    FLASH_ProgramByte(WriteAddr, *pBuffer);
    WriteAddr++;
    pBuffer++;
    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
  }
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

/************************************************
函数名称 ： EEPROM_ReadNByte
功    能 ： 读EEPROM
参    数 ： pBuffer ---- 数据buf
            ReadAddr --- 要读取Flash的首地址
            nByte ------ 要读取的字节数
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void EEPROM_ReadNByte(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t nByte)
{
  while(nByte--)
  {
    *pBuffer = FLASH_ReadByte(ReadAddr);
    ReadAddr++;
    pBuffer++;
  }
}

/************************************************
函数名称 ： EEPROM_EraseNByte
功    能 ： 连续擦除N字节
参    数 ： EraseAddr --- 要擦除的首地址
            nByte ------- 要擦除的字节数
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void EEPROM_EraseNByte(uint32_t EraseAddr, uint16_t nByte)
{
  uint16_t i;

  FLASH_Unlock(FLASH_MEMTYPE_DATA);

  while(FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);

  for(i=0; i<nByte; i++)
  {
    *(PointerAttr uint8_t*) (MemoryAddressCast)EraseAddr = 0xFF;
    EraseAddr++;
    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
  }
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}


/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
