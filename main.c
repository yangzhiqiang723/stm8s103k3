#include "stdio.h"
//#include "stdlib.h"
#include <stdarg.h>
#include "stm8s.h"
#include "qst_i2c.h"
#include "delay.h"

//#define STM_IRQ_RX_ENABLE
#define QST_UART_DEBUG

static u32 qst_run_count = 0;
uint8_t RxTxbuf[20];
uint8_t RxTxLen = 0;

enum
{
	DEVICE_ACC = 0x01,
	DEVICE_MAG = 0x02,
	DEVICE_PRESS = 0x04,
	DEVICE_GYRO = 0x08,

	DEVICE_MAX = 0xff
};

#if defined(QST_CONFIG_QMAX981)
extern uint8_t qmaX981_init(void);
extern void qma6981_read_xyz(void);
#endif
#if defined(QST_CONFIG_QMP6988)
extern uint8_t qmp6988_init(void);
extern void qma6988_calc_press(void);
#endif
#if defined(QST_CONFIG_QMCX983)
extern uint8_t qmcX983_init(void);
extern void qmcX983_read_xyz(void);
#endif



#if defined(QST_UART_DEBUG)
uint8_t itoa10(uint8_t *buf, int32_t i)
{
     uint32_t rem;
     uint8_t *s,length=0;
	 uint8_t flag=0;

     s = buf;
     if (i == 0) 
     {
     	*s++ = '0'; 
     }
     else 
     { 
          if (i < 0) 
          {
	          *buf++ = '-';
			  flag = 1;
	          s = buf;
	          i = -i;
          }
          while (i) 
          {
	          ++length;
	          rem = i % 10;
	          *s++ = rem + '0';
	          i /= 10;
          }
          for(rem=0; ((unsigned char)rem)<length/2; rem++) 
          {
	          *(buf+length) = *(buf+((unsigned char)rem));
	          *(buf+((unsigned char)rem)) = *(buf+(length-((unsigned char)rem)-1));
	          *(buf+(length-((unsigned char)rem)-1)) = *(buf+length);
          }
     }
     *s=0;

	 return ((s-buf)+flag);
}

void ftoa(uint8_t *buf, float i)
{
	int32_t i_data;
	uint8_t length=0;

	if((i>=0.000001f) && (i<=0.000001f))		
	{
	   *buf++ = '0'; 
	   *buf++ = '.'; 
	   *buf++ = '0'; 
	}
	else
	{
		if (i < 0) 
		{
			*buf++ = '-';
			i = -i;
		}
		i_data = (int32_t)i;
		length = itoa10(buf, i_data);
		buf += length;
		*buf++ = '.';
		i_data = (int32_t)((i-i_data)*10000);
		length = itoa10(buf, i_data);
		buf += length;
	}
	
	*buf=0;
}

void qst_send_string(uint8_t *str)
{
	while(*str)
	{
		while((UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET));
		UART1_SendData8(*str);
		while((UART1_GetFlagStatus(UART1_FLAG_TC)==RESET));
		str++;

	}
}
#endif

void qst_printf(const char *format, ...)
{
#if defined(QST_UART_DEBUG)
	int8_t *pc;
	int32_t value;
	float f_value;
	uint8_t buf[20];
	
	va_list arg;
	va_start(arg, format);
	//buf[0]=va_arg(arg, char);
	while (*format)
	{
		int8_t ret = *format;
		if(ret == '%')
		{
			switch (*++format)
			{
			case 'c':
			{
				buf[0] = va_arg(arg, char);
				//putchar(ch);
				buf[1] = 0;
				qst_send_string(buf);
				break;
			}
			case 's':
			{
				pc = va_arg(arg, int8_t *);
				//while (*pc)
				//{
				//	putchar(*pc);
				//	pc++;
				//}
				qst_send_string((uint8_t*)pc);
				break;
			}
			case 'd':
			{
				value =	va_arg(arg, int16_t);
				itoa10(buf, value);
				qst_send_string(buf);
				break;
			}
			case 'l':
			{
				value =	va_arg(arg, int32_t);
				itoa10(buf, value);
				qst_send_string(buf);
				break;
			}			
			case 'f':
			{
				f_value = va_arg(arg, float);
				ftoa(buf, f_value);
				qst_send_string(buf);
				break;
			}			
			default:
				break;
			}
		}
		else
		{
			//putchar(*format);
			//qst_send_string((uint8_t*)&ret);
			while((UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET));
			UART1_SendData8(ret);
			while((UART1_GetFlagStatus(UART1_FLAG_TC)==RESET));
		}
		format++;
	}
	va_end(arg);
#endif
}

void clk_config(void)
{
	CLK_DeInit(); 
	// system clock
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO,CLK_SOURCE_HSI,ENABLE,CLK_CURRENTCLOCKSTATE_ENABLE);
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	CLK_HSICmd(ENABLE);
}

void uart_config(void)
{
	// uart1
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1,ENABLE);
	UART1_DeInit();
	UART1_Init(115200,UART1_WORDLENGTH_8D,UART1_STOPBITS_1,UART1_PARITY_NO,UART1_SYNCMODE_CLOCK_DISABLE,
		UART1_MODE_TXRX_ENABLE);
	UART1_Cmd(ENABLE);
#if defined(STM_IRQ_RX_ENABLE)
	UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);
#else
	UART1_ITConfig(UART1_IT_RXNE_OR,DISABLE);
#endif
}

void i2c_config(uint16_t addr)
{
	//hardware i2c
#if !defined(QST_SW_IIC)
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C,ENABLE);	
	I2C_DeInit();
	I2C_Init(400000, addr, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, 16);
	I2C_Cmd(ENABLE);
#endif
}

void gpio_config(void)
{
#if defined(QST_SW_IIC)
	GPIO_DeInit(GPIOB);
	GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);	// clk
	GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST);	// data
#else
	GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);	// clk
	GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);	// data
#endif
	GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);	// led
	GPIO_WriteLow(GPIOD, GPIO_PIN_3);
}

void sys_init(void)
{
	clk_config();
	disableInterrupts();
	gpio_config();
	uart_config();
	i2c_config(0x12);
	delay_init(16);
	enableInterrupts();
}

void main( void )
{
	uint8_t chip_id = 0;
	uint8_t device_type = 0;

	sys_init();

	RxTxbuf[0] = 0;
	RxTxLen = 0;
#if defined(QST_CONFIG_QMAX981)
	if((chip_id=qmaX981_init())!=0)
	{
		device_type |= DEVICE_ACC;
		qst_printf("qmaX981 OK! \n");
	}
#endif
#if defined(QST_CONFIG_QMP6988)
	if((chip_id=qmp6988_init())!=0)
	{
		device_type |= DEVICE_PRESS;
		qst_printf("qmp6988 OK! \n");
	}
#endif
#if defined(QST_CONFIG_QMCX983)
	if((chip_id=qmcX983_init())!=0)
	{
		device_type |= DEVICE_MAG;
		qst_printf("qmcX983 OK! \n");
	}
#endif

	while(1)
	{
		qst_run_count++;
		//GPIO_WriteHigh(GPIOD, GPIO_PIN_3);
#if defined(QST_CONFIG_QMAX981)
		if(device_type & DEVICE_ACC)
		{
			qma6981_read_xyz();
            delay_ms(20);
		}
#endif
#if defined(QST_CONFIG_QMP6988)
		if(device_type & DEVICE_PRESS)
		{
			qma6988_calc_press();
            delay_ms(20);
		}
#endif
#if defined(QST_CONFIG_QMCX983)
		if(device_type & DEVICE_MAG)
		{
			qmcX983_read_xyz();
            delay_ms(10);
		}
#endif
		//GPIO_WriteLow(GPIOD, GPIO_PIN_3);
		delay_ms(20);
	}
}


#if defined(STM_IRQ_RX_ENABLE)
INTERRUPT_HANDLER(UART1_RX_IRQHandler, ITC_IRQ_UART1_RX)
{
	if(UART1_GetITStatus(UART1_IT_RXNE_OR) != RESET)		// interrupt flag
	{
		UART1_ClearITPendingBit(UART1_IT_RXNE_OR);
	}
	RxTxbuf[RxTxLen] = UART1_ReceiveData8();
	if(RxTxbuf[RxTxLen] == '\n')
	{
		RxTxbuf[RxTxLen] = 0;
		RxTxLen = 0;
	}
	else
	{
		RxTxLen++;
	}
	//if(UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET)
	//{
	//	RxTxLen = 0;
	//}
}
#endif

