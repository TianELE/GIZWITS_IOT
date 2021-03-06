#ifndef _MYIIC2_H
#define _MYIIC2_H
#include "sys.h"


/*********************************************************************************
			  ___   _     _____  _____  _   _  _____  _____  _   __
			 / _ \ | |   |_   _||  ___|| \ | ||_   _||  ___|| | / /
			/ /_\ \| |     | |  | |__  |  \| |  | |  | |__  | |/ /
			|  _  || |     | |  |  __| | . ` |  | |  |  __| |    \
			| | | || |_____| |_ | |___ | |\  |  | |  | |___ | |\  \
			\_| |_/\_____/\___/ \____/ \_| \_/  \_/  \____/ \_| \_/

 *	******************************************************************************
 *	本程序只供学习使用，未经作者许可，不得用于其它任何用途
 *	ALIENTEK W601开发板
 *	IIC驱动代码
 *	正点原子@ALIENTEK
 *	技术论坛:www.openedv.com
 *	创建日期:2019/7/10
 *	版本：V1.0
 *	版权所有，盗版必究。
 *	Copyright(C) 广州市星翼电子科技有限公司 2019-2029
 *	All rights reserved
 *	******************************************************************************
 *	初始版本
 *	******************************************************************************/

 /*
    	IIC_SCL2	PA2
    	IIC_SDA		PA1
    */

//IO方向设置
#define SDA2_IN() 	tls_gpio_cfg(WM_IO_PA_01, WM_GPIO_DIR_INPUT, 	WM_GPIO_ATTR_PULLHIGH);	//PC1输入模式
#define SDA2_OUT()  	tls_gpio_cfg(WM_IO_PA_01, WM_GPIO_DIR_OUTPUT, 	WM_GPIO_ATTR_PULLHIGH);	//PC1输出模式

//IO操作
#define IIC2_SCL		PAout(2)
#define IIC2_SDA		PAout(1)
#define READ2_SDA  	PAin(1)

//IIC所有操作函数
void IIC2_Init(void);                //初始化IIC的IO口
void IIC2_Start(void);				//发送IIC开始信号
void IIC2_Stop(void);	  			//发送IIC停止信号
void IIC2_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC2_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC2_Wait_Ack(void); 				//IIC等待ACK信号
void IIC2_Ack(void);					//IIC发送ACK信号
void IIC2_NAck(void);				//IIC不发送ACK信号

void IIC2_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC2_Read_One_Byte(u8 daddr, u8 addr);
#endif

