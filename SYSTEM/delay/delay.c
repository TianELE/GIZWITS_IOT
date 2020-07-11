#include "delay.h"
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
 *	使用SysTick的普通计数模式对延迟进行管理
 *	包括delay_us,delay_ms
 *	正点原子@ALIENTEK
 *	技术论坛:www.openedv.com
 *	创建日期:2019/7/10
 *	版本：V1.0
 *	版权所有，盗版必究。
 *	Copyright(C) 广州市星翼电子科技有限公司 2019-2029
 *	All rights reserved
 *	******************************************************************************
 *	修改说明
 *	******************************************************************************/

static u8  fac_us = 0;							//us延时倍乘数
static u32 fac_ms = 0;							//ms延时倍乘数,在os下,代表每个节拍的ms数


/**
 * @brief	初始化延迟函数
 *
 * @remark	滴答时钟频率：sysclk(MHz)  SysTick_CLKSource_HCLK:不分频
 *			如果时钟为80MHz,滴答定时器最大计时时间为：2^24/80≈209ms
 *
 * @param   sysclk	系统时钟频率
 *
 * @return  void
 */
void delay_init(u8 sysclk)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

    fac_us = sysclk;	//每个us需要的systick时钟数

    fac_ms = (u16)fac_us * 1000; //每个ms需要的systick时钟数
}

/**
 * @brief	延时微秒(us)函数
 *
 * @remark	nus:0~209ms(最大值即2^24/80≈209ms)
 *
 * @param   nus		需要延时多少微秒
 *
 * @return  void
 */
void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD = nus * fac_us; 					//时间加载
    SysTick->VAL = 0x00;        					//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//开始倒数

    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16)));		//等待时间到达

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
    SysTick->VAL = 0X00;      					 //清空计数器
}
/**
 * @brief	延时毫秒(ms)函数
 *
 * @param   nms		需要延时多少毫秒,如果时钟为80MHz：2^24/80(us)≈209ms
 *
 * @return  void
 */
void delay_xms(u16 nms)
{
    u32 temp;
    SysTick->LOAD = (u32)nms * fac_ms;			//时间加载(SysTick->LOAD为24bit)
    SysTick->VAL = 0x00;           			//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;        //开始倒数

    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16)));	//等待时间到达

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;     //关闭计数器
    SysTick->VAL = 0X00;     		  		//清空计数器
}

/**
 * @brief	延时毫秒(ms)函数
 *
 * @param   nms		需要延时多少毫秒,范围0~65535
 *
 * @return  void
 */
void delay_ms(u16 nms)
{
    u16 repeat = nms / 100;

    u16 remain = nms % 100;

    while(repeat)
    {
        delay_xms(100);
        repeat--;
    }

    if(remain)delay_xms(remain);
}





































