#include "led.h"

/**
 * @brief	LED IO³õÊ¼»¯º¯Êý
 *
 * @param   void
 *
 * @return  void
 */
void LED_Init(void)
{
    /*
    	LED-B	PA13
    	LED-G	PA14
    	LED-R	PA15
    */

    tls_gpio_cfg(WM_IO_PA_13, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
    tls_gpio_cfg(WM_IO_PA_14, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
    tls_gpio_cfg(WM_IO_PA_15, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);

    LED_R = 1;
    LED_G = 1;
    LED_B = 1;
}





