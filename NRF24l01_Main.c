#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
/*GPIO Include*/
#include "GPIO.h"
#include "nRF24L01.h"
struct spi_device* dev = 0;
struct nrf24l01_platform_data{
	char model[10];//250kbs 1mbps 2mbps
 };
static struct nrf24l01_platform_data nrf24l01_data= {
 	 .model = "250k",
};
static struct spi_board_info initdata = {  
	 .modalias = "Nrf24l01",  
	 .platform_data = &nrf24l01_data,  
	 .mode  = SPI_MODE_0,  
	 .irq  = IRQ_GPIO_C_START+7,  
	 .max_speed_hz = 25000000,
	 .bus_num = 0,  
	 .chip_select = 1,  
};
uint8_t SPI_ExchangeByte(uint8_t input)
{
	return spi_w8r8(dev,input);
}
static int NRF24l01_Init(void)
{
	struct spi_master *spi_mast = 0;
	GPIO_Typedef GPIO;
	spi_mast = spi_busnum_to_master(0);
	if(!spi_mast)
	{
		printk("cannot get spi_master!\n");
		return 1;
	}
	printk("spi_master Get\n");
	dev = spi_new_device(spi_mast,&initdata);
	if(dev)
	{
		dev->bits_per_word = 8;
		printk("spi_new_device OK\n");
	}
	else
	{
		printk("spi_new_device failed\n");
		return 1;
	}
	L01_Init();
	/****** GPIO IRQ初始化 ******/
	GPIO.Pin = GPIO_Pin_8;
	GPIO.Mode = Output;
	GPIO.Pull = Pull_Up;
	GPIO_Init(GPIOC_BASE,&GPIO);
	GPIO_ResetBit(GPIOC_BASE,GPIO_Pin_8);
	printk("NRF24l01 Is Online\n");
	return 0;
}
static void NRF24l01_Exit(void)
{
	printk("NRF24l01 is Offline\n");
	GPIO_ResetBit(GPIOC_BASE,GPIO_Pin_8);
	GPIO_Free(GPIOC_BASE,GPIO_Pin_8);
	if(dev)
	{
		spi_unregister_device(dev);
	}
}
MODULE_LICENSE("Dual BSD/GPL");
module_init(NRF24l01_Init);
module_exit(NRF24l01_Exit);
