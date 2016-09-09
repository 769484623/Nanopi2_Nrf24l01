//#include <linux/slab.h>
#include <linux/delay.h>
/*GPIO Include*/
#include "nRF24L01.h"
struct spi_device* dev = 0;
uint8_t SPI_ExchangeByte(uint8_t input)
{
	return spi_w8r8(dev,input);
}
static void spi0_cs(u32 chipselect)
{
#if (!CFG_SPI0_CS_GPIO_MODE)/*we don't use FFS,and cs is manualy controled.*/
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
	.com_mode = 1,
	.iface = SSP_INTERFACE_MOTOROLA_SPI,
	.hierarchy = SSP_MASTER,
	.slave_tx_disable = 1,
	.rx_lev_trig = SSP_RX_1_OR_MORE_ELEM,
	.tx_lev_trig = SSP_TX_1_OR_MORE_EMPTY_LOC,
	.ctrl_len = SSP_BITS_8,
	.wait_state = SSP_MWIRE_WAIT_ZERO,
	.duplex = SSP_MICROWIRE_CHANNEL_HALF_DUPLEX,
#if (CFG_SPI0_CS_GPIO_MODE)
	.cs_control = spi0_cs,
#endif
};
static struct spi_board_info initdata = {  
	 .modalias = "nrf24l01",  
	 .platform_data = &nrf24l01_data,
	 .controller_data = &spi0_info,  
	 .mode  = SPI_MODE_0,
	 .max_speed_hz = 10*1000000,
	 .bus_num = 0,  
	 .chip_select = 0,  
	 .irq = PB_PIO_IRQ(IRQ_GPIO_C_START + 5),
};
static void SPI_GPIO(void)
{
	nxp_soc_gpio_set_io_func(PAD_GPIO_B+ 26, 1);//ce
	nxp_soc_gpio_set_io_pull_sel(PAD_GPIO_B+ 26,1);
	L01_CE_LOW();
	nxp_soc_gpio_set_io_func(PAD_GPIO_C+ 30, 0);//cs
	L01_CSN_HIGH();
	nxp_soc_gpio_set_io_func(PAD_GPIO_C+ 29, 1); 
	nxp_soc_gpio_set_io_func(PAD_GPIO_D+ 0, 1); 
	nxp_soc_gpio_set_io_func(PAD_GPIO_C+ 31, 1);
	
}
void RF_Initial(void)
{
	mdelay(200);
	L01_Init();             // 初始化L01寄存器
	
	L01_SetTRMode(RX_MODE); // 接收模式      
	L01_FlushRX();          // 复位接收FIFO指针
	L01_FlushTX();          // 复位发送FIFO指针
  	L01_ClearIRQ(IRQ_ALL);  // 清除所有中断
  	L01_CE_HIGH();          // CE = 1, 启动接收  
	//SetL01IRQ();	
}
static int NRF24l01_Init(void)
{
	struct spi_master *spi_mast = 0;
	char recv_buffer[32];
	int length = 0;
	spi_mast = spi_busnum_to_master(0);
	if(!spi_mast)
	{
		printk("cannot get spi_master!\n");
		return 1;
	}
	dev = spi_new_device(spi_mast,&initdata);
	if(dev)
	{
		dev->bits_per_word = 8;
	}
	else
	{
		printk("spi_new_device failed\n");
		return 1;
	}	
	SPI_GPIO();
	spi_setup(dev);
	//printk("%x\n",L01_ReadSingleReg(R_REGISTER + L01REG_EN_AA));
	RF_Initial();
	if(L01_ReadIRQSource() & (1<<RX_DR))   // 检测无线模块是否接收到数据
	{
		// 读取接收到的数据长度和数据内容
		length = L01_ReadRXPayload(recv_buffer);
		// 判断接收数据是否正确
		if(length)
		{
		}
		L01_FlushRX();                          // 复位接收FIFO指针
		L01_ClearIRQ(IRQ_ALL);                  // 清除中断
	}
	printk("NRF24l01 Is Online\n");
	return 0;
}
static void NRF24l01_Exit(void)
{
	printk("NRF24l01 is Offline\n");
	if(dev)
	{
		spi_unregister_device(dev);
	}
}
MODULE_LICENSE("Dual BSD/GPL");
module_init(NRF24l01_Init);
module_exit(NRF24l01_Exit);
