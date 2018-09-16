
#include "stm8s.h"
#include "qst_i2c.h"

extern void qst_printf(const char *format, ...);

void qma6981_read_xyz(int16_t raw[3])
{
	uint8_t reg_data[6];

	qst_iic_read((0x12<<1), 0x01, reg_data, 6);
 	raw[0] = (int16_t)((int16_t)(reg_data[1]<<8)|(int16_t)(reg_data[0]));
	raw[1] = (int16_t)((int16_t)(reg_data[3]<<8)|(int16_t)(reg_data[2]));
	raw[2] = (int16_t)((int16_t)(reg_data[5]<<8)|(int16_t)(reg_data[4]));

	raw[0] = raw[0]>>6;
	raw[1] = raw[1]>>6;
	raw[2] = raw[2]>>6;

	qst_printf("acc %f %f %f\n",(float)raw[0]*9.807/128.0f,(float)raw[1]*9.807/128.0f,(float)raw[2]*9.807/128.0f);
	//qst_printf("%s", "read raw data \n");
}

unsigned char qmaX981_init(void)
{
	unsigned char chip;

	qst_iic_read((0x12<<1), 0x00, &chip, 1);
	if((chip == 0xb0) || (chip == 0xe0))
	{
		qst_iic_write((0x12<<1), 0x0f, 0x02);		// lsb 128
		qst_iic_write((0x12<<1), 0x10, 0x05);
		qst_iic_write((0x12<<1), 0x11, 0x80);
	}
	else
	{
		chip = 0;
	}

	return chip;
}

