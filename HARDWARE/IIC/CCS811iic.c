#include "CCS811iic.h"
#include "delay.h"

//初始化IIC
void IIC3_Init(void)
{					     
	tls_gpio_cfg(WM_IO_PB_17, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
   tls_gpio_cfg(WM_IO_PB_18, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
}
//产生IIC起始信号
u8 IIC3_Start(void)
{
	SDA3_OUT();     //sda线输出
	IIC3_SDA=1;	  	  
	IIC3_SCL=1;
//	if(!SDA_read) return 0;
	delay_us(50);
 	IIC3_SDA=0;//START:when CLK is high,DATA change form high to low 
//	if(!SDA_read) return 0;
	delay_us(50);
	IIC3_SCL=0;//钳住IIC总线，准备发送或接收数据 
	return 1;
}	  
//产生IIC停止信号
void IIC3_Stop(void)
{
	SDA3_OUT();//sda线输出
	IIC3_SCL=0;
	IIC3_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(50);
	IIC3_SCL=1; 
	IIC3_SDA=1;//发送IIC总线结束信号
	delay_us(50);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC3_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA3_IN();      //SDA设置为输入  
	IIC3_SDA=1;delay_us(1);	   
	IIC3_SCL=1;delay_us(1);	 
	while(READ3_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC3_Stop();
			return 1;
		}
	}
	IIC3_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC3_Ack(void)
{
	IIC3_SCL=0;
	SDA3_OUT();
	IIC3_SDA=0;
	delay_us(50);
	IIC3_SCL=1;
	delay_us(50);
	IIC3_SCL=0;
}
//不产生ACK应答		    
void IIC3_NAck(void)
{
	IIC3_SCL=0;
	SDA3_OUT();
	IIC3_SDA=1;
	delay_us(50);
	IIC3_SCL=1;
	delay_us(50);
	IIC3_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC3_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA3_OUT(); 	    
    IIC3_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			IIC3_SDA=1;
		else
			IIC3_SDA=0;
		txd<<=1; 	  
		delay_us(50);   //对TEA5767这三个延时都是必须的
		IIC3_SCL=1;
		delay_us(50); 
		IIC3_SCL=0;	
		delay_us(50); 
		
    }	 

} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC3_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA3_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
		{
        IIC3_SCL=0; 
        delay_us(50);
				IIC3_SCL=1;
        receive<<=1;
        if(READ3_SDA)receive++;   
				delay_us(50); 
    }					 
    if (!ack)
        IIC3_NAck();//发送nACK
    else
        IIC3_Ack(); //发送ACK   
    return receive;
}
u8 Single_WriteIIC3_byte(u8 Slave_Address,u8 REG_Address,u8 data)
{
	if(IIC3_Start()==0)  //起始信号
		{IIC3_Start(); return RESET;}           

    IIC3_Send_Byte(Slave_Address);   //发送设备地址+写信号
 	  if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;}
   
		IIC3_Send_Byte(REG_Address);    //内部寄存器地址，
 	  if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;}
   
		IIC3_Send_Byte(data);       //内部寄存器数据，
	  if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;}
		
		IIC3_Stop();   //发送停止信号
		
		return SET;
}

u8 Single_MWriteIIC3_byte(u8 Slave_Address,u8 REG_Address,u8 const *data,u8 length)
{
	  IIC3_Start();         

	IIC3_Send_Byte(Slave_Address);   //发送设备地址+写信号
 	  if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;}
   
		IIC3_Send_Byte(REG_Address);    //内部寄存器地址，
 	  if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;}
	 
	while(length)
	{
		IIC3_Send_Byte(*data++);       //内部寄存器数据，
	   if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;}           //应答
		length--;
	}
		IIC3_Stop();   //发送停止信号		
		return SET;
}

u8 Single_ReadIIC3(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length)
{
	
	if(IIC3_Start()==0)  //起始信号
		{IIC3_Stop(); return RESET;}          
	 
	IIC3_Send_Byte(Slave_Address);    //发送设备地址+写信号
 	if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;} 
	
	IIC3_Send_Byte(REG_Address);     //发送存储单元地址
 	if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;} 
	
	if(IIC3_Start()==0)  //起始信号
			{IIC3_Stop(); return RESET;}            

	IIC3_Send_Byte(Slave_Address+1);  //发送设备地址+读信号
 	if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;}
	
	while(length-1)
	{
		*REG_data++=IIC3_Read_Byte(1);       //读出寄存器数据
		length--;
	}
	*REG_data=IIC3_Read_Byte(0);  

	IIC3_Stop();                    //停止信号
	return SET;
}























