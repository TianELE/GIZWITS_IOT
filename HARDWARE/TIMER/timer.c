#include "sys.h"
#include "timer.h"
#include "string.h"
#include "led.h"
#include "gizwits_product.h"

static u8 timer_id = 0;

/**
 * @brief	��ʱ����ʼ��,�ú������Զ�ε��ã������Դ�����6����ʱ��,���ǲ���ʹ�õĶ�ʱ����Ҫ�ݻ�,�����ظ�����TIMER��ʧ��
 *
 * @param   time	��ʱ�����ʱ�䣬��λms
 * @param   p		��ʱ���ص�����
 *
 * @return  u8 		�����ɹ�����:0~5
 *					����ʧ�ܷ��أ�0xFF
 */
u8 TIM_Init(u32 time, void (*p)(void *))
{
    struct tls_timer_cfg timer_cfg;
    memset(&timer_cfg, 0, sizeof(timer_cfg));
    timer_cfg.unit = TLS_TIMER_UNIT_MS;				//��ʱ��λ��ms��
    timer_cfg.timeout = time;						//���ʱ��
    timer_cfg.is_repeat = TRUE;						//����ѭ��
    timer_cfg.callback = p;	//��ʱ���ص�����
    timer_id = tls_timer_create(&timer_cfg);		//������ʱ��
    if(WM_TIMER_ID_INVALID != timer_id)
    {
        tls_timer_start(timer_id);					//������ʱ��
        return timer_id;
    }
    return WM_TIMER_ID_INVALID;
}

/**
 * @brief	������ʱ��
 *
 * @param   timer_id	��ʱ��ID��tls_timer_create�����ķ���ֵ
 *
 * @return  void
 */
void TimerStart(u8 timer_id)
{
    tls_timer_start(timer_id);
}

/**
 * @brief	ֹͣ��ʱ��
 *
 * @param   timer_id	��ʱ��ID��tls_timer_create�����ķ���ֵ
 *
 * @return  void
 */
void TimerStop(u8 timer_id)
{
    tls_timer_stop(timer_id);
}

/**
 * @brief	�ݻٶ�ʱ��
 *
 * @param   timer_id	��ʱ��ID��tls_timer_create�����ķ���ֵ
 *
 * @return  void
 */
void TimerDestory(u8 timer_id)
{
    tls_timer_destroy(timer_id);
}


/**
 * @brief	��ʱ���жϻص�����
 *
 * @param   arg		��ʱ���ص�������ڲ���
 *
 * @return  void
 */
void TIM_Callback_IRQHandler(void *arg)
{
   gizTimerMs();
}


