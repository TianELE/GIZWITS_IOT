#include "myiic.h"
#include "delay.h"



 /* IIC�ײ���ʱ����*/
void IIC_Delay(void)
{
    delay_us(500);
}

 /* IIC��ʼ������*/
void IIC_Init(void)
{
    tls_gpio_cfg(WM_IO_PA_00, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
    tls_gpio_cfg(WM_IO_PA_01, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
}

 /* ����IIC��ʼ�ź�*/
void IIC_Start(void)
{
    SDA_OUT();     //sda�����
    IIC_SDA = 1;
    IIC_SCL = 1;
    IIC_Delay();
    IIC_SDA = 0;//START:when CLK is high,DATA change form high to low
    IIC_Delay();
    IIC_SCL = 0;//ǯסI2C���ߣ�׼�����ͻ��������
}
/*	����IICֹͣ�ź�*/

void IIC_Stop(void)
{
    SDA_OUT();//sda�����
    IIC_SDA = 0;
    IIC_SCL = 1;
    IIC_Delay();
    IIC_SDA = 1;//STOP:when CLK is high DATA change form low to high
    IIC_Delay();
    IIC_SCL = 0;//����I2C���߽����ź�
}

/* �ȴ�Ӧ���źŵ���
1������Ӧ��ʧ��	0������Ӧ��ɹ�
 */
u8 IIC_Wait_Ack(void)
{
    u16 ucErrTime = 0;
    SDA_IN();      //SDA����Ϊ����
    IIC_SDA = 1;
    IIC_Delay();
    IIC_SCL = 1;
    IIC_Delay();
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime > 2000)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL = 0;//ʱ�����0
    return 0;
}
 /* ����ACKӦ��*/
void IIC_Ack(void)
{
    IIC_SCL = 0;
    SDA_OUT();
    IIC_SDA = 0;
    IIC_Delay();
    IIC_SCL = 1;
    IIC_Delay();
    IIC_SCL = 0;
}

 /* ������ACKӦ��*/
void IIC_NAck(void)
{
    IIC_SCL = 0;
    SDA_OUT();
    IIC_SDA = 1;
    IIC_Delay();
    IIC_SCL = 1;
    IIC_Delay();
    IIC_SCL = 0;
}
 /* IIC����һ���ֽ�*/
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    IIC_SCL = 0;//����ʱ�ӿ�ʼ���ݴ���

    for(t = 0; t < 8; t++)
    {
        IIC_SDA = (txd & 0x80) >> 7;
        txd <<= 1;
        IIC_SCL = 1;
        IIC_Delay();
        IIC_SCL = 0;
        IIC_Delay();
    }
}
 /* ��1���ֽ�����*/
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN();//SDA����Ϊ����
    for(i = 0; i < 8; i++)
    {
        IIC_SCL = 0;
        IIC_Delay();
        IIC_SCL = 1;
        receive <<= 1;
        if(READ_SDA)receive++;
        IIC_Delay();
    }
    if(!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}