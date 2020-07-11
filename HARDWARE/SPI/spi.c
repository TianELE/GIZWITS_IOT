#include "sys.h"
#include "spi.h"

/**
 * @brief	SPI初始化代码
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
    /*1.IO口配置*/
    wm_spi_ck_config(WM_IO_PB_27);
    wm_spi_do_config(WM_IO_PB_02);
    wm_spi_di_config(WM_IO_PB_01);

    /*2.SPI初始化*/
    tls_spi_init();

    /*3.配置DMA传输模式*/
    tls_spi_trans_type(SPI_DMA_TRANSFER);

    /*4.配置CPHA/CPOL/片选低有效/频率最大(这里为APB/2=20MHz)*/
    tls_spi_setup(TLS_SPI_MODE_3, TLS_SPI_CS_LOW, TLS_SPI_FCLK_MAX);
}


/**
 * @brief	SPI发送数据
 *
 * @param   TxData	要写入的字节
 * @param   size	写入字节大小
 *
 * @return  u8		0:写入成功,其他:写入失败
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
        //发送数据大于DMA缓存
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
 * @brief	SPI读取数据
 *
 * @param   RxData	读缓存
 * @param   size	读取字节大小
 *
 * @return  u8		0:读取成功,其他:读取失败
 */
u8 SPI_ReadByte(u8 *RxData, u32 size)
{
    return tls_spi_read(RxData, size);
}

/**
 * @brief	设置SPI低速模式
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
 * @brief	设置SPI高速模式
 *
 * @param   void
 *
 * @return  void
 */
void SPI_SetHightSpeed(void)
{
    tls_spi_setup(TLS_SPI_MODE_3, TLS_SPI_CS_LOW, TLS_SPI_FCLK_MAX);
}
