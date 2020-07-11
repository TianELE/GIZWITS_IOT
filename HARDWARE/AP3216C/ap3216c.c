#include "ap3216c.h"
#include "myiic2.h"
#include "delay.h"


/*��ʼ��AP3216C*/
u8 AP3216C_Init(void)
{
    u8 temp = 0;
    IIC2_Init();         				//��ʼ��IIC
    AP3216C_WriteOneByte(0x00, 0X04);	//��λAP3216C
    delay_ms(50);						//AP33216C��λ����10ms
    AP3216C_WriteOneByte(0x00, 0X03);	//����ALS��PS+IR
    temp = AP3216C_ReadOneByte(0X00);		//��ȡ�ո�д��ȥ��0X03

    if(temp == 0X03)return 0;				//AP3216C����

    else return 1;						//AP3216Cʧ��
}

/*��ȡAP3216C������,��ȡԭʼ���ݣ�����ALS,PS��IR*/
void AP3216C_ReadData(u16* ir, u16* ps, u16* als)
{
    u8 buf[6];
    u8 i;

    for(i = 0; i < 6; i++)
    {
        buf[i] = AP3216C_ReadOneByte(0X0A + i);		//ѭ����ȡ���д���������
    }

    if(buf[0] & 0X80)*ir = 0;						//IR_OFλΪ1,��������Ч

    else *ir = ((u16)buf[1] << 2) | (buf[0] & 0X03); 	//��ȡIR������������

    *als = ((u16)buf[3] << 8) | buf[2];				//��ȡALS������������

    if(buf[4] & 0x40)*ps = 0;    					//IR_OFλΪ1,��������Ч

    else *ps = ((u16)(buf[5] & 0X3F) << 4) | (buf[4] & 0X0F); //��ȡPS������������
}

/* IICдһ���ֽ�*/
u8 AP3216C_WriteOneByte(u8 reg, u8 data)
{
    IIC2_Start();
    IIC2_Send_Byte(AP3216C_ADDR | 0X00); //����������ַ+д����

    if(IIC2_Wait_Ack())          //�ȴ�Ӧ��
    {
        IIC2_Stop();
        return 1;
    }

    IIC2_Send_Byte(reg);         //д�Ĵ�����ַ
    IIC2_Wait_Ack();             //�ȴ�Ӧ��
    IIC2_Send_Byte(data);        //��������

    if(IIC2_Wait_Ack())          //�ȴ�ACK
    {
        IIC2_Stop();
        return 1;
    }

    IIC2_Stop();
    return 0;
}

/*IIC��һ���ֽ�*/
u8 AP3216C_ReadOneByte(u8 reg)
{
    u8 res;
    IIC2_Start();
    IIC2_Send_Byte(AP3216C_ADDR | 0X00); //����������ַ+д����
    IIC2_Wait_Ack();             //�ȴ�Ӧ��
    IIC2_Send_Byte(reg);         //д�Ĵ�����ַ
    IIC2_Wait_Ack();             //�ȴ�Ӧ��
    IIC2_Start();
    IIC2_Send_Byte(AP3216C_ADDR | 0X01); //����������ַ+������
    IIC2_Wait_Ack();             //�ȴ�Ӧ��
    res = IIC2_Read_Byte(0);		//������,����nACK
    IIC2_Stop();                 //����һ��ֹͣ����
    return res;
}

