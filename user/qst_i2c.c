
#include "stm8s.h"
#include "qst_i2c.h"


#if defined(QST_SW_IIC)
#define GPIO_PORT_I2C		GPIOB
#define I2C_SCL_PIN			GPIO_PIN_4			/* SCL GPIO */
#define I2C_SDA_PIN			GPIO_PIN_5			/* SDA GPIO */

#define I2C_SCL_OUTPUT()	GPIO_Init(GPIO_PORT_I2C, I2C_SCL_PIN, GPIO_MODE_OUT_PP_HIGH_FAST)	
#define I2C_SCL_INPUT()			
#define I2C_SDA_OUTPUT()	GPIO_Init(GPIO_PORT_I2C, I2C_SDA_PIN, GPIO_MODE_OUT_OD_HIZ_FAST)		
#define I2C_SDA_INPUT()		GPIO_Init(GPIO_PORT_I2C, I2C_SDA_PIN, GPIO_MODE_IN_PU_NO_IT)	
#define I2C_SCL_1()  		GPIO_WriteHigh(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 1 */
#define I2C_SCL_0()  		GPIO_WriteLow(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 0 */
#define I2C_SDA_1()  		GPIO_WriteHigh(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 1 */
#define I2C_SDA_0()  		GPIO_WriteLow(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 0 */
#define I2C_SDA_READ()  	GPIO_ReadInputPin(GPIO_PORT_I2C, I2C_SDA_PIN)	/* Read SDA */


void i2c_Ack(void);
void i2c_NAck(void);

/*
*********************************************************************************************************
*	? ? ?: i2c_Delay
*	????: I2C?????,??400KHz
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
}

/*
*********************************************************************************************************
*	? ? ?: i2c_Start
*	????: CPU??I2C??????
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* ?SCL????,SDA?????????I2C?????? */
	
	I2C_SCL_OUTPUT();
	I2C_SDA_OUTPUT();
	
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	? ? ?: i2c_Start
*	????: CPU??I2C??????
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	I2C_SCL_OUTPUT();
	I2C_SDA_OUTPUT();

	/* ?SCL????,SDA?????????I2C?????? */
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

/*
*********************************************************************************************************
*	? ? ?: i2c_SendByte
*	????: CPU?I2C??????8bit??
*	?    ?:_ucByte : ???????
*	? ? ?: ?
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* ????????bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		//i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();	
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // ????
		}
		_ucByte <<= 1;	/* ????bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	? ? ?: i2c_ReadByte
*	????: CPU?I2C??????8bit??
*	?    ?:?
*	? ? ?: ?????
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(u8 ack)
{
	uint8_t i;
	uint8_t value;

	/* ???1?bit????bit7 */
	I2C_SDA_INPUT();	// set data input	
	i2c_Delay();
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		//I2C_SCL_1();
		//i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_1();
		i2c_Delay();
		I2C_SCL_0();
		i2c_Delay();
	}
	
	I2C_SDA_OUTPUT();	// set data output	
	i2c_Delay();
	if(ack==0)
		i2c_NAck();
	else
		i2c_Ack();
	return value;
}

/*
*********************************************************************************************************
*	? ? ?: i2c_WaitAck
*	????: CPU??????,??????ACK????
*	?    ?:?
*	? ? ?: ??0??????,1???????
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU??SDA?? */
	I2C_SDA_INPUT();	//set data input
	i2c_Delay();
	I2C_SCL_1();	/* CPU??SCL = 1, ???????ACK?? */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU??SDA???? */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	I2C_SDA_OUTPUT();	//set data input
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	? ? ?: i2c_Ack
*	????: CPU????ACK??
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU??SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU??1??? */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU??SDA?? */
}

/*
*********************************************************************************************************
*	? ? ?: i2c_NAck
*	????: CPU??1?NACK??
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU??SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU??1??? */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();	
}

#endif


uint8_t qst_iic_write(uint8_t slave,uint8_t Addr, uint8_t Data)
{
#if defined(QST_SW_IIC)
	i2c_Start();
	i2c_SendByte(slave);
	if(i2c_WaitAck())
	{
		return 0;
	}
	i2c_SendByte(Addr);	
	if(i2c_WaitAck())
	{
		return 0;
	}
	i2c_SendByte(Data);	
	if(i2c_WaitAck())
	{
		return 0;
	}
	i2c_Stop();

	return 1;
#else
  while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
  /* 1.��ʼ */
  I2C_GenerateSTART(ENABLE);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)); //while(!(I2C->SR1&0x01));
  /* 2.�豸��ַ/д */
  I2C_Send7bitAddress(slave, I2C_DIRECTION_TX);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); //while(!((I2C->SR1)&0x02));
  /* 3.���ݵ�ַ */
  I2C_SendData((Addr&0xFF));
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //while(!(I2C->SR1 & 0x04));
  /* 4.дһ�ֽ����� */
  I2C_SendData(Data);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //while(!(I2C->SR1 & 0x04));
  /* 5.ֹͣ */
  I2C_GenerateSTOP(ENABLE);

  return 1;
#endif
}

/************************************************
�������� �� EEPROM_WriteNByte
��    �� �� EEPROMдN�ֽ�
��    �� �� Addr ----- ��ַ
            pData ---- ����
            Length --- ����
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
uint8_t qst_iic_read(uint8_t slave, uint8_t Addr, uint8_t *pData, uint16_t Length)
{
#if defined(QST_SW_IIC)
	uint8_t i;

	i2c_Start();
	i2c_SendByte(slave);
	if(i2c_WaitAck())
	{
		return 0;
	}
	i2c_SendByte(Addr);
	if(i2c_WaitAck())
	{
		return 0;
	}

	i2c_Start();
	i2c_SendByte(slave+1);
	if(i2c_WaitAck())
	{
		return 0;
	}

	for(i=0;i<(Length-1);i++){
		*pData=i2c_ReadByte(1);
		pData++;
	}
	*pData=i2c_ReadByte(0);
	i2c_Stop();

	return 1;
#else
  uint16_t cnt;

  while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
  /* 1.��ʼ */
  I2C_GenerateSTART(ENABLE);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
  /* 2.�豸��ַ/д */
  I2C_Send7bitAddress(slave, I2C_DIRECTION_TX);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  /* 3.���ݵ�ַ */
  I2C_SendData((Addr&0xFF));
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  /* 4.���¿�ʼ */
  I2C_GenerateSTART(ENABLE);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
  /* 5.�豸��ַ/�� */
  I2C_Send7bitAddress(slave, I2C_DIRECTION_RX);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  /* 6.�����ֽ����� */
  for(cnt=0; cnt<(Length-1); cnt++)
  {
    I2C_AcknowledgeConfig(I2C_ACK_CURR);                             //����Ӧ��
    while(I2C_GetFlagStatus(I2C_FLAG_RXNOTEMPTY) == RESET);
    *pData = I2C_ReceiveData();                                      //������ȡ(Length-1)�ֽ�
    pData++;
  }
  I2C_AcknowledgeConfig(I2C_ACK_NONE);                               //��ȡ���1�ֽ�(������Ӧ��)
  while(I2C_GetFlagStatus(I2C_FLAG_RXNOTEMPTY) == RESET);
  *pData = I2C_ReceiveData();                                        //��ȡ����
  /* 7.ֹͣ */
  I2C_GenerateSTOP(ENABLE);

  return 1;
#endif
}


/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
