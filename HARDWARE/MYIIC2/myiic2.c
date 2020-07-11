#include "myiic2.h"
#include "delay.h"


/**
 * @brief	IIC底层延时函数
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
 * @brief	IIC初始化函数
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
 * @brief	产生IIC起始信号
 *
 * @param   void
 *
 * @return  void
 */
void IIC2_Start(void)
{
    SDA2_OUT();     //sda线输出
    IIC2_SDA = 1;
    IIC2_SCL = 1;
    IIC2_Delay();
    IIC2_SDA = 0;//START:when CLK is high,DATA change form high to low
    IIC2_Delay();
    IIC2_SCL = 0;//钳住I2C总线，准备发送或接收数据
}
/**
 * @brief	产生IIC停止信号
 *
 * @param   void
 *
 * @return  void
 */
void IIC2_Stop(void)
{
    SDA2_OUT();//sda线输出
    IIC2_SDA = 0;
    IIC2_SCL = 1;
    IIC2_Delay();
    IIC2_SDA = 1;//STOP:when CLK is high DATA change form low to high
    IIC2_Delay();
    IIC2_SCL = 0;//发送I2C总线结束信号
}

/**
 * @brief	等待应答信号到来
 *
 * @param   void
 *
 * @return  u8		1，接收应答失败
 *					0，接收应答成功
 */
u8 IIC2_Wait_Ack(void)
{
    u16 ucErrTime = 0;
    SDA2_IN();      //SDA设置为输入
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

    IIC2_SCL = 0;//时钟输出0
    return 0;
}
/**
 * @brief	产生ACK应答
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
 * @brief	不产生ACK应答
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
 * @brief	IIC发送一个字节
 *
 * @param   txd		需要发送的数据
 *
 * @return  void
 */
void IIC2_Send_Byte(u8 txd)
{
    u8 t;
    SDA2_OUT();
    IIC2_SCL = 0;//拉低时钟开始数据传输

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
 * @brief	读1个字节数据
 *
 * @param   ack		1，发送ACK		0，发送nACK
 *
 * @return  u8		返回读取数据
 */
u8 IIC2_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA2_IN();//SDA设置为输入

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
        IIC2_NAck();//发送nACK

    else
        IIC2_Ack(); //发送ACK

    return receive;
}


