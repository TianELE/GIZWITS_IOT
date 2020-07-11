#include "sys.h"
#include "timer.h"
#include "string.h"
#include "led.h"
#include "gizwits_product.h"

static u8 timer_id = 0;

/**
 * @brief	定时器初始化,该函数可以多次调用，最多可以创建啊6个定时器,但是不再使用的定时器需要摧毁,否则重复申请TIMER会失败
 *
 * @param   time	定时器溢出时间，单位ms
 * @param   p		定时器回调函数
 *
 * @return  u8 		创建成功返回:0~5
 *					创建失败返回：0xFF
 */
u8 TIM_Init(u32 time, void (*p)(void *))
{
    struct tls_timer_cfg timer_cfg;
    memset(&timer_cfg, 0, sizeof(timer_cfg));
    timer_cfg.unit = TLS_TIMER_UNIT_MS;				//计时单位（ms）
    timer_cfg.timeout = time;						//溢出时间
    timer_cfg.is_repeat = TRUE;						//周期循环
    timer_cfg.callback = p;	//定时器回调函数
    timer_id = tls_timer_create(&timer_cfg);		//创建定时器
    if(WM_TIMER_ID_INVALID != timer_id)
    {
        tls_timer_start(timer_id);					//启动定时器
        return timer_id;
    }
    return WM_TIMER_ID_INVALID;
}

/**
 * @brief	启动定时器
 *
 * @param   timer_id	定时器ID，tls_timer_create函数的返回值
 *
 * @return  void
 */
void TimerStart(u8 timer_id)
{
    tls_timer_start(timer_id);
}

/**
 * @brief	停止定时器
 *
 * @param   timer_id	定时器ID，tls_timer_create函数的返回值
 *
 * @return  void
 */
void TimerStop(u8 timer_id)
{
    tls_timer_stop(timer_id);
}

/**
 * @brief	摧毁定时器
 *
 * @param   timer_id	定时器ID，tls_timer_create函数的返回值
 *
 * @return  void
 */
void TimerDestory(u8 timer_id)
{
    tls_timer_destroy(timer_id);
}


/**
 * @brief	定时器中断回调函数
 *
 * @param   arg		定时器回调函数入口参数
 *
 * @return  void
 */
void TIM_Callback_IRQHandler(void *arg)
{
   gizTimerMs();
}


