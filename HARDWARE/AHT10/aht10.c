#include "aht10.h"
#include "delay.h"
#include "myiic.h"

 /* 向ATH10写入数据*/
u8 AHT10_Write_Data(u8 cmd, u8 *data, u8 len)
{
    IIC_Start();
    IIC_Send_Byte((AHT10_IIC_ADDR << 1) | 0); //发送器件地址+写命令

    if(IIC_Wait_Ack())          //等待应答
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(cmd);         //写寄存器地址
    IIC_Wait_Ack();             //等待应答
    for(u8 i = 0; i < len; i++)
    {
        IIC_Send_Byte(data[i]);     //发送数据
        IIC_Wait_Ack();				//等待应答
    }
    IIC_Stop();
    return 0;
}

 /*	读一个字节*/
u8 AHT10_ReadOneByte(void)
{
    u8 res = 0;
    IIC_Start();
    IIC_Send_Byte((AHT10_IIC_ADDR << 1) | 0X01); //发送器件地址+读命令
    if(IIC_Wait_Ack())          //等待应答
    {
        IIC_Stop();
        return 1;
    }
    res = IIC_Read_Byte(0);		//读数据,发送nACK
    IIC_Stop();                 //产生一个停止条件
    return res;
}

 /* 读取AHT10的状态寄存器*/
static u8 AHT10_Read_Status(void)
{
    u8 res;
    res = 20;
    while(res--)
    {
        if(AHT10_ReadOneByte() == 0x19) return 0;
        delay_ms(100);
    }
    return 1;
}
/*	读数据*/
u8 AHT10_Read_Data(u8 *data, u8 len)
{
    IIC_Start();
    IIC_Send_Byte((AHT10_IIC_ADDR << 1) | 0x01); //发送器件地址+读命令
    if(IIC_Wait_Ack())          //等待应答
    {
        IIC_Stop();
        return 1;
    }
    for(u8 i = 0; i < len; i++)
    {
        if(i == (len - 1))
            data[i] = IIC_Read_Byte(0);		//读数据,发送nACK
        else
            data[i] = IIC_Read_Byte(1);		//读数据,发送ACK
    }
    IIC_Stop();
    return 0;
}

/*读取温度数据 */
float AHT10_Read_Temperature(void)
{
    u8 res = 0;
    u8 cmd[2] = {0, 0};
    u8 temp[6];
    float cur_temp;
    if(AHT10_Read_Status())	return 0;
    res = AHT10_Write_Data(AHT10_GET_DATA, cmd, 2); //发送读取数据命令
    if(res)	return 0;
    delay_ms(50);
    res = AHT10_Read_Data(temp, 6);				//读取数据
    if(res)	return 0;
    cur_temp = ((temp[3] & 0xf) << 16 | temp[4] << 8 | temp[5]) * 200.0 / (1 << 20) - 50;
    return cur_temp;
}

/*	读取湿度数据*/
float AHT10_Read_Humidity(void)
{
    u8 res = 0;
    u8 cmd[2] = {0, 0};
    u8 humi[6];
    float cur_humi;
    if(AHT10_Read_Status())	return 0;
    res = AHT10_Write_Data(AHT10_GET_DATA, cmd, 2); //发送读取数据命令
    if(res)	return 0;
    delay_ms(50);
    res = AHT10_Read_Data(humi, 6);				//读取数据
    if(res)	return 0;
    cur_humi = ((humi[1]) << 12 | humi[2] << 4 | (humi[3] & 0xF0)) * 100.0 / (1 << 20);
    return cur_humi;
}



/*ATH10传感器初始化*/
u8 AHT10_Init(void)
{
    u8 res;
    u8 temp[2] = {0, 0};
    IIC_Init();		//初始化IIC接口：注意这里的IIC总线为：SCL-PD6 SDA-PC1
    if(AHT10_Read_Status())	return 1;
    res = AHT10_Write_Data(AHT10_NORMAL_CMD, temp, 2);
    if(res != 0)	return 1;
    delay_ms(300);
    temp[0] = 0x08;
    temp[1] = 0x00;
    res = AHT10_Write_Data(AHT10_CALIBRATION_CMD, temp, 2);
    if(res != 0)	return 2;
    delay_ms(300);
    return 0;
}

