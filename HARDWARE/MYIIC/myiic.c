#include "myiic.h"
#include "delay.h"



 /* IIC底层延时函数*/
void IIC_Delay(void)
{
    delay_us(500);
}

 /* IIC初始化函数*/
void IIC_Init(void)
{
    tls_gpio_cfg(WM_IO_PA_00, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
    tls_gpio_cfg(WM_IO_PA_01, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
}

 /* 产生IIC起始信号*/
void IIC_Start(void)
{
    SDA_OUT();     //sda线输出
    IIC_SDA = 1;
    IIC_SCL = 1;
    IIC_Delay();
    IIC_SDA = 0;//START:when CLK is high,DATA change form high to low
    IIC_Delay();
    IIC_SCL = 0;//钳住I2C总线，准备发送或接收数据
}
/*	产生IIC停止信号*/

void IIC_Stop(void)
{
    SDA_OUT();//sda线输出
    IIC_SDA = 0;
    IIC_SCL = 1;
    IIC_Delay();
    IIC_SDA = 1;//STOP:when CLK is high DATA change form low to high
    IIC_Delay();
    IIC_SCL = 0;//发送I2C总线结束信号
}

/* 等待应答信号到来
1，接收应答失败	0，接收应答成功
 */
u8 IIC_Wait_Ack(void)
{
    u16 ucErrTime = 0;
    SDA_IN();      //SDA设置为输入
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
    IIC_SCL = 0;//时钟输出0
    return 0;
}
 /* 产生ACK应答*/
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

 /* 不产生ACK应答*/
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
 /* IIC发送一个字节*/
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    IIC_SCL = 0;//拉低时钟开始数据传输

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
 /* 读1个字节数据*/
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN();//SDA设置为输入
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
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK
    return receive;
}