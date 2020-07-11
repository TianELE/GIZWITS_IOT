#include "ap3216c.h"
#include "myiic2.h"
#include "delay.h"


/*初始化AP3216C*/
u8 AP3216C_Init(void)
{
    u8 temp = 0;
    IIC2_Init();         				//初始化IIC
    AP3216C_WriteOneByte(0x00, 0X04);	//复位AP3216C
    delay_ms(50);						//AP33216C复位至少10ms
    AP3216C_WriteOneByte(0x00, 0X03);	//开启ALS、PS+IR
    temp = AP3216C_ReadOneByte(0X00);		//读取刚刚写进去的0X03

    if(temp == 0X03)return 0;				//AP3216C正常

    else return 1;						//AP3216C失败
}

/*读取AP3216C的数据,读取原始数据，包括ALS,PS和IR*/
void AP3216C_ReadData(u16* ir, u16* ps, u16* als)
{
    u8 buf[6];
    u8 i;

    for(i = 0; i < 6; i++)
    {
        buf[i] = AP3216C_ReadOneByte(0X0A + i);		//循环读取所有传感器数据
    }

    if(buf[0] & 0X80)*ir = 0;						//IR_OF位为1,则数据无效

    else *ir = ((u16)buf[1] << 2) | (buf[0] & 0X03); 	//读取IR传感器的数据

    *als = ((u16)buf[3] << 8) | buf[2];				//读取ALS传感器的数据

    if(buf[4] & 0x40)*ps = 0;    					//IR_OF位为1,则数据无效

    else *ps = ((u16)(buf[5] & 0X3F) << 4) | (buf[4] & 0X0F); //读取PS传感器的数据
}

/* IIC写一个字节*/
u8 AP3216C_WriteOneByte(u8 reg, u8 data)
{
    IIC2_Start();
    IIC2_Send_Byte(AP3216C_ADDR | 0X00); //发送器件地址+写命令

    if(IIC2_Wait_Ack())          //等待应答
    {
        IIC2_Stop();
        return 1;
    }

    IIC2_Send_Byte(reg);         //写寄存器地址
    IIC2_Wait_Ack();             //等待应答
    IIC2_Send_Byte(data);        //发送数据

    if(IIC2_Wait_Ack())          //等待ACK
    {
        IIC2_Stop();
        return 1;
    }

    IIC2_Stop();
    return 0;
}

/*IIC读一个字节*/
u8 AP3216C_ReadOneByte(u8 reg)
{
    u8 res;
    IIC2_Start();
    IIC2_Send_Byte(AP3216C_ADDR | 0X00); //发送器件地址+写命令
    IIC2_Wait_Ack();             //等待应答
    IIC2_Send_Byte(reg);         //写寄存器地址
    IIC2_Wait_Ack();             //等待应答
    IIC2_Start();
    IIC2_Send_Byte(AP3216C_ADDR | 0X01); //发送器件地址+读命令
    IIC2_Wait_Ack();             //等待应答
    res = IIC2_Read_Byte(0);		//读数据,发送nACK
    IIC2_Stop();                 //产生一个停止条件
    return res;
}

