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
 *	������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 *	ALIENTEK W601������
 *	�ⲿ�ж���������
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


void EXTI_KEY0_IRQHandler(void *arg);
void EXTI_KEY1_IRQHandler(void *arg);
void EXTI_KEY_UP_IRQHandler(void *arg);
/**
 * @brief	�ⲿ�жϳ�ʼ������
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
    	KEY_UP 	- PA8	�ߵ�ƽ��Ч
    */
    tls_gpio_cfg(WM_IO_PA_06, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);	//IO��ʼ��
    tls_gpio_isr_register(WM_IO_PA_06, EXTI_KEY1_IRQHandler, NULL);			//�ص�����
    tls_gpio_irq_enable(WM_IO_PA_06, WM_GPIO_IRQ_TRIG_FALLING_EDGE);		//�ⲿ�жϳ�ʼ�� - �½���

    tls_gpio_cfg(WM_IO_PA_07, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);	//IO��ʼ��
    tls_gpio_isr_register(WM_IO_PA_07, EXTI_KEY0_IRQHandler, NULL);			//�ص�����
    tls_gpio_irq_enable(WM_IO_PA_07, WM_GPIO_IRQ_TRIG_FALLING_EDGE);		//�ⲿ�жϳ�ʼ�� - �½���

    tls_gpio_cfg(WM_IO_PA_08, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);	//IO��ʼ��
    tls_gpio_isr_register(WM_IO_PA_08, EXTI_KEY_UP_IRQHandler, NULL);		//�ص�����
    tls_gpio_irq_enable(WM_IO_PA_08, WM_GPIO_IRQ_TRIG_RISING_EDGE);			//�ⲿ�жϳ�ʼ�� - ������

}


/**
 * @brief	KEY0�жϻص�����
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

        if(KEY0 == 0)	//����LED_R��״̬��ת
            LED_R = !LED_R;
    }
}

/**
 * @brief	KEY1�жϻص�����
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

        if(KEY1 == 0)	//����LED_G��״̬��ת
            LED_G = !LED_G;
    }
}

/**
 * @brief	KEY_UP�жϻص�����
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

        if(KEY_UP == 1)	//���Ʒ�����״̬��ת
            BEEP = !BEEP;
    }
}




