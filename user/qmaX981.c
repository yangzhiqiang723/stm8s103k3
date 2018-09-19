
#include "stm8s.h"
#include "qst_i2c.h"

#if defined(QST_CONFIG_QMAX981)
//#define STEPCOUNTER_SUPPORT
extern void qst_printf(const char *format, ...);

void qma6981_read_xyz(void)
{
	uint8_t reg_data[6];
	int16_t raw[3];
#if defined(STEPCOUNTER_SUPPORT)
	uint16_t step = 0;
#endif

	qst_iic_read((0x12<<1), 0x01, reg_data, 6);
 	raw[0] = (int16_t)((int16_t)(reg_data[1]<<8)|(int16_t)(reg_data[0]));
	raw[1] = (int16_t)((int16_t)(reg_data[3]<<8)|(int16_t)(reg_data[2]));
	raw[2] = (int16_t)((int16_t)(reg_data[5]<<8)|(int16_t)(reg_data[4]));

	raw[0] = raw[0]>>6;
	raw[1] = raw[1]>>6;
	raw[2] = raw[2]>>6;

#if defined(STEPCOUNTER_SUPPORT)
	qst_iic_read((0x12<<1), 0x07, reg_data, 2);
	step = (((uint16_t)reg_data[1]<<8)|reg_data[0]);
#endif

#if defined(STEPCOUNTER_SUPPORT)
	qst_printf("acc %f %f %f %d\n",(float)raw[0]*9.807/64.0f,(float)raw[1]*9.807/64.0f,(float)raw[2]*9.807/64.0f, step);
#else
	qst_printf("acc %f %f %f %d\n",(float)raw[0]*9.807/128.0f,(float)raw[1]*9.807/128.0f,(float)raw[2]*9.807/128.0f, 0);
#endif
	//qst_printf("%s", "read raw data \n");
}

uint8_t qmaX981_init(void)
{
	uint8_t chip;

	qst_iic_read((0x12<<1), 0x00, &chip, 1);
	if(chip == 0xb0)
	{
		qst_iic_write((0x12<<1), 0x0f, 0x02);		// lsb 128
		qst_iic_write((0x12<<1), 0x10, 0x05);
		qst_iic_write((0x12<<1), 0x11, 0x80);
#if defined(STEPCOUNTER_SUPPORT)
		qst_iic_write((0x12<<1), 0x0f, 0x04);
		qst_iic_write((0x12<<1), 0x11, 0x80);
		qst_iic_write((0x12<<1), 0x10, 0x2a);
		qst_iic_write((0x12<<1), 0x12, 0x8f);
		qst_iic_write((0x12<<1), 0x13, 0x10);
		qst_iic_write((0x12<<1), 0x14, 0x14);
		qst_iic_write((0x12<<1), 0x15, 0x10);
		
		qst_iic_write((0x12<<1), 0x16, 0x0c);
#endif
	}
	else if(chip == 0xe0)
	{
	}
	else
	{
		chip = 0;
	}

	return chip;
}
#endif

