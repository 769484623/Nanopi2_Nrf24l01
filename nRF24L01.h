/*===========================================================================
* ��ַ ��http://yhmcu.taobao.com/   http://www.cdebyte.com/                 *
* ���� ������  ԭ �ں͵��ӹ�����  �� �ڰ��ص��ӿƼ����޹�˾                 * 
* �ʼ� ��yihe_liyong@126.com                                                *
* �绰 ��18615799380                                                        *
============================================================================*/

#ifndef _nRF24L01_H_
#define _nRF24L01_H_

#include "nRF24L01_Reg.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <linux/amba/pl022.h>
/* nexell soc headers */
//#include <mach/platform.h>
//#include <mach/devices.h>
#include <mach/soc.h>
#include "GPIO.h"
#define REPEAT_CNT      15  // 0-15, repeat transmit count
#define INIT_ADDR       1, 2, 3, 4, 5

struct nrf24l01_platform_data {
	uint8_t model;//250kbs 1mbps 2mbps
	unsigned long irq_flags;
	struct spi_device *dev;
};
// nRF24L01P��ؿ������Ŷ���
#define L01_CSN_LOW()   nxp_soc_gpio_set_out_value(CFG_SPI0_CS, 0)
#define L01_CSN_HIGH()  nxp_soc_gpio_set_out_value(CFG_SPI0_CS, 1)

#define L01_CE_LOW()    nxp_soc_gpio_set_out_value(PAD_GPIO_B+26,0)
#define L01_CE_HIGH()   nxp_soc_gpio_set_out_value(PAD_GPIO_B+26,1)

typedef unsigned char uint8_t;
// nRF24L01P��غ����ӿ�
// ��ʼ��L01
void L01_Init(void);

// ��λTX FIFOָ��      
void L01_FlushTX(void);

// ��λRX FIFOָ��     
void L01_FlushRX(void);     

// ��ȡ�ж�
uint8_t L01_ReadIRQSource(void);          

// ����ж�
#define IRQ_ALL  ((1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT))
void L01_ClearIRQ(uint8_t IRQ_Source); 
   
// ��ȡFIFO���ݿ��
uint8_t L01_ReadTopFIFOWidth(void);

// ��ȡ���յ�������       
uint8_t L01_ReadRXPayload(uint8_t *pBuff);  

// ����L01ģʽ 
typedef enum{ TX_MODE, RX_MODE } L01MD; 
void L01_SetTRMode(L01MD mode);

// ����L01����                 
typedef enum{ SPD_250K, SPD_1M, SPD_2M } L01SPD;
void L01_SetSpeed(L01SPD speed);

// ����L01����                 
typedef enum{ P_F18DBM, P_F12DBM, P_F6DBM, P_0DBM } L01PWR;
void L01_SetPower(L01PWR power);

// ����L01Ƶ��                
void L01_WriteHoppingPoint(uint8_t FreqPoint);    

uint8_t L01_ReadStatusReg(void);

// д���ݵ�һ���Ĵ���
void L01_WriteSingleReg(uint8_t Addr, uint8_t Value);

// ��ȡһ���Ĵ�����ֵ   
uint8_t L01_ReadSingleReg(uint8_t Addr);

// ��ȡ����Ĵ�����ֵ                 
void L01_ReadMultiReg(uint8_t StartAddr, uint8_t nBytes, uint8_t *pBuff);

// д���ݵ�����Ĵ���
void L01_WriteMultiReg(uint8_t StartAddr, uint8_t *pBuff, uint8_t Length);

// д���ݵ�TXFIFO(��ACK����)
void L01_WriteTXPayload_Ack(uint8_t *pBuff, uint8_t nBytes);

// д���ݵ�TXFIFO(����ACK����)
void L01_WriteTXPayload_NoAck(uint8_t *Data, uint8_t Data_Length);

// ���÷��������ַ
void L01_SetTXAddr(uint8_t *pAddr, uint8_t Addr_Length);

// ���ý��������ַ
void L01_SetRXAddr(uint8_t PipeNum, uint8_t *pAddr, uint8_t Addr_Length);

#endif//_nRF24L01_H_
