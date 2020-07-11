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
 *	������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 *	ALIENTEK W601������
 *	RTC��������
 *	����ԭ��@ALIENTEK
 *	������̳:www.openedv.com
 *	��������:2019/7/10
 *	�汾��V1.0
 *	��Ȩ���У�����ؾ���
 *	Copyright(C) ������������ӿƼ����޹�˾ 2019-2029
 *	All rights reserved
 *	******************************************************************************
 *	��ʼ�汾
 *	******************************************************************************/



/**
 * @brief	��ȡRTC��ǰʱ�亯��
 *
 * @param   tm		ʱ���������
 *
 * @return  void
 */
void RTC_Get_Date(struct tm *data)
{
    tls_get_rtc(data);
}


/**
 * @brief	RTCʱ�����ú���
 *
 * @param   year	��
 * @param   month	��
 * @param   date	��
 * @param   hour	Сʱ
 * @param   min		����
 * @param   sec		����
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
 * @brief	RTC��ʼ������
 *
 * @param   year	��
 * @param   month	��
 * @param   date	��
 * @param   hour	Сʱ
 * @param   min		����
 * @param   sec		����
 *
 * @return  void
 */
void RTC_Init(u8 year, u8 month, u8 date, u8 hour, u8 min, u8 sec)
{
    /*SystemInit������ѡ��RTCʱ��Ϊ�ڲ�32Kʱ��*/
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
 * @brief	���Ӷ�ʱ�жϴ�����
 *
 * @param   arg		������ڲ���
 *
 * @return  void
 */
static void RTC_Alarm_IRQ(void *arg)
{
    printf("ALARM\r\n");
    BEEP = 1;	//��������
}

/**
 * @brief	��������ʱ��
 *
 * @param   year	��
 * @param   month	��
 * @param   date	��
 * @param   hour	Сʱ
 * @param   min		����
 * @param   sec		����
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

    tls_rtc_isr_register(RTC_Alarm_IRQ, NULL);	//�жϻص�����ע��
}


