#ifdef __cplusplus
extern "C" {
#endif 

#include "i2c.h"
#include "nvic.h"
#include "delay.h"

static void IIC_SCL_H(uint8_t PB_I2Cx){
    
    if (PB_I2Cx==1){
        IIC1_SCL=1;
    }
    else if(PB_I2Cx==2){
    }
    else if(PB_I2Cx==3){
        IIC3_SCL=1;
    }
}

static void IIC_SCL_L(uint8_t PB_I2Cx){
    
    if (PB_I2Cx==1){
        IIC1_SCL=0;
    }
    else if(PB_I2Cx==2){
    }
    else if(PB_I2Cx==3){
        IIC3_SCL=0;
    }
}

static void IIC_SDA_H(uint8_t PB_I2Cx){
    
    if (PB_I2Cx==1){
        IIC1_SDA=1;
    }
    else if(PB_I2Cx==2){
    }
    else if(PB_I2Cx==3){
        IIC3_SDA=1;
    }
}

static void IIC_SDA_L(uint8_t PB_I2Cx){
    
    if (PB_I2Cx==1){
        IIC1_SDA=0;
    }
    else if(PB_I2Cx==2){
    }
    else if(PB_I2Cx==3){
        IIC3_SDA=0;
    }
}

static void SDA_OUT(uint8_t PB_I2Cx){
    
    if (PB_I2Cx==1){
        SDA1_OUT();
    }
    else if(PB_I2Cx==2){
    }
    else if(PB_I2Cx==3){
        SDA3_OUT();
    }
}

static void SDA_IN(uint8_t PB_I2Cx){
    
    if (PB_I2Cx==1){
        SDA1_IN();
    }
    else if(PB_I2Cx==2){
    }
    else if(PB_I2Cx==3){
        SDA3_IN();
    }
}

static uint8_t IIC_READ_SDA(uint8_t PB_I2Cx){
    
    if (PB_I2Cx==1){
        return IIC1_READ_SDA;
    }
    else if(PB_I2Cx==2){
    }
    else if(PB_I2Cx==3){
        return IIC3_READ_SDA;
    }

    return 0;
}


//初始化IIC
void IIC_Init(uint8_t PB_I2Cx)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

    if (PB_I2Cx==1){
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
        GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
    }
    else if(PB_I2Cx==2){
    }
    else if(PB_I2Cx==3){
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOB时钟
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOB时钟
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化

    }

    IIC_SCL_H(PB_I2Cx);
    IIC_SDA_H(PB_I2Cx);
}
//产生IIC起始信号
void IIC_Start(uint8_t PB_I2Cx)
{
    SDA_OUT(PB_I2Cx);
    IIC_SDA_H(PB_I2Cx);
    IIC_SCL_H(PB_I2Cx);
    delay_us(4);
    IIC_SDA_L(PB_I2Cx);
    delay_us(4);
    IIC_SCL_L(PB_I2Cx);
}

//产生IIC停止信号
void IIC_Stop(uint8_t PB_I2Cx)
{
    SDA_OUT(PB_I2Cx);//sda线输出
    IIC_SCL_L(PB_I2Cx);
    IIC_SDA_L(PB_I2Cx);
    delay_us(4);
    IIC_SCL_H(PB_I2Cx);
    IIC_SDA_H(PB_I2Cx);
    delay_us(4);			   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(uint8_t PB_I2Cx)
{
	u8 ucErrTime=0;
	SDA_IN(PB_I2Cx);
	IIC_SDA_H(PB_I2Cx);
    delay_us(1);	   
	IIC_SCL_H(PB_I2Cx);
    delay_us(1);	 
	while(IIC_READ_SDA(PB_I2Cx))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop(PB_I2Cx);
			return 1;
		}
	}
	IIC_SCL_L(PB_I2Cx);
	return 0;  
} 
//产生ACK应答
void IIC_Ack(uint8_t PB_I2Cx)
{
	IIC_SCL_L(PB_I2Cx);
	SDA_OUT(PB_I2Cx);
	IIC_SDA_L(PB_I2Cx);
	delay_us(2);
	IIC_SCL_H(PB_I2Cx);
	delay_us(2);
	IIC_SCL_L(PB_I2Cx);
}
//不产生ACK应答		    
void IIC_NAck(uint8_t PB_I2Cx)
{
	IIC_SCL_L(PB_I2Cx);
	SDA_OUT(PB_I2Cx);
	IIC_SDA_H(PB_I2Cx);
	delay_us(2);
	IIC_SCL_H(PB_I2Cx);
	delay_us(2);
	IIC_SCL_L(PB_I2Cx);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t PB_I2Cx, u8 txd)
{                        
    u8 t;   
	SDA_OUT(PB_I2Cx); 	    
    IIC_SCL_L(PB_I2Cx);
    for(t=0;t<8;t++)
    {          
        if ((txd&0x80)>>7){
            IIC_SDA_H(PB_I2Cx);
        }
        else{
            IIC_SDA_L(PB_I2Cx);
        }

        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL_H(PB_I2Cx);
		delay_us(2); 
		IIC_SCL_L(PB_I2Cx);
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(uint8_t PB_I2Cx, unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN(PB_I2Cx);//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_L(PB_I2Cx);
        delay_us(4);
		IIC_SCL_H(PB_I2Cx);
        receive<<=1;
        if(IIC_READ_SDA(PB_I2Cx))
            receive++;   
		delay_us(4); 
    }					 
    if (!ack)
        IIC_NAck(PB_I2Cx);//发送nACK
    else
        IIC_Ack(PB_I2Cx); //发送ACK   
    return receive;
}

int PB_I2C_Write_Byte(uint8_t PB_I2Cx, uint8_t addr, uint8_t reg, uint8_t data)
{
    IIC_Start(PB_I2Cx);  
	// if(EE_TYPE>AT24C16)
	// {
	// 	IIC_Send_Byte(0XA0);	    //发送写命令
	// 	IIC_Wait_Ack();
	// 	IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	// }else 
    IIC_Send_Byte(PB_I2Cx, addr);   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack(PB_I2Cx);	   
    IIC_Send_Byte(PB_I2Cx, reg);   //发送低地址
	IIC_Wait_Ack(PB_I2Cx); 	 										  		   
	IIC_Send_Byte(PB_I2Cx, data);     //发送字节							   
	IIC_Wait_Ack(PB_I2Cx);  		    	   
    IIC_Stop(PB_I2Cx);//产生一个停止条件 
	delay_ms(1);

    return 1;
}

int PB_I2C_Read_Byte(uint8_t PB_I2Cx, uint8_t addr, uint8_t reg, uint8_t* data)
{
    IIC_Start(PB_I2Cx);  
	// if(EE_TYPE>AT24C16)
	// {
	// 	IIC_Send_Byte(0XA0);	   //发送写命令
	// 	IIC_Wait_Ack();
	// 	IIC_Send_Byte(reg>>8);//发送高地址	    
	// }else 
    IIC_Send_Byte(PB_I2Cx, addr);   //发送器件地址0XA0,写数据 	   
	IIC_Wait_Ack(PB_I2Cx); 
    IIC_Send_Byte(PB_I2Cx, reg);   //发送低地址
	IIC_Wait_Ack(PB_I2Cx);	    
	IIC_Start(PB_I2Cx);  	 	   
	IIC_Send_Byte(PB_I2Cx, addr+1);           //进入接收模式			   
	IIC_Wait_Ack(PB_I2Cx);	 
    *data=IIC_Read_Byte(PB_I2Cx, 0);		   
    IIC_Stop(PB_I2Cx);//产生一个停止条件	    

    return 1;
}	

int PB_I2C_Write_Buf(uint8_t PB_I2Cx, uint8_t addr, uint8_t reg, uint8_t* data, uint8_t size)
{
    IIC_Start(PB_I2Cx);  
	// if(EE_TYPE>AT24C16)
	// {
	// 	IIC_Send_Byte(0XA0);	    //发送写命令
	// 	IIC_Wait_Ack();
	// 	IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	// }else 
    IIC_Send_Byte(PB_I2Cx, addr);   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack(PB_I2Cx);	   
    IIC_Send_Byte(PB_I2Cx, reg);   //发送低地址
	IIC_Wait_Ack(PB_I2Cx);
    int i=0;
    for(i=0;i<size;i++){
        IIC_Send_Byte(PB_I2Cx, data[i]);     //发送字节
        IIC_Wait_Ack(PB_I2Cx);
    }

    IIC_Stop(PB_I2Cx);//产生一个停止条件 
	delay_ms(10);

    return 1;
}	

int PB_I2C_Read_Buf(uint8_t PB_I2Cx, uint8_t addr,uint8_t reg, uint8_t* data, uint8_t size)
{
    IIC_Start(PB_I2Cx);  

    IIC_Send_Byte(PB_I2Cx, addr);   //发送器件地址0XA0,写数据 	   
	IIC_Wait_Ack(PB_I2Cx); 
    IIC_Send_Byte(PB_I2Cx, reg);   //发送低地址
	IIC_Wait_Ack(PB_I2Cx);	    
	IIC_Start(PB_I2Cx);  	 	   
	IIC_Send_Byte(PB_I2Cx, addr+1);           //进入接收模式			   
	IIC_Wait_Ack(PB_I2Cx);	 

    while(size){
        if (size==1)
            *data=IIC_Read_Byte(PB_I2Cx, 0);
        else
            *data=IIC_Read_Byte(PB_I2Cx, 1);

        data++;
        size--;
    }
    IIC_Stop(PB_I2Cx);//产生一个停止条件	    

    return 1;
}	

void PB_I2C_Init(uint8_t PB_I2Cx)
{
     IIC_Init(PB_I2Cx);
}

#ifdef __cplusplus
}
#endif 

