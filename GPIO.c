#include <linux/gpio.h>
#include <asm/io.h>
#include "GPIO.h"
volatile unsigned * GPIOCOUT;
volatile unsigned * GPIOCALTFN0;
volatile unsigned * PIOCOUTENB;
unsigned PHYToPad_Base(GPIO_BASE Base)
{
	switch(Base)
	{
		case PHY_BASEADDR_GPIOA:
		return PAD_GPIO_A;
		break;
		case PHY_BASEADDR_GPIOB:
		return PAD_GPIO_B;
		break;
		case PHY_BASEADDR_GPIOC:
		return PAD_GPIO_C;
		break;
		case PHY_BASEADDR_GPIOD:
		return PAD_GPIO_D;
		break;
		case PHY_BASEADDR_GPIOE:
		return PAD_GPIO_E;
		break;
		default:
		printk("Unknown Base Addr\n");
		return 0;
		break;
	}
	return 0;
}
unsigned PHYToPad_Pin(GPIO_Pin Pin)//one pin
{
	unsigned i = 0;
	for(i = 0;i<32;i++)
	{
		if(Pin&(1<<i))
		{
			return i;
		}
	}
	return 0;
}
GPIO_BASE PadToPHY_Base(unsigned Base)
{
	switch(Base)
	{
		case PAD_GPIO_A:
				return PHY_BASEADDR_GPIOA;
				break;
		case PAD_GPIO_B:
				return PHY_BASEADDR_GPIOB;
				break;
		case PAD_GPIO_C:
				return PHY_BASEADDR_GPIOC;
				break;
		case PAD_GPIO_D:
				return PHY_BASEADDR_GPIOD;
				break;
		case PAD_GPIO_E:
				return PHY_BASEADDR_GPIOE;
				break;
		default:
			printk("Unknown Base Addr\n");
			return 0;
			break;
	}
	return 0;
}
GPIO_Pin PadToPHY_Pin(unsigned Pin)
{
	return 1<<Pin;
}
void GPIO_SetBit(GPIO_BASE Base,GPIO_Pin Pin)
{
	 GPIOCOUT = ioremap(Base, 4);
	 *GPIOCOUT = Pin;
	 iounmap(GPIOCOUT);
}
void GPIO_ResetBit(GPIO_BASE Base,GPIO_Pin Pin)
{
	GPIOCOUT = ioremap(Base, 4);
	*GPIOCOUT &= (~Pin);
	iounmap(GPIOCOUT);
}
void GPIO_Init(GPIO_BASE Base,GPIO_Typedef* Init)//物理地址多pin初始化
{
	char GPIO_N = 0;
	unsigned i = 0;
	for(i = 0;i<32;i++)
	{
		if(Init->Pin&(1<<i))
		{
			int ret = gpio_request(PHYToPad_Base(Base) + i,"USR_OC_GPIO");
			if(ret<0)
			{
				printk("GPIO Request failed! %d\n",ret);
				return ;
			}
			if(Init->Mode == 0)//in
			    gpio_direction_input(PHYToPad_Base(Base) + i);
			else if(Init->Mode == 1)//out
			    gpio_direction_output(PHYToPad_Base(Base) + i , Init->Pull);
		}
	}
	switch(Base)
	{
		case PHY_BASEADDR_GPIOA:
				GPIO_N = 'A';
				break;
		case PHY_BASEADDR_GPIOB:
				GPIO_N = 'B';
				break;
		case PHY_BASEADDR_GPIOC:
				GPIO_N = 'C';
				break;
		case PHY_BASEADDR_GPIOD:
				GPIO_N = 'D';
				break;
		case PHY_BASEADDR_GPIOE:
				GPIO_N = 'E';
				break;
		default:
			printk("Unknown Base Addr\n");
			return;
			break;
	}
	printk("GPIO%c is Initial\n",GPIO_N);
}
void GPIO_Free(GPIO_BASE Base,GPIO_Pin Pin)
{
	unsigned Pad = 0 , i = 0;
	Pad = PHYToPad_Base(Base);
	for(i = 0;i<32;i++)
	{
		if(Pin&(1<<i))
		{
			gpio_free(Pad + Pin);
		}
	}
}
void Set_GPIO_IRQ(GPIO_BASE Base,GPIO_Pin Pin,irq_handler_t handler,unsigned long flags, const char *name)
{
	if(request_irq(gpio_to_irq(PHYToPad_Base(Base)+PHYToPad_Pin(Pin)),handler,flags,name,NULL)<0)
	{
		printk("Set IRQ Failed!");
	}
}
