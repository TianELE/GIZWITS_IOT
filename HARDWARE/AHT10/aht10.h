#ifndef __AHT10_H
#define __AHT10_H
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
 *	AHT10驱动代码
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


#define AHT10_IIC_ADDR	0x38			//AHT10 IIC地址

#define AHT10_CALIBRATION_CMD 	0xE1 	//校准命令(上电后只需要发送一次)
#define AHT10_NORMAL_CMD 		0xA8 	//正常工作模式
#define AHT10_GET_DATA 			0xAC 	//读取数据命令

u8 AHT10_Init(void);
float AHT10_Read_Temperature(void);
float AHT10_Read_Humidity(void);

#endif



