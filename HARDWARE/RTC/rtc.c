#include "rtc.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "beep.h"
#include "time.h"


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
 *	RTC驱动代码
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



/**
 * @brief	获取RTC当前时间函数
 *
 * @param   tm		时间参数缓存
 *
 * @return  void
 */
void RTC_Get_Date(struct tm *data)
{
    tls_get_rtc(data);
}


/**
 * @brief	RTC时间设置函数
 *
 * @param   year	年
 * @param   month	月
 * @param   date	日
 * @param   hour	小时
 * @param   min		分钟
 * @param   sec		秒钟
 *
 * @return  void
 */
void RTC_Set_Date(u8 year, u8 month, u8 date, u8 hour, u8 min, u8 sec)
{
    struct tm rtc_time;

    rtc_time.tm_year 	= 	year;
    rtc_time.tm_mon 	= 	month;
    rtc_time.tm_mday 	= 	date;
    rtc_time.tm_hour 	= 	hour;
    rtc_time.tm_min 	= 	min;
    rtc_time.tm_sec 	= 	sec;
    tls_set_rtc(&rtc_time);
}
/**
 * @brief	RTC初始化函数
 *
 * @param   year	年
 * @param   month	月
 * @param   date	日
 * @param   hour	小时
 * @param   min		分钟
 * @param   sec		秒钟
 *
 * @return  void
 */
void RTC_Init(u8 year, u8 month, u8 date, u8 hour, u8 min, u8 sec)
{
    /*SystemInit函数中选择RTC时钟为内部32K时钟*/
    struct tm rtc_time;

    rtc_time.tm_year 	= 	year;
    rtc_time.tm_mon 	= 	month;
    rtc_time.tm_mday 	= 	date;
    rtc_time.tm_hour 	= 	hour;
    rtc_time.tm_min 	= 	min;
    rtc_time.tm_sec 	= 	sec;
    tls_set_rtc(&rtc_time);
}


/**
 * @brief	闹钟定时中断处理函数
 *
 * @param   arg		函数入口参数
 *
 * @return  void
 */
static void RTC_Alarm_IRQ(void *arg)
{
    printf("ALARM\r\n");
    BEEP = 1;	//蜂鸣器响
}

/**
 * @brief	设置闹钟时间
 *
 * @param   year	年
 * @param   month	月
 * @param   date	日
 * @param   hour	小时
 * @param   min		分钟
 * @param   sec		秒钟
 *
 * @return  void
 */
void RTC_Set_Alarm(u8 year, u8 month, u8 date, u8 hour, u8 min, u8 sec)
{
    struct tm rtc_time;

    rtc_time.tm_year 	= year;
    rtc_time.tm_mon 	= month;
    rtc_time.tm_mday 	= date;
    rtc_time.tm_hour 	= hour;
    rtc_time.tm_min 	= min;
    rtc_time.tm_sec 	= sec;
    tls_rtc_timer_start(&rtc_time);

    tls_rtc_isr_register(RTC_Alarm_IRQ, NULL);	//中断回调函数注册
}


