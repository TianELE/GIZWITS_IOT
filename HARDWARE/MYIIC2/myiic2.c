#include "myiic2.h"
#include "delay.h"


/**
 * @brief	IIC�ײ���ʱ����
 *
 * @param   void
 *
 * @return  void
 */
void IIC2_Delay(void)
{
    delay_us(3);
}


/**
 * @brief	IIC��ʼ������
 *
 * @param   void
 *
 * @return  void
 */
void IIC2_Init(void)
{
    /*
    	IIC_SCL2	PA2
    	IIC_SDA		PA1
    */

    tls_gpio_cfg(WM_IO_PA_01, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
    tls_gpio_cfg(WM_IO_PA_02, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
}

/**
 * @brief	����IIC��ʼ�ź�
 *
 * @param   void
 *
 * @return  void
 */
void IIC2_Start(void)
{
    SDA2_OUT();     //sda�����
    IIC2_SDA = 1;
    IIC2_SCL = 1;
    IIC2_Delay();
    IIC2_SDA = 0;//START:when CLK is high,DATA change form high to low
    IIC2_Delay();
    IIC2_SCL = 0;//ǯסI2C���ߣ�׼�����ͻ��������
}
/**
 * @brief	����IICֹͣ�ź�
 *
 * @param   void
 *
 * @return  void
 */
void IIC2_Stop(void)
{
    SDA2_OUT();//sda�����
    IIC2_SDA = 0;
    IIC2_SCL = 1;
    IIC2_Delay();
    IIC2_SDA = 1;//STOP:when CLK is high DATA change form low to high
    IIC2_Delay();
    IIC2_SCL = 0;//����I2C���߽����ź�
}

/**
 * @brief	�ȴ�Ӧ���źŵ���
 *
 * @param   void
 *
 * @return  u8		1������Ӧ��ʧ��
 *					0������Ӧ��ɹ�
 */
u8 IIC2_Wait_Ack(void)
{
    u16 ucErrTime = 0;
    SDA2_IN();      //SDA����Ϊ����
    IIC2_SDA = 1;
    IIC2_Delay();
    IIC2_SCL = 1;
    IIC2_Delay();

    while(READ2_SDA)
    {
        ucErrTime++;

        if(ucErrTime > 2000)
        {
            IIC2_Stop();
            return 1;
        }
    }

    IIC2_SCL = 0;//ʱ�����0
    return 0;
}
/**
 * @brief	����ACKӦ��
 *
 * @param   void
 *
 * @return  void
 */
void IIC2_Ack(void)
{
    IIC2_SCL = 0;
    SDA2_OUT();
    IIC2_SDA = 0;
    IIC2_Delay();
    IIC2_SCL = 1;
    IIC2_Delay();
    IIC2_SCL = 0;
}
/**
 * @brief	������ACKӦ��
 *
 * @param   void
 *
 * @return  void
 */
void IIC2_NAck(void)
{
    IIC2_SCL = 0;
    SDA2_OUT();
    IIC2_SDA = 1;
    IIC2_Delay();
    IIC2_SCL = 1;
    IIC2_Delay();
    IIC2_SCL = 0;
}
/**
 * @brief	IIC����һ���ֽ�
 *
 * @param   txd		��Ҫ���͵�����
 *
 * @return  void
 */
void IIC2_Send_Byte(u8 txd)
{
    u8 t;
    SDA2_OUT();
    IIC2_SCL = 0;//����ʱ�ӿ�ʼ���ݴ���

    for(t = 0; t < 8; t++)
    {
        IIC2_SDA = (txd & 0x80) >> 7;
        txd <<= 1;
        IIC2_SCL = 1;
        IIC2_Delay();
        IIC2_SCL = 0;
        IIC2_Delay();
    }
}
/**
 * @brief	��1���ֽ�����
 *
 * @param   ack		1������ACK		0������nACK
 *
 * @return  u8		���ض�ȡ����
 */
u8 IIC2_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA2_IN();//SDA����Ϊ����

    for(i = 0; i < 8; i++)
    {
        IIC2_SCL = 0;
        IIC2_Delay();
        IIC2_SCL = 1;
        receive <<= 1;

        if(READ2_SDA)receive++;

        IIC2_Delay();
    }

    if(!ack)
        IIC2_NAck();//����nACK

    else
        IIC2_Ack(); //����ACK

    return receive;
}


