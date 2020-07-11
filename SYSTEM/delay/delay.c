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
 *	������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 *	ALIENTEK W601������
 *	ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���
 *	����delay_us,delay_ms
 *	����ԭ��@ALIENTEK
 *	������̳:www.openedv.com
 *	��������:2019/7/10
 *	�汾��V1.0
 *	��Ȩ���У�����ؾ���
 *	Copyright(C) ������������ӿƼ����޹�˾ 2019-2029
 *	All rights reserved
 *	******************************************************************************
 *	�޸�˵��
 *	******************************************************************************/

static u8  fac_us = 0;							//us��ʱ������
static u32 fac_ms = 0;							//ms��ʱ������,��os��,����ÿ�����ĵ�ms��


/**
 * @brief	��ʼ���ӳٺ���
 *
 * @remark	�δ�ʱ��Ƶ�ʣ�sysclk(MHz)  SysTick_CLKSource_HCLK:����Ƶ
 *			���ʱ��Ϊ80MHz,�δ�ʱ������ʱʱ��Ϊ��2^24/80��209ms
 *
 * @param   sysclk	ϵͳʱ��Ƶ��
 *
 * @return  void
 */
void delay_init(u8 sysclk)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

    fac_us = sysclk;	//ÿ��us��Ҫ��systickʱ����

    fac_ms = (u16)fac_us * 1000; //ÿ��ms��Ҫ��systickʱ����
}

/**
 * @brief	��ʱ΢��(us)����
 *
 * @remark	nus:0~209ms(���ֵ��2^24/80��209ms)
 *
 * @param   nus		��Ҫ��ʱ����΢��
 *
 * @return  void
 */
void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD = nus * fac_us; 					//ʱ�����
    SysTick->VAL = 0x00;        					//��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//��ʼ����

    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16)));		//�ȴ�ʱ�䵽��

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
    SysTick->VAL = 0X00;      					 //��ռ�����
}
/**
 * @brief	��ʱ����(ms)����
 *
 * @param   nms		��Ҫ��ʱ���ٺ���,���ʱ��Ϊ80MHz��2^24/80(us)��209ms
 *
 * @return  void
 */
void delay_xms(u16 nms)
{
    u32 temp;
    SysTick->LOAD = (u32)nms * fac_ms;			//ʱ�����(SysTick->LOADΪ24bit)
    SysTick->VAL = 0x00;           			//��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;        //��ʼ����

    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16)));	//�ȴ�ʱ�䵽��

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;     //�رռ�����
    SysTick->VAL = 0X00;     		  		//��ռ�����
}

/**
 * @brief	��ʱ����(ms)����
 *
 * @param   nms		��Ҫ��ʱ���ٺ���,��Χ0~65535
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





































