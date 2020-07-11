

#ifndef CCS811_h
#define CCS811_h
//////////////////////////////////////////////////////////////////////////////////	  
 //SCL B31  SDA B30   WAKE B24 INT B25

#include "delay.h"
#include "CCS811iic.h"

#define boolean 				u8
#define byte 					u8
#define uint8_t 				u8
	typedef struct {
	u16 eco2;
	u16 tvoc;
	u8 status;
	u8 error_id;
	u8 hw_id;
	u16 raw_data;
	} ccs811_measurement_t;

#define nWAKE           PBout(24)      // pin B5 is tied to CCS811 nWAKE pin in sensor node
#define nINT            PBout(25)      // pin E5 is tied to CCS811 nINT pin in sensor node
//读地址为AB,写地址AA，I2C_ADDR接地

#define CCSADDR				0x5A<<1 // when I2C_ADDR pin is LOW
 //#define CCSADDR					0x5B<<1 when I2C_ADDR pin is HIGH
#define READ            	0xB5  
#define WRIT            	0xB4   
// Registers for CCS811
#define STATUS          	0x00
#define MEAS_MODE       	0x01
#define ALG_RESULT_DATA 	0x02
#define RAW_DATA			0x03
#define ENV_DATA        	0x05
#define NTC        			0x06
#define THRESHOLDS      	0x10
#define BASELINE      		0x11
#define HW_ID           	0x20
#define HW_Version 			0x21
#define FW_Boot_Version		0x23
#define FW_App_Version  	0x24
#define ERROR_ID        	0xE0
#define SW_RESET        	0xFF
//测量模式
#define MODE0 				0x00    //不侧量
#define MODE1 				0x10	//每秒持续测量
#define MODE2 				0x20	//每10s测量
#define MODE3 				0x30	//每分钟测量
#define MODE4 				0x40	//强力测量，250ms一次

    void CCS811Init(void);     // constructor
	u8 CCS811_ReadOneByte(u8 ReadAddr);
	void CCS811_WriteOneByte(u8 RegAddr,u8 DataToWrite);
    void readStatus(void);
    void readHW_ID(void);
	void readErrorID(void);
	void readALG_RESULT_DATA(u8 *data);
	void app_Start(void);	
		
    int readTVOC(void);
    int readCO2(void);
    void getData(void);
    void compensate(float t, float rh);
    void setMode(u8 mode);
	
    void reset(void);
    void sleep(void);
#endif
