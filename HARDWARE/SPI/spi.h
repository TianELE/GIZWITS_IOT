#ifndef __SPI_H
#define __SPI_H

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
 *	SPI驱动代码
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


void SPI_Init(void);

u8 SPI_WriteByte(u8 *TxData, u32 size);
u8 SPI_ReadByte(u8 *RxData, u32 size);
void SPI_SetLowSpeed(void);
void SPI_SetHightSpeed(void);
#endif



