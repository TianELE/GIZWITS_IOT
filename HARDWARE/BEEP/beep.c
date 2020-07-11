#include "sys.h"
#include "beep.h"


/**
 * @brief	·äÃùÆ÷ IO³õÊ¼»¯º¯Êý
 *
 * @param   void
 *
 * @return  void
 */
void BEEP_Init(void)
{
    /*
    	BEEP	PB15
    */

    tls_gpio_cfg(WM_IO_PB_15, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);

    BEEP = 0;
}













