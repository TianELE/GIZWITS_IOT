/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "led.h"
#include "lcd.h"
#include "beep.h"
#include "usart2.h"
#include "delay.h"

static uint32_t timerMsCount;
extern u8 wifi_sta;
extern u8 flag;
extern u8 machineflag;
extern u8 lightflag;
extern u8 acflag;
extern u8 waterflag;
extern u8 fanflag;
extern u8 autoflag;
u8 cout0=0;
u8 cout2=0;
u8 cout3=0;
char buff[100]={0};
/** Current datapoint */
dataPoint_t currentDataPoint;

/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {

      case EVENT_CTRL:
        currentDataPoint.valueCTRL = dataPointPtr->valueCTRL;
        GIZWITS_LOG("Evt: EVENT_CTRL %d\n", currentDataPoint.valueCTRL);
        switch(currentDataPoint.valueCTRL)
        {
          case CTRL_VALUE0:
             if(lightflag==lightoff){cout0++;lightflag=lighton;LED_R = 0;LED_G = 0;LED_B = 0;LCD_ShowString(140, 160, 100, 16, 16, "Light OPEN   ");}
             if(lightflag==lighton&&cout0==0){lightflag=lightoff;LED_R = 1;LED_G = 1;LED_B = 1;LCD_ShowString(140, 160, 100, 16, 16, "Light CLOSE  ");}
             cout0=0;
            //user handle
            break;
          case CTRL_VALUE1:
             BEEP=0;
            //user handle
            break;
          case CTRL_VALUE2:
             if(machineflag==sunoff) {
                cout2++;
                M_N=0;
                M_P=1;
                delay_ms(800);
                M_P=0;
                machineflag=sunon;
                LCD_ShowString(140, 140, 100, 16, 16, "Sun OPEN     ");
             }
             if(machineflag==sunon&&cout2==0) {
                M_P=0;
                M_N=1;
                delay_ms(800);
                M_N=0;
                machineflag=sunoff;
                LCD_ShowString(140, 140, 100, 16, 16, "Sun CLOSE    ");
             }
             cout2=0;
            //user handle
            break;
          case CTRL_VALUE3:
             if(autoflag==autooff){
                cout3++;
               autoflag=autoon;
               LCD_ShowString(0,240-16,80,16,16,"AUTO Ctrl");               
             }
             if(autoflag==autoon&&cout3==0){
                //cout3=0;
                autoflag=autooff;
                BEEP=0;
                LED_R = 1;
                LED_G = 1;
                LED_B = 1;
                M_N=0;
                M_P=0;
                LCD_ShowString(120, 0, 120, 16, 16, "               ");
                LCD_ShowString(120, 20, 120, 16, 16, "               ");
                LCD_ShowString(120, 40, 120, 16, 16, "               ");
                LCD_ShowString(120, 60, 120, 16, 16, "               ");
                LCD_ShowString(120, 80, 120, 16, 16, "               ");
                LCD_ShowString(140, 100, 100, 16, 16, "AirC CLOSE   ");
                LCD_ShowString(140, 120, 100, 16, 16, "Water CLOSE  ");
                LCD_ShowString(140, 140, 100, 16, 16, "Sun CLOSE    ");
                LCD_ShowString(140, 140, 100, 16, 16, "Light CLOSE  ");
                LCD_ShowString(140, 160, 100, 16, 16, "FAN CLOSE    ");
                LCD_ShowString(0,240-16,80,16,16,"HAND Ctrl");
             }
             cout3=0;
//             if(acflag==acoff){LCD_ShowString(140, 100, 100, 16, 16, "AirC OPEN    ");}
//             if(acflag==acon){LCD_ShowString(140, 100, 100, 16, 16, "AirC CLOSE   ");}
//             if(waterflag==wateron){LCD_ShowString(140, 120, 100, 16, 16, "Water CLOSE");}
//             if(waterflag==wateroff){LCD_ShowString(140, 120, 100, 16, 16, "Water OPEN");}
//             if(fanflag==fanoff){LCD_ShowString(140, 160, 100, 16, 16, "FAN OPEN");}
//             if(fanflag==fanon){LCD_ShowString(140, 160, 100, 16, 16, "FAN CLOSE");}
            //user handle
            break;
          default:
            break;
        }
        break;



      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
 
        break;
      case WIFI_DISCON_ROUTER:
 
        break;
      case WIFI_CON_M2M:
         wifi_sta=1;    //连接标志
        break;
      case WIFI_DISCON_M2M:
         wifi_sta=0;
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
         sprintf((char*)buff,"%d-%d-%d %02d:%02d:%02d",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second);
			LCD_ShowString(90,240-16,150,16,16,buff);
			memset(buff,0,100);
        //GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
        break;
      case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
      #else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
    break;
      default:
        break;
    }
  }

  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
//void userHandle(void)
//{
// /*
//    currentDataPoint.valueTEMP = ;//Add Sensor Data Collection
//    currentDataPoint.valueHUMI = ;//Add Sensor Data Collection
//    currentDataPoint.valueALS = ;//Add Sensor Data Collection
//    currentDataPoint.valueTEMP_S = ;//Add Sensor Data Collection
//    currentDataPoint.valueCO2 = ;//Add Sensor Data Collection

//    */
//    
//}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    
      currentDataPoint.valueCTRL = 0;
      currentDataPoint.valueTEMP = 0;
      currentDataPoint.valueHUMI = 0;
      currentDataPoint.valueALS = 0;
      currentDataPoint.valueTEMP_S = 30;
      currentDataPoint.valueCO2 =0 ;
    

}


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
	__set_FAULTMASK(1);//关闭所有中断
   NVIC_SystemReset();//复位
}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
  gizTimerMs();
}

/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
  uint8_t value = 0;
  //value = USART_ReceiveData(USART2);//STM32 test demo
  gizPutData(&value, 1);
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1，Input Param Illegal
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;   
    if(NULL == buf)
    {
        return -1;
    }   
    #ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);
    }
    GIZWITS_LOG("\n");
    #endif
   for(i=0; i<len; i++)
   {
      tls_reg_write32(HR_UART2_TX_WIN, buf[i]);
      while(tls_reg_read32(HR_UART2_FIFO_STATUS) & 0x3F);	//等待发送完毕
     if(i >=2 && buf[i] == 0xFF)
     {
         tls_reg_write32(HR_UART2_TX_WIN, 0x55);
         while(tls_reg_read32(HR_UART2_FIFO_STATUS) & 0x3F);	//等待发送完毕
     }
   }


    
    return len;
}


