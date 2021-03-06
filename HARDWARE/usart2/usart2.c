#include "sys.h"
#include "usart2.h"
#include "gizwits_product.h"

//////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
//#if 1
//#pragma import(__use_no_semihosting)
////标准库需要的支持函数
//struct __FILE
//{
//    int handle;
//};

//FILE __stdout;
///**
// * @brief	定义_sys_exit()以避免使用半主机模式
// *
// * @param	void
// *
// * @return  void
// */
//void _sys_exit(int x)
//{
//    x = x;
//}
///**
// * @brief	重定义fputc函数
// *
// * @param	ch		输出字符量
// * @param	f		文件指针
// *
// * @return  void
// */
//int fputc(int ch, FILE *f)
//{
//    while(tls_reg_read32(HR_UART2_FIFO_STATUS) & 0x3F);	//等待发送完毕

//    tls_reg_write32(HR_UART2_TX_WIN, (u8) ch);
//    return ch;
//}
//#endif

#if EN_USART2_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA = 0;     //接收状态标记


/**
 * @brief	初始化串口0函数
 *
 * @param	bound	串口波特率
 *
 * @return  void
 */
void uart2_init(u32 bound)
{
    u32 bd;
    u32 apbclk;
    tls_sys_clk sysclk;

    /* 1.配置IO */
    wm_uart2_tx_scio_config(WM_IO_PB_20);
    wm_uart2_rx_config(WM_IO_PB_19);

    /* 2.波特率设置:
    	ubdiv = (apbclk / (16 * bound) - 1)
    	ubdiv_frac = ((apbclk % (bound * 16)) / (bound)) */
    tls_sys_clk_get(&sysclk);
    apbclk = sysclk.apbclk * 1000000;
    bd = (apbclk / (16 * bound) - 1) | (((apbclk % (bound * 16)) / (bound)) << 16);
    tls_reg_write32(HR_UART2_BAUD_RATE_CTRL, bd);

    /* 2.串口参数设置：8位数据位/1位停止位/无奇偶校验位/发送使能/接收使能 */
    tls_reg_write32(HR_UART2_LINE_CTRL, ULCON_WL8 | ULCON_TX_EN | ULCON_RX_EN);
    /* 3.硬件流控关闭 */
    tls_reg_write32(HR_UART2_FLOW_CTRL, 0);
    /* 3.不使能DMA */
    tls_reg_write32(HR_UART2_DMA_CTRL, 0);
    /* 4.FIFO触发设置：1个字节 */
    tls_reg_write32(HR_UART2_FIFO_CTRL, 0);
    /* 5.开启RX中断:接收触发中断和接收超时中断*/
    tls_reg_write32(HR_UART2_INT_MASK, 0xFF & (~(UIS_RX_FIFO | UIS_RX_FIFO_TIMEOUT)));

    /* 6.串口接收中断配置 */
    NVIC_ClearPendingIRQ(UART2_IRQn);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = UART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 7;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

/**
 * @brief	串口0中断服务程序
 *
 * @param   void
 *
 * @return  void
 */
void UART2_IRQHandler(void)
{
    u8 res;

    if(tls_reg_read32(HR_UART2_INT_SRC) & UIS_RX_FIFO)	//接收到数据
    {
        res = (u8)tls_reg_read32(HR_UART2_RX_WIN);
				gizPutData(&res, 1);

        if((USART2_RX_STA & 0x8000) == 0) //接收未完成
        {
            if(USART2_RX_STA & 0x4000) //接收到了0x0d
            {
                if(res != 0x0a)USART2_RX_STA = 0; //接收错误,重新开始

                else USART2_RX_STA |= 0x8000;	//接收完成了
            }

            else   //还没收到0X0D
            {
                if(res == 0x0d)USART2_RX_STA |= 0x4000;

                else
                {
                    USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = res;
                    USART2_RX_STA++;

                    if(USART2_RX_STA > (USART2_REC_LEN - 1))USART2_RX_STA = 0; //接收数据错误,重新开始接收
                }
            }
        }

        tls_reg_write32(HR_UART2_INT_SRC, UIS_RX_FIFO);		//清除状态标志位
    }

    if(tls_reg_read32(HR_UART2_INT_SRC) & UIS_RX_FIFO_TIMEOUT)	//接收到数据
    {
        tls_reg_write32(HR_UART2_INT_SRC, UIS_RX_FIFO_TIMEOUT);		//清除状态标志位
    }
}
#endif





