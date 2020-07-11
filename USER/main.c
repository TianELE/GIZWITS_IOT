#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usart2.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "spi.h"
#include "beep.h"
#include "timer.h"
#include "aht10.h"
#include "ap3216c.h"
#include <string.h>
#include "gizwits_product.h"
#include "common.h"
#include "ds18b20.h"
#include "CCS811.h"
#include "stdio.h"

#define tempflag 1
#define humiflag 2
#define alsflag 3
#define co2flag 4

u8 wifi_sta=0;
u8 st1 = 0;
extern   ccs811_measurement_t CCS;
u8 machineflag=sunoff;
u8 lightflag=lightoff;
u8 acflag=acoff;
u8 waterflag=wateroff;
u8 fanflag=fanoff;
u8 flag=0;
u8 autoflag=autoon;
void dataCtrl(float temp,float humi,short temp_s,u16 ALS,u16 CO2 );
//Э���ʼ��
void Gizwits_Init(void)
{
	TIM_Init(1, TIM_Callback_IRQHandler);//1MSϵͳ��ʱ
	uart2_init(9600);//WIFI��ʼ�������ʱ���Ϊ9600
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();
}
//////////////////////////////
void M_Init(void)
{
    /*
    	P	PB12
    	N	PB10
    */

    tls_gpio_cfg(WM_IO_PB_10, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);
    tls_gpio_cfg(WM_IO_PB_12, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);

    M_P = 0;
    M_N = 0;
}
/****************************C02ģ��***********/
void CCS881_CO2(){
   st1 = CCS811_ReadOneByte(0x00);
   switch(st1)
   {
      case 0x90:
      {
              setMode(MODE1);
      }break;
      case 0x98:
      {
        getData();
        delay_ms(10);
        if(CCS.eco2<10000)
        {
                printf("co2 = %d tvoc = %d\r\n",CCS.eco2,CCS.tvoc);
        }
        delay_ms(100);
      }break;
      case 0x10:
      {
        app_Start();
        printf("CCS811 Tries to enter App Mode!\r\n");
        delay_ms(100);
      }
      case 0x99:
      {
        st1 = CCS811_ReadOneByte(0xE0);
        printf("ERROR = %d\r\n",st1);
      }
      default:
      {
        reset();
        printf("CCS811 Tries to reset\r\n");
        delay_ms(100);
      }break;
   }
}
//���ݲɼ�
void userHandle(void)
{
   u16 ir, als, ps;
	static u8 t=0;
	static u8 tt=0;
	static u8 fl=0;
	static float temperature, humidity;
   static short temps;
	if(flag!=0){
	currentDataPoint.valueCTRL = 1;
   }
if(wifi_sta){
//	printf("WIFI_OK\r\n");
   if(fl==0){
   Display_imagewifi1(190,190); //��ʾwifiͼ��
      fl=1; 
   }
   if(tt>=9){
      tt=0;
      gizwitsGetNTP();  //����ʱ�� Լ1s��ȡһ��
   }
   if(t >= 20) //ÿ2000ms��ȡһ�θ������ **��ƽ̨���ƣ�ÿ��������ϴ�15������**
     {
         t=0;
         temperature = AHT10_Read_Temperature();//��ȡ�¶�
         humidity = AHT10_Read_Humidity();//��ȡʪ��
         AP3216C_ReadData(&ir, &ps, &als);		//��ȡ ir���� ps�ӽ��� als��ǿ�������ǿ��
         temps=DS18B20_Get_Temp();//��ȡ�����¶�
         CCS881_CO2();//��ȡco2����
         currentDataPoint.valueTEMP = temperature ;//�¶�����
         currentDataPoint.valueHUMI = humidity/100;//ʪ������
         currentDataPoint.valueALS=als;//��ǿ����
         currentDataPoint.valueTEMP_S=temps/10.0;//�����¶�����
         currentDataPoint.valueCO2=CCS.eco2;//C02�����ϴ�
         if(temperature < 0)
         {
             LCD_ShowChar(30 + 40, 190, '-', 16);	//��ʾ����
             temperature = -temperature;				//תΪ����
         }
         else
             LCD_ShowChar(30 + 40, 190, ' ', 16);	//ȥ������
         
         LCD_ShowNum(30 + 40, 130, CCS.eco2, 5, 16);	//��ʾCO2
         LCD_ShowNum(30 + 40, 150, als, 5, 16);	//��ʾALS����
         LCD_ShowNum(30 + 64, 170, temps/10, 2, 16);	//��ʾ�����¶�����
         LCD_ShowNum(30 + 88, 170, temps%10, 1, 16);	//��ʾ�����¶�С��
         LCD_ShowNum(30 + 48, 190, temperature, 2, 16);					//��ʾ�¶�����
         LCD_ShowNum(30 + 72, 190, (u32)(temperature * 10) % 10, 1, 16);	//��ʾ�¶�С��
         LCD_ShowNum(30 + 48, 210, humidity, 2, 16);						//��ʾʪ������
         LCD_ShowNum(30 + 72, 210, (u32)(humidity * 10) % 10, 1, 16);	//��ʾʪ��С��
         if(autoflag==autoon){
            LCD_ShowString(0,240-16,80,16,16,"AUTO Ctrl");
            dataCtrl(temperature,humidity,temps/10,als,CCS.eco2);//�¼��Զ�ִ�к���
         }
         }
         t++;
         tt++;
   }
else{
   Display_imagewifi2(190,190);//��ʾwifiͼ��
   printf("WIFI_error\r\n");
   fl=0;
}
////////////////////////////
}
int main(void)
{  
   int key;
   key=0;
   delay_init(80);			//��ʱ������ʼ��
   uart_init(115200);		//���ڳ�ʼ��
   CCS811Init();           //co2��ʼ��
   M_Init();
   LED_Init();				//LED�ӿڳ�ʼ��
   KEY_Init();
   SPI_Init();				//��ʼ��SPI�ӿ�
   LCD_Init();				//��ʼ��LCD
   userInit();
   Gizwits_Init();
   gizwitsInit();
   //	Display_image1(0,0);
   Display_image2(0,0);
   Display_image_als(15,150);
   Display_image_t(15,170);
   Display_image_t(15,190);
   Display_image_h(15,208);
	
   while(AHT10_Init())			//��ʼ��AHT10
   {
     LCD_ShowString(130, 90, 110, 16, 16, "AHT10 Error");
     delay_ms(200);
     LCD_Fill(130, 90, 240, 90 + 16, WHITE);
     delay_ms(100);
   }
   while(AP3216C_Init())		//��ʼ��AP3216C
    {
        LCD_ShowString(130, 110, 110, 16, 16, "AP3216C Error");
       delay_ms(200);
         LCD_Fill(130, 110, 240, 110 + 16, WHITE);
        delay_ms(200);
    }
//   LCD_ShowString(130, 90, 110, 16, 16, "AHT10 OK");
//   LCD_ShowString(130, 110, 110, 16, 16, "AP3216C OK");
   POINT_COLOR = BLUE; //��������Ϊ��ɫ
    LCD_ShowString(30, 130, 120, 16, 16, "CO2:      PPM");
   LCD_ShowString(30, 150, 120, 16, 16, "ALS:      LUX");
    LCD_ShowString(30, 170, 120, 16, 16, "T_soil:   . C");
   LCD_ShowString(30, 190, 100, 16, 16, "Temp:   . C");
   LCD_ShowString(30, 210, 100, 16, 16, "Humi:   . %RH");
    
   while(1)
   {
     userHandle();        
     gizwitsHandle((dataPoint_t *)&currentDataPoint);
         key = KEY_Scan(0);
         if(key==KEY1_PRES)//KEY1����
         {
               printf("WIFI����AirLink����ģʽ\r\n");
               gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
         }
         if(key==WKUP_PRES)//KEY_UP����
         {
               printf("WIFI��λ����������������\r\n");
               gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ
               wifi_sta=0;//��־wifi�ѶϿ�		
         }
         if(key==KEY0_PRES){
            autoflag=autoon;
            BEEP=0;
            LED_R = 1;
            LED_G = 1;
            LED_B = 1;
            M_N=0;
            M_P=0;
            machineflag=sunoff;
            lightflag=lightoff;
            acflag=acoff;
            waterflag=wateroff;
            fanflag=fanoff;
             CCS881_CO2();
            LCD_ShowString(140, 100, 100, 16, 16, "AirC CLOSE   ");
            LCD_ShowString(140, 120, 100, 16, 16, "Water CLOSE  ");
            LCD_ShowString(140, 140, 100, 16, 16, "Sun CLOSE    ");
            LCD_ShowString(140, 140, 100, 16, 16, "Light CLOSE  ");
            LCD_ShowString(140, 160, 100, 16, 16, "FAN CLOSE    ");
         }
   }
   }
/***************************************************************************/
void dataCtrl(float temp,float humi,short temp_s,u16 ALS,u16 CO2 ){
   if((temp>20&&temp<35)&&(temp_s>23&&temp_s<32)){
      flag=0;
      acflag=acoff;
      LCD_ShowString(120, 0, 120, 16, 16, "               ");
      LCD_ShowString(120, 20, 120, 16, 16, "               ");
      LCD_ShowString(140, 100, 100, 16, 16, "AirC CLOSE   ");
   }
   if(temp>=35||temp_s>=32){
      if(temp>=35) LCD_ShowString(120, 0, 120, 16, 16, "air temp over  ");
      if(temp_s>=35) LCD_ShowString(120, 20, 120, 16, 16, "soil temp over ");
      BEEP=1;
      delay_ms(100);
      BEEP=0;
      flag=tempflag;
      acflag=acon;
      LCD_ShowString(140, 100, 100, 16, 16, "AirC OPEN    ");
   }
   if(temp<=20||temp_s<=23){
      if(temp<=20) LCD_ShowString(120, 0, 120, 16, 16, "air temp low   ");
      if(temp_s<=23) LCD_ShowString(120, 20, 120, 16, 16, "soil temp low  ");
      BEEP=1;
      delay_ms(100);
      BEEP=0;
      flag=tempflag;
      acflag=acon;
      LCD_ShowString(140, 100, 100, 16, 16, "AirC OPEN    ");
   }
   if(humi>30){
      flag=0;
      waterflag=wateroff;
      LCD_ShowString(120, 40, 120, 16, 16, "               ");
      LCD_ShowString(140, 120, 100, 16, 16, "Water CLOSE  ");
   }
   if(humi<=30){
      LCD_ShowString(120, 40, 120, 16, 16, "air humi low ");
      BEEP=1;
      delay_ms(100);
      BEEP=0;
      flag=humiflag;
      waterflag=wateron;
      LCD_ShowString(140, 120, 100, 16, 16, "Water OPEN   ");
   }
   if(ALS>20&&ALS<2000){
      flag=0;
      LCD_ShowString(140, 140, 100, 16, 16, "Sun CLOSE    ");
      LCD_ShowString(140, 160, 100, 16, 16, "Light CLOSE  ");
      LCD_ShowString(120, 60, 120, 16, 16, "               ");
   }
   if(ALS<=20||ALS>=2000){
      if(ALS<=20){ 
         LCD_ShowString(120, 60, 120, 16, 16, "Light low      ");
         BEEP=1;
         delay_ms(100);
         BEEP=0;
         LED_R = 0;
         LED_G = 0;
         LED_B = 0;
         flag=alsflag;
         if(machineflag==sunoff){
            M_N=0;
            M_P=1;
            delay_ms(800);
            M_P=0;
            machineflag=sunon;
         }
         lightflag=lighton;
         LCD_ShowString(140, 140, 100, 16, 16, "Sun OPEN     ");
         LCD_ShowString(140, 160, 100, 16, 16, "Light OPEN   ");
      }
      if(ALS>=2000) {
         LCD_ShowString(120, 60, 120, 16, 16, "Light over     ");
         BEEP=1;
         delay_ms(100);
         BEEP=0;
         LED_R = 1;
         LED_G = 1;
         LED_B = 1;
         flag=alsflag;
         if(machineflag==sunon){
            M_P=0;
            M_N=1;
            delay_ms(800);
            M_N=0;
            machineflag=sunoff;
         }
         lightflag=lightoff;
         LCD_ShowString(140, 140, 100, 16, 16, "Sun CLOSE    ");
         LCD_ShowString(140, 160, 100, 16, 16, "Light CLOSE  ");
      }
   }
   if(CO2<1000){
      flag=0;
      fanflag=fanoff;
      LCD_ShowString(120, 80, 120, 16, 16, "               ");
      LCD_ShowString(140, 180, 100, 16, 16, "FAN CLOSE    ");
   }
   if(CO2>=1000){
      LCD_ShowString(120, 80, 120, 16, 16, "CO2 over       ");
      BEEP=1;
      delay_ms(100);
      BEEP=0;
      flag=co2flag;
      fanflag=fanon;
      LCD_ShowString(140, 180, 100, 16, 16, "FAN OPEN     ");
   }
}

