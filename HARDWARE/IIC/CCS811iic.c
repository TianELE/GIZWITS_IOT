#include "CCS811iic.h"
#include "delay.h"

//��ʼ��IIC
void IIC3_Init(void)
{					     
	tls_gpio_cfg(WM_IO_PB_17, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
   tls_gpio_cfg(WM_IO_PB_18, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
}
//����IIC��ʼ�ź�
u8 IIC3_Start(void)
{
	SDA3_OUT();     //sda�����
	IIC3_SDA=1;	  	  
	IIC3_SCL=1;
//	if(!SDA_read) return 0;
	delay_us(50);
 	IIC3_SDA=0;//START:when CLK is high,DATA change form high to low 
//	if(!SDA_read) return 0;
	delay_us(50);
	IIC3_SCL=0;//ǯסIIC���ߣ�׼�����ͻ�������� 
	return 1;
}	  
//����IICֹͣ�ź�
void IIC3_Stop(void)
{
	SDA3_OUT();//sda�����
	IIC3_SCL=0;
	IIC3_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(50);
	IIC3_SCL=1; 
	IIC3_SDA=1;//����IIC���߽����ź�
	delay_us(50);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC3_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA3_IN();      //SDA����Ϊ����  
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
	IIC3_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC3_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA3_OUT(); 	    
    IIC3_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			IIC3_SDA=1;
		else
			IIC3_SDA=0;
		txd<<=1; 	  
		delay_us(50);   //��TEA5767��������ʱ���Ǳ����
		IIC3_SCL=1;
		delay_us(50); 
		IIC3_SCL=0;	
		delay_us(50); 
		
    }	 

} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC3_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA3_IN();//SDA����Ϊ����
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
        IIC3_NAck();//����nACK
    else
        IIC3_Ack(); //����ACK   
    return receive;
}
u8 Single_WriteIIC3_byte(u8 Slave_Address,u8 REG_Address,u8 data)
{
	if(IIC3_Start()==0)  //��ʼ�ź�
		{IIC3_Start(); return RESET;}           

    IIC3_Send_Byte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;}
   
		IIC3_Send_Byte(REG_Address);    //�ڲ��Ĵ�����ַ��
 	  if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;}
   
		IIC3_Send_Byte(data);       //�ڲ��Ĵ������ݣ�
	  if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;}
		
		IIC3_Stop();   //����ֹͣ�ź�
		
		return SET;
}

u8 Single_MWriteIIC3_byte(u8 Slave_Address,u8 REG_Address,u8 const *data,u8 length)
{
	  IIC3_Start();         

	IIC3_Send_Byte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;}
   
		IIC3_Send_Byte(REG_Address);    //�ڲ��Ĵ�����ַ��
 	  if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;}
	 
	while(length)
	{
		IIC3_Send_Byte(*data++);       //�ڲ��Ĵ������ݣ�
	   if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;}           //Ӧ��
		length--;
	}
		IIC3_Stop();   //����ֹͣ�ź�		
		return SET;
}

u8 Single_ReadIIC3(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length)
{
	
	if(IIC3_Start()==0)  //��ʼ�ź�
		{IIC3_Stop(); return RESET;}          
	 
	IIC3_Send_Byte(Slave_Address);    //�����豸��ַ+д�ź�
 	if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;} 
	
	IIC3_Send_Byte(REG_Address);     //���ʹ洢��Ԫ��ַ
 	if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;} 
	
	if(IIC3_Start()==0)  //��ʼ�ź�
			{IIC3_Stop(); return RESET;}            

	IIC3_Send_Byte(Slave_Address+1);  //�����豸��ַ+���ź�
 	if(IIC3_Wait_Ack()){IIC3_Stop(); return RESET;}
	
	while(length-1)
	{
		*REG_data++=IIC3_Read_Byte(1);       //�����Ĵ�������
		length--;
	}
	*REG_data=IIC3_Read_Byte(0);  

	IIC3_Stop();                    //ֹͣ�ź�
	return SET;
}























