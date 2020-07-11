#include "sys.h"
#include "spi.h"

/**
 * @brief	SPI��ʼ������
 *
 * @param   void
 *
 * @return  void
 */
void SPI_Init(void)
{
    /*
    	spi_clk:	PB27
    	spi_do:		PB2
    	LCD_di:		PB1
    */
    /*1.IO������*/
    wm_spi_ck_config(WM_IO_PB_27);
    wm_spi_do_config(WM_IO_PB_02);
    wm_spi_di_config(WM_IO_PB_01);

    /*2.SPI��ʼ��*/
    tls_spi_init();

    /*3.����DMA����ģʽ*/
    tls_spi_trans_type(SPI_DMA_TRANSFER);

    /*4.����CPHA/CPOL/Ƭѡ����Ч/Ƶ�����(����ΪAPB/2=20MHz)*/
    tls_spi_setup(TLS_SPI_MODE_3, TLS_SPI_CS_LOW, TLS_SPI_FCLK_MAX);
}


/**
 * @brief	SPI��������
 *
 * @param   TxData	Ҫд����ֽ�
 * @param   size	д���ֽڴ�С
 *
 * @return  u8		0:д��ɹ�,����:д��ʧ��
 */
u8 SPI_WriteByte(u8 *TxData, u32 size)
{
    u32 cnt;
    u32 repeat;
    u32 remain;

    cnt = 0;
    remain = size;

    if(size > SPI_DMA_BUF_MAX_SIZE)
    {
        //�������ݴ���DMA����
        repeat = size / SPI_DMA_BUF_MAX_SIZE;
        remain = size % SPI_DMA_BUF_MAX_SIZE;

        while(cnt < (repeat * SPI_DMA_BUF_MAX_SIZE))
        {
            tls_spi_write(TxData + cnt, SPI_DMA_BUF_MAX_SIZE);
            cnt += SPI_DMA_BUF_MAX_SIZE;
        }
    }

    return tls_spi_write(TxData + cnt, remain);
}

/**
 * @brief	SPI��ȡ����
 *
 * @param   RxData	������
 * @param   size	��ȡ�ֽڴ�С
 *
 * @return  u8		0:��ȡ�ɹ�,����:��ȡʧ��
 */
u8 SPI_ReadByte(u8 *RxData, u32 size)
{
    return tls_spi_read(RxData, size);
}

/**
 * @brief	����SPI����ģʽ
 *
 * @param   void
 *
 * @return  void
 */
void SPI_SetLowSpeed(void)
{
    tls_spi_setup(TLS_SPI_MODE_3, TLS_SPI_CS_LOW, 200000);
}

/**
 * @brief	����SPI����ģʽ
 *
 * @param   void
 *
 * @return  void
 */
void SPI_SetHightSpeed(void)
{
    tls_spi_setup(TLS_SPI_MODE_3, TLS_SPI_CS_LOW, TLS_SPI_FCLK_MAX);
}
