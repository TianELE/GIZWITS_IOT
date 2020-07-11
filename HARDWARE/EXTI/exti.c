#include "exti.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"

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
 *	外部中断驱动代码
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


void EXTI_KEY0_IRQHandler(void *arg);
void EXTI_KEY1_IRQHandler(void *arg);
void EXTI_KEY_UP_IRQHandler(void *arg);
/**
 * @brief	外部中断初始化函数
 *
 * @param   void
 *
 * @return  void
 */
void EXTI_Init(void)
{
    /*
    	KEY0 	- PA7
    	KEY1 	- PA6
    	KEY_UP 	- PA8	高电平有效
    */
    tls_gpio_cfg(WM_IO_PA_06, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);	//IO初始化
    tls_gpio_isr_register(WM_IO_PA_06, EXTI_KEY1_IRQHandler, NULL);			//回调函数
    tls_gpio_irq_enable(WM_IO_PA_06, WM_GPIO_IRQ_TRIG_FALLING_EDGE);		//外部中断初始化 - 下降沿

    tls_gpio_cfg(WM_IO_PA_07, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);	//IO初始化
    tls_gpio_isr_register(WM_IO_PA_07, EXTI_KEY0_IRQHandler, NULL);			//回调函数
    tls_gpio_irq_enable(WM_IO_PA_07, WM_GPIO_IRQ_TRIG_FALLING_EDGE);		//外部中断初始化 - 下降沿

    tls_gpio_cfg(WM_IO_PA_08, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);	//IO初始化
    tls_gpio_isr_register(WM_IO_PA_08, EXTI_KEY_UP_IRQHandler, NULL);		//回调函数
    tls_gpio_irq_enable(WM_IO_PA_08, WM_GPIO_IRQ_TRIG_RISING_EDGE);			//外部中断初始化 - 上升沿

}


/**
 * @brief	KEY0中断回调函数
 *
 * @param   void
 *
 * @return  void
 */
void EXTI_KEY0_IRQHandler(void *arg)
{
    u16 res;

    res = tls_get_gpio_irq_status(WM_IO_PA_07);

    if(res)
    {
        tls_clr_gpio_irq_status(WM_IO_PA_07);

        delay_ms(50);

        if(KEY0 == 0)	//控制LED_R灯状态翻转
            LED_R = !LED_R;
    }
}

/**
 * @brief	KEY1中断回调函数
 *
 * @param   void
 *
 * @return  void
 */
void EXTI_KEY1_IRQHandler(void *arg)
{
    u16 res;

    res = tls_get_gpio_irq_status(WM_IO_PA_06);

    if(res)
    {
        tls_clr_gpio_irq_status(WM_IO_PA_06);

        delay_ms(50);

        if(KEY1 == 0)	//控制LED_G灯状态翻转
            LED_G = !LED_G;
    }
}

/**
 * @brief	KEY_UP中断回调函数
 *
 * @param   void
 *
 * @return  void
 */
void EXTI_KEY_UP_IRQHandler(void *arg)
{
    u16 res;

    res = tls_get_gpio_irq_status(WM_IO_PA_08);

    if(res)
    {
        tls_clr_gpio_irq_status(WM_IO_PA_08);

        delay_ms(50);

        if(KEY_UP == 1)	//控制蜂鸣器状态翻转
            BEEP = !BEEP;
    }
}




