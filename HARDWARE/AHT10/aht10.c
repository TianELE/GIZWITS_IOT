#include "aht10.h"
#include "delay.h"
#include "myiic.h"

 /* ��ATH10д������*/
u8 AHT10_Write_Data(u8 cmd, u8 *data, u8 len)
{
    IIC_Start();
    IIC_Send_Byte((AHT10_IIC_ADDR << 1) | 0); //����������ַ+д����

    if(IIC_Wait_Ack())          //�ȴ�Ӧ��
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(cmd);         //д�Ĵ�����ַ
    IIC_Wait_Ack();             //�ȴ�Ӧ��
    for(u8 i = 0; i < len; i++)
    {
        IIC_Send_Byte(data[i]);     //��������
        IIC_Wait_Ack();				//�ȴ�Ӧ��
    }
    IIC_Stop();
    return 0;
}

 /*	��һ���ֽ�*/
u8 AHT10_ReadOneByte(void)
{
    u8 res = 0;
    IIC_Start();
    IIC_Send_Byte((AHT10_IIC_ADDR << 1) | 0X01); //����������ַ+������
    if(IIC_Wait_Ack())          //�ȴ�Ӧ��
    {
        IIC_Stop();
        return 1;
    }
    res = IIC_Read_Byte(0);		//������,����nACK
    IIC_Stop();                 //����һ��ֹͣ����
    return res;
}

 /* ��ȡAHT10��״̬�Ĵ���*/
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
/*	������*/
u8 AHT10_Read_Data(u8 *data, u8 len)
{
    IIC_Start();
    IIC_Send_Byte((AHT10_IIC_ADDR << 1) | 0x01); //����������ַ+������
    if(IIC_Wait_Ack())          //�ȴ�Ӧ��
    {
        IIC_Stop();
        return 1;
    }
    for(u8 i = 0; i < len; i++)
    {
        if(i == (len - 1))
            data[i] = IIC_Read_Byte(0);		//������,����nACK
        else
            data[i] = IIC_Read_Byte(1);		//������,����ACK
    }
    IIC_Stop();
    return 0;
}

/*��ȡ�¶����� */
float AHT10_Read_Temperature(void)
{
    u8 res = 0;
    u8 cmd[2] = {0, 0};
    u8 temp[6];
    float cur_temp;
    if(AHT10_Read_Status())	return 0;
    res = AHT10_Write_Data(AHT10_GET_DATA, cmd, 2); //���Ͷ�ȡ��������
    if(res)	return 0;
    delay_ms(50);
    res = AHT10_Read_Data(temp, 6);				//��ȡ����
    if(res)	return 0;
    cur_temp = ((temp[3] & 0xf) << 16 | temp[4] << 8 | temp[5]) * 200.0 / (1 << 20) - 50;
    return cur_temp;
}

/*	��ȡʪ������*/
float AHT10_Read_Humidity(void)
{
    u8 res = 0;
    u8 cmd[2] = {0, 0};
    u8 humi[6];
    float cur_humi;
    if(AHT10_Read_Status())	return 0;
    res = AHT10_Write_Data(AHT10_GET_DATA, cmd, 2); //���Ͷ�ȡ��������
    if(res)	return 0;
    delay_ms(50);
    res = AHT10_Read_Data(humi, 6);				//��ȡ����
    if(res)	return 0;
    cur_humi = ((humi[1]) << 12 | humi[2] << 4 | (humi[3] & 0xF0)) * 100.0 / (1 << 20);
    return cur_humi;
}



/*ATH10��������ʼ��*/
u8 AHT10_Init(void)
{
    u8 res;
    u8 temp[2] = {0, 0};
    IIC_Init();		//��ʼ��IIC�ӿڣ�ע�������IIC����Ϊ��SCL-PD6 SDA-PC1
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

