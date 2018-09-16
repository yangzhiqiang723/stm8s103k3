
#ifndef _QST_IIC_H_
#define _QST_IIC_H_

//#define QST_SW_IIC
extern uint8_t qst_iic_write(uint8_t slave, uint8_t Addr, uint8_t Data);
extern uint8_t qst_iic_read(uint8_t slave, uint8_t Addr, uint8_t *pData, uint16_t Length);
#endif

