#include "sys.h"
#include "usart2.h"
#include "gizwits_product.h"

//////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
//#if 1
//#pragma import(__use_no_semihosting)
////��׼����Ҫ��֧�ֺ���
//struct __FILE
//{
//    int handle;
//};

//FILE __stdout;
///**
// * @brief	����_sys_exit()�Ա���ʹ�ð�����ģʽ
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
// * @brief	�ض���fputc����
// *
// * @param	ch		����ַ���
// * @param	f		�ļ�ָ��
// *
// * @return  void
// */
//int fputc(int ch, FILE *f)
//{
//    while(tls_reg_read32(HR_UART2_FIFO_STATUS) & 0x3F);	//�ȴ��������

//    tls_reg_write32(HR_UART2_TX_WIN, (u8) ch);
//    return ch;
//}
//#endif

#if EN_USART2_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
u8 USART2_RX_BUF[USART2_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART2_RX_STA = 0;     //����״̬���


/**
 * @brief	��ʼ������0����
 *
 * @param	bound	���ڲ�����
 *
 * @return  void
 */
void uart2_init(u32 bound)
{
    u32 bd;
    u32 apbclk;
    tls_sys_clk sysclk;

    /* 1.����IO */
    wm_uart2_tx_scio_config(WM_IO_PB_20);
    wm_uart2_rx_config(WM_IO_PB_19);

    /* 2.����������:
    	ubdiv = (apbclk / (16 * bound) - 1)
    	ubdiv_frac = ((apbclk % (bound * 16)) / (bound)) */
    tls_sys_clk_get(&sysclk);
    apbclk = sysclk.apbclk * 1000000;
    bd = (apbclk / (16 * bound) - 1) | (((apbclk % (bound * 16)) / (bound)) << 16);
    tls_reg_write32(HR_UART2_BAUD_RATE_CTRL, bd);

    /* 2.���ڲ������ã�8λ����λ/1λֹͣλ/����żУ��λ/����ʹ��/����ʹ�� */
    tls_reg_write32(HR_UART2_LINE_CTRL, ULCON_WL8 | ULCON_TX_EN | ULCON_RX_EN);
    /* 3.Ӳ�����عر� */
    tls_reg_write32(HR_UART2_FLOW_CTRL, 0);
    /* 3.��ʹ��DMA */
    tls_reg_write32(HR_UART2_DMA_CTRL, 0);
    /* 4.FIFO�������ã�1���ֽ� */
    tls_reg_write32(HR_UART2_FIFO_CTRL, 0);
    /* 5.����RX�ж�:���մ����жϺͽ��ճ�ʱ�ж�*/
    tls_reg_write32(HR_UART2_INT_MASK, 0xFF & (~(UIS_RX_FIFO | UIS_RX_FIFO_TIMEOUT)));

    /* 6.���ڽ����ж����� */
    NVIC_ClearPendingIRQ(UART2_IRQn);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = UART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 7;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

/**
 * @brief	����0�жϷ������
 *
 * @param   void
 *
 * @return  void
 */
void UART2_IRQHandler(void)
{
    u8 res;

    if(tls_reg_read32(HR_UART2_INT_SRC) & UIS_RX_FIFO)	//���յ�����
    {
        res = (u8)tls_reg_read32(HR_UART2_RX_WIN);
				gizPutData(&res, 1);

        if((USART2_RX_STA & 0x8000) == 0) //����δ���
        {
            if(USART2_RX_STA & 0x4000) //���յ���0x0d
            {
                if(res != 0x0a)USART2_RX_STA = 0; //���մ���,���¿�ʼ

                else USART2_RX_STA |= 0x8000;	//���������
            }

            else   //��û�յ�0X0D
            {
                if(res == 0x0d)USART2_RX_STA |= 0x4000;

                else
                {
                    USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = res;
                    USART2_RX_STA++;

                    if(USART2_RX_STA > (USART2_REC_LEN - 1))USART2_RX_STA = 0; //�������ݴ���,���¿�ʼ����
                }
            }
        }

        tls_reg_write32(HR_UART2_INT_SRC, UIS_RX_FIFO);		//���״̬��־λ
    }

    if(tls_reg_read32(HR_UART2_INT_SRC) & UIS_RX_FIFO_TIMEOUT)	//���յ�����
    {
        tls_reg_write32(HR_UART2_INT_SRC, UIS_RX_FIFO_TIMEOUT);		//���״̬��־λ
    }
}
#endif





