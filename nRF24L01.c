/*===========================================================================
* ��ַ ��http://yhmcu.taobao.com/   http://www.cdebyte.com/                 *
* ���� ������  ԭ �ں͵��ӹ�����  �� �ڰ��ص��ӿƼ����޹�˾                 * 
* �ʼ� ��yihe_liyong@126.com                                                *
* �绰 ��18615799380                                                        *
============================================================================*/

#include "nRF24L01.h"
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
extern struct spi_device* dev;

/*===========================================================================
* ���� ��L01_ReadSingleReg() => ��ȡһ���Ĵ�����ֵ                          * 
* ���� ��Addr����ȡ�ļĴ�����ַ                                             * 
* ��� ��������ֵ                                                           * 
============================================================================*/
uint8_t L01_ReadSingleReg(uint8_t Addr)
{
    uint8_t btmp[2],Command[2] = {R_REGISTER | Addr,0xFF};
    
    spi_write_then_read(dev,Command,2,btmp,2);
    
    return (btmp[1]);
}

/*===========================================================================
* ���� ��L01_WriteSingleReg() => д���ݵ�һ���Ĵ���                         * 
* ���� ��Addr��д��Ĵ����ĵ�ַ��Value����д���ֵ                          * 
============================================================================*/
void L01_WriteSingleReg(uint8_t Addr, uint8_t Value)
{
    uint8_t Command[2] = {Addr|W_REGISTER,Value};
    spi_write(dev,Command,2);
}

/*===========================================================================
* ���� ��L01_WriteMultiReg() => д���ݵ�����Ĵ���                          * 
* ���� ��StartAddr,д��Ĵ������׵�ַ��pBuff,ָ���д���ֵ��Length,����    * 
============================================================================*/
void L01_WriteMultiReg(uint8_t StartAddr, uint8_t *pBuff, uint8_t Length)
{
    uint8_t Command[Length+1];
    Command[0] = W_REGISTER | L01REG_TX_ADDR;
    strncpy(Command+1,pBuff,Length);
    spi_write_then_read(dev,Command,Length+1,NULL,Length+1);
}

/*===========================================================================
* ���� ��L01_FlushTX() => ��λTX FIFOָ��                                   * 
============================================================================*/
void L01_FlushTX(void)
{
    uint8_t Command = FLUSH_TX;
    spi_w8r8(dev,Command);
}

/*===========================================================================
* ���� ��L01_FlushRX() => ��λRX FIFOָ��                                   *
============================================================================*/
void L01_FlushRX(void)
{
    uint8_t Command = FLUSH_RX;
    spi_w8r8(dev,Command);
}

uint8_t L01_ReadStatusReg(void)
{
    return spi_w8r8(dev,R_REGISTER + L01REG_STATUS);
}

/*===========================================================================
* ���� ��L01_ClearIRQ() => ����ж�                                         * 
* ���� ��IRQ_Source����Ҫ������ж�Դ                                       * 
============================================================================*/
void L01_ClearIRQ(uint8_t IRQ_Source)
{
    uint8_t btmp = 0;
    uint8_t Command[2] = {W_REGISTER + L01REG_STATUS,IRQ_Source & ((1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT))};

    btmp = L01_ReadStatusReg();

    spi_write_then_read(dev,Command,2,NULL,2);
    
    L01_ReadStatusReg();
}

/*===========================================================================
* ���� ��L01_ReadIRQSource() => ��ȡ�ж�                                    *         
* ��� ���������ж�Դ                                                       * 
============================================================================*/
uint8_t L01_ReadIRQSource(void)
{
    return (L01_ReadStatusReg() & ((1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT)));
}

/*===========================================================================
* ���� ��L01_ReadTopFIFOWidth() => ��ȡFIFO���ݿ��                         * 
============================================================================*/
uint8_t L01_ReadTopFIFOWidth(void)
{
    uint8_t btmp[2],Command[2] = {R_RX_PL_WID,0xFF};
    
    spi_write_then_read(dev,Command,2,btmp,2);
    
    return (btmp[1]);
}

/*===========================================================================
* ���� ��L01_ReadRXPayload() => ��ȡ���յ�������                            * 
* ���� ��pBuff��ָ���յ�������                                              * 
* ��� �����ݳ���                                                           * 
============================================================================*/
uint8_t L01_ReadRXPayload(uint8_t *pBuff)
{
    uint8_t width, PipeNum , Command[34] , i;
    PipeNum = (L01_ReadSingleReg(L01REG_STATUS)>>1) & 0x07;
    width = L01_ReadTopFIFOWidth();
    for(i = 0;i<34;i++)
    {
        Command[i] = 0xff;
    }
    Command[0] = R_RX_PAYLOAD;
    spi_write_then_read(dev,Command,width,pBuff,width);
    L01_FlushRX();
    return (width);
}

/*===========================================================================
* ���� ��L01_WriteTXPayload_Ack() => д���ݵ�TXFIFO(��ACK����)              * 
* ���� ��pBuff��ָ���д������ݣ�nBytes��д�����ݵĳ���                    * 
============================================================================*/
void L01_WriteTXPayload_Ack(uint8_t *pBuff, uint8_t nBytes)
{
    uint8_t length = (nBytes>32) ? 32 : nBytes;
    uint8_t Command[length+1];

    L01_FlushTX();
    Command[0] = W_TX_PAYLOAD;
    strncpy(Command+1,pBuff,length);
    spi_write_then_read(dev,Command,length,NULL,length);
}

/*===========================================================================
* ���� ��L01_WriteTXPayload_Ack() => д���ݵ�TXFIFO(����ACK����)            * 
* ���� ��Data��ָ���д������ݣ�Data_Length��д�����ݵĳ���                * 
============================================================================*/
void L01_WriteTXPayload_NoAck(uint8_t *Data, uint8_t Data_Length)
{
    uint8_t Command[Data_Length+1];
    if ((Data_Length>32) || (Data_Length==0))   { return; }
    Command[0] = W_TX_PAYLOAD_NOACK;
    strncpy(Command+1,Data,Data_Length);
    spi_write_then_read(dev,Command,Data_Length,NULL,Data_Length);
}

/*===========================================================================
* ���� ��L01_SetTXAddr() => ���÷��������ַ                                * 
* ���� ��pAddrָ����Ҫ���õĵ�ַ���ݣ�Addr_Length����ַ����                 * 
============================================================================*/
void L01_SetTXAddr(uint8_t *pAddr, uint8_t Addr_Length)
{
    uint8_t Length = (Addr_Length>5) ? 5 : Addr_Length;
    L01_WriteMultiReg(L01REG_TX_ADDR, pAddr, Length);
}

/*===========================================================================
* ���� ��L01_SetRXAddr() => ���ý��������ַ                                * 
* ���� ��PipeNum���ܵ��ţ�pAddrָ����Ҫ���õ�ַ���ݣ�Addr_Length����ַ����  * 
============================================================================*/
void L01_SetRXAddr(uint8_t PipeNum, uint8_t *pAddr, uint8_t Addr_Length)
{
    uint8_t Length = (Addr_Length>5) ? 5 : Addr_Length;
    uint8_t pipe = (PipeNum>5) ? 5 : PipeNum;

    L01_WriteMultiReg(L01REG_RX_ADDR_P0 + pipe, pAddr, Length);
}

/*===========================================================================
* ���� ��L01_SetSpeed() => ����L01����                                      * 
* ���� ��speed��=SPD_250K(250K), =SPD_1M(1M), =SPD_2M(2M)                   * 
============================================================================*/
void L01_SetSpeed(L01SPD speed)
{
	uint8_t btmp = L01_ReadSingleReg(L01REG_RF_SETUP);

	btmp &= ~((1<<5) | (1<<3));
	
	switch (speed)
	{
	    case SPD_250K:  btmp |= (1<<5);             break;  // 250K
        case SPD_1M:    btmp &= ~((1<<5) | (1<<3)); break;  // 1M
        case SPD_2M:    btmp |= (1<<3);             break;  // 2M
        default:        break;                                     
	        
	}
	L01_WriteSingleReg(L01REG_RF_SETUP, btmp);
}

/*===========================================================================
* ���� ��L01_SetPower() => ����L01����                                      * 
* ���� ��power, =P_F18DBM(18DB),=P_F12DBM(12DB),=P_F6DBM(6DB),=P_0DBM(0DB)  *
============================================================================*/
void L01_SetPower(L01PWR power)
{
    uint8_t btmp = L01_ReadSingleReg(L01REG_RF_SETUP) & ~0x07;
    
    switch(power)
    {
        case P_F18DBM:  btmp |= PWR_18DB; break;    // 18DBM
        case P_F12DBM:  btmp |= PWR_12DB; break;    // 12DBM
        case P_F6DBM:   btmp |= PWR_6DB;  break;    // 6DBM
        case P_0DBM:    btmp |= PWR_0DB;  break;    // 0DBM
        default:        break;
    }
    L01_WriteSingleReg(L01REG_RF_SETUP, btmp);
}

/*===========================================================================
* ���� ��L01_WriteHoppingPoint() => ����L01Ƶ��                             * 
* ���� ��FreqPoint�������õ�Ƶ��                                            * 
============================================================================*/
void L01_WriteHoppingPoint(uint8_t FreqPoint)
{
    L01_WriteSingleReg(L01REG_RF_CH, FreqPoint & 0x7F);
}

/*===========================================================================
* ���� ��L01_SetTRMode() => ����L01ģʽ                                     * 
* ���� ��mode��=TX_MODE, TX mode; =RX_MODE, RX mode                         * 
============================================================================*/
void L01_SetTRMode(L01MD mode)
{
    uint8_t controlreg = L01_ReadSingleReg(L01REG_CONFIG);
    if      (mode == TX_MODE)       { controlreg &= ~(1<<PRIM_RX); }
    else if (mode == RX_MODE)       { controlreg |= (1<<PRIM_RX); }

    L01_WriteSingleReg(L01REG_CONFIG, controlreg);
}

/*===========================================================================
* ���� ��L01_Init() => ��ʼ��L01                                             * 
============================================================================*/
void L01_Init(void)
{
    uint8_t addr[5] = { INIT_ADDR };

    L01_CE_LOW();    
    L01_ClearIRQ(IRQ_ALL);
    
    // ʹ�ܹܵ�0��̬������
    L01_WriteSingleReg(L01REG_DYNPD, (1<<0));
    L01_WriteSingleReg(L01REG_FEATRUE, 0x06);
    L01_ReadSingleReg(L01REG_DYNPD);
    L01_ReadSingleReg(L01REG_FEATRUE);

    L01_WriteSingleReg(L01REG_CONFIG, (1<<EN_CRC)|(1<<PWR_UP));
    L01_WriteSingleReg(L01REG_EN_AA, (1<<ENAA_P0));     // �Զ�Ӧ�𣨹ܵ�0��
    L01_WriteSingleReg(L01REG_EN_RXADDR, (1<<ERX_P0));  // ʹ�ܽ��գ��ܵ�0��
    L01_WriteSingleReg(L01REG_SETUP_AW, AW_5BYTES);     // ��ַ��� 5byte
    L01_WriteSingleReg(L01REG_RETR, ARD_4000US|(REPEAT_CNT&0x0F));
                  
    L01_WriteSingleReg(L01REG_RF_CH, 60);               // ��ʼ��Ƶ��
    L01_WriteSingleReg(L01REG_RF_SETUP, 0x26); 
    L01_SetTXAddr(&addr[0], 5);                         // ���õ�ַ�����ͣ�
    L01_SetRXAddr(0, &addr[0], 5);                      // ���õ�ַ�����գ�
    
    L01_WriteHoppingPoint(77);                          // ����Ƶ��
    L01_SetSpeed(SPD_250K);                             // ���ÿ���Ϊ250K
}

/*===========================================================================
-----------------------------------�ļ�����----------------------------------
===========================================================================*/
