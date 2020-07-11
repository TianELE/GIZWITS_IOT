#ifndef __CCS811iic_H
#define __CCS811iic_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	  
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//myiic 代码	   
//by lty
//技术论坛:ltyworking@outlook.com
//修改日期:2018/08/02
//版本：V1.0
//版权所有，盗版必究。
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//IO方向设置
 
#define SDA3_IN()  {tls_gpio_cfg(WM_IO_PB_18, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_PULLHIGH);}
#define SDA3_OUT() {tls_gpio_cfg(WM_IO_PB_18, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);}

//IO操作函数	 
#define IIC3_SCL    PBout(17) //SCL
#define IIC3_SDA    PBout(18) //SDA	 
#define READ3_SDA   PBin(18)  //输入SDA 

//#define SCL_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
//#define SDA_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
//IIC所有操作函数
void IIC3_Init(void);                //初始化IIC的IO口				 
u8 IIC3_Start(void);				//发送IIC开始信号
void IIC3_Stop(void);	  			//发送IIC停止信号
void IIC3_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC3_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC3_Wait_Ack(void); 				//IIC等待ACK信号
void IIC3_Ack(void);					//IIC发送ACK信号
void IIC3_NAck(void);				//IIC不发送ACK信号

u8 Single_WriteIIC3_byte(u8 Slave_Address,u8 REG_Address,u8 data);
u8 Single_MWriteIIC3_byte(u8 Slave_Address,u8 REG_Address,u8 const *data,u8 length);
u8 Single_ReadIIC3(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length);
#endif
















