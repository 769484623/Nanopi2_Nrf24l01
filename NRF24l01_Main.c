#include <linux/slab.h>
#include <linux/delay.h>
/*GPIO Include*/
#include "nRF24L01.h"
struct spi_device* dev = 0;
uint8_t SPI_ExchangeByte(uint8_t input)
{
	return 0;
}
static void spi0_cs(u32 chipselect)
{
#if (CFG_SPI0_CS_GPIO_MODE)
	if (nxp_soc_gpio_get_io_func( CFG_SPI0_CS ) != nxp_soc_gpio_get_altnum( CFG_SPI0_CS ))
		nxp_soc_gpio_set_io_func( CFG_SPI0_CS, nxp_soc_gpio_get_altnum( CFG_SPI0_CS ));

	nxp_soc_gpio_set_io_dir( CFG_SPI0_CS, 1);
	nxp_soc_gpio_set_out_value( CFG_SPI0_CS , chipselect);
#else
	;
#endif
}
static struct nrf24l01_platform_data nrf24l01_data= {
 	.model = 0,
};
struct pl022_config_chip spi0_info = {
	.com_mode = CFG_SPI0_COM_MODE,/* available POLLING_TRANSFER, INTERRUPT_TRANSFER, DMA_TRANSFER */
	.iface = SSP_INTERFACE_MOTOROLA_SPI,
	/* We can only act as master but SSP_SLAVE is possible in theory */
	.hierarchy = SSP_MASTER,
	/* 0 = drive TX even as slave, 1 = do not drive TX as slave */
	.slave_tx_disable = 1,
	.rx_lev_trig = SSP_RX_4_OR_MORE_ELEM,
	.tx_lev_trig = SSP_TX_4_OR_MORE_EMPTY_LOC,
	.ctrl_len = SSP_BITS_8,
	.wait_state = SSP_MWIRE_WAIT_ZERO,
	.duplex = SSP_MICROWIRE_CHANNEL_FULL_DUPLEX,
	/*
	 * This is where you insert a call to a function to enable CS
	 * (usually GPIO) for a certain chip.
	 */
#if (CFG_SPI0_CS_GPIO_MODE)
	.cs_control = spi0_cs,
#endif
	.clkdelay = SSP_FEEDBACK_CLK_DELAY_1T,
};
static struct spi_board_info initdata = {  
	 .modalias = "nrf24l01",  
	 .platform_data = &nrf24l01_data,
	 .controller_data = &spi0_info,  
	 .mode  = SPI_MODE_0,
	 .max_speed_hz = 25*1000000,
	 .bus_num = 0,  
	 .chip_select = 0,  
};
static void SPI_GPIO(void)
{
	nxp_soc_gpio_set_io_func(PAD_GPIO_C+ 30, 0);//cs
	nxp_soc_gpio_set_io_func(PAD_GPIO_C+ 29, 1); 
	nxp_soc_gpio_set_io_func(PAD_GPIO_D+ 0, 1); 
	nxp_soc_gpio_set_io_func(PAD_GPIO_C+ 31, 1);
}
static int __devinit nrf24l01_probe(struct spi_device *spi)
{
	return 0;
}
static int __devexit nrf24l01_remove(struct spi_device *spi)
{
	return 0;
}
static struct spi_driver nrf24l01_driver = {
	.driver = {
		.name = "nrf24l01",
		.bus = &spi_bus_type,
		.owner = THIS_MODULE,
	},

	.probe = nrf24l01_probe,
	.remove = __devexit_p(nrf24l01_remove),
	//.suspend = mcp251x_can_suspend,
	//.resume = mcp251x_can_resume,
};

static int NRF24l01_Init(void)
{
	struct spi_master *spi_mast = 0;
	GPIO_Typedef GPIO;
	SPI_GPIO();
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
	spi_register_driver(&nrf24l01_driver);
	spi_setup(dev);
	//L01_Init();
	/****** GPIO IRQ初始化 ******/
	GPIO.Pin = GPIO_Pin_26;
	GPIO.Mode = Output;
	GPIO.Pull = Pull_Up;
	GPIO_Init(GPIOB_BASE,&GPIO);
        GPIO_SetBit(GPIOB_BASE,GPIO_Pin_26);
	GPIO_ResetBit(GPIOB_BASE,GPIO_Pin_26);
        spi_w8r8(dev,0xff);
        GPIO_SetBit(GPIOB_BASE,GPIO_Pin_26);
	printk("NRF24l01 Is Online\n");
	return 0;
}
static void NRF24l01_Exit(void)
{
	printk("NRF24l01 is Offline\n");
	GPIO_ResetBit(GPIOB_BASE,GPIO_Pin_26);
	GPIO_Free(GPIOB_BASE,GPIO_Pin_26);
	if(dev)
	{
		spi_unregister_device(dev);
		spi_unregister_driver(&nrf24l01_driver);
	}
}
MODULE_LICENSE("Dual BSD/GPL");
module_init(NRF24l01_Init);
module_exit(NRF24l01_Exit);
