#ifndef __CCS811iic_H
#define __CCS811iic_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	  
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//myiic ����	   
//by lty
//������̳:ltyworking@outlook.com
//�޸�����:2018/08/02
//�汾��V1.0
//��Ȩ���У�����ؾ���
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//IO��������
 
#define SDA3_IN()  {tls_gpio_cfg(WM_IO_PB_18, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_PULLHIGH);}
#define SDA3_OUT() {tls_gpio_cfg(WM_IO_PB_18, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);}

//IO��������	 
#define IIC3_SCL    PBout(17) //SCL
#define IIC3_SDA    PBout(18) //SDA	 
#define READ3_SDA   PBin(18)  //����SDA 

//#define SCL_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
//#define SDA_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
//IIC���в�������
void IIC3_Init(void);                //��ʼ��IIC��IO��				 
u8 IIC3_Start(void);				//����IIC��ʼ�ź�
void IIC3_Stop(void);	  			//����IICֹͣ�ź�
void IIC3_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC3_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC3_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC3_Ack(void);					//IIC����ACK�ź�
void IIC3_NAck(void);				//IIC������ACK�ź�

u8 Single_WriteIIC3_byte(u8 Slave_Address,u8 REG_Address,u8 data);
u8 Single_MWriteIIC3_byte(u8 Slave_Address,u8 REG_Address,u8 const *data,u8 length);
u8 Single_ReadIIC3(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length);
#endif
















