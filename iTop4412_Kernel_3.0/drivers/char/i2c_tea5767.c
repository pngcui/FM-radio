/*
 *使用GPIO口模拟I2C
 *author @pngcui 20160414
 *
 *EXYNOS4212_GPJ1(2)----SDA----pin14-CAM_HREF
 *EXYNOS4212_GPJ0(2)----SCL----pin17-CAM_PCLK
 */
 
 
#include "i2c_tea5767.h"

#include <linux/delay.h>
#include <linux/init.h>
#include <linux/module.h>

/*Linux中申请GPIO的头文件*/
#include <linux/gpio.h>
/*三星平台的GPIO配置函数头文件*/
/*三星平台EXYNOS系列平台，GPIO配置参数宏定义头文件*/
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
/*三星平台4412平台，GPIO宏定义头文件*/
#include <mach/gpio-exynos4.h>

//产生i2c开始信号
void IIC_start(){
	
	//sda线输出
	s3c_gpio_cfgpin(SDA, S3C_GPIO_OUTPUT);
	s3c_gpio_cfgpin(SCL, S3C_GPIO_OUTPUT);
	
	gpio_set_value(SCL, 1);
	gpio_set_value(SDA, 1);
	
	mdelay(2);
	
 	//START:when CLK is high,DATA change form high to low 
	gpio_set_value(SDA, 0);
	
	mdelay(2);
	
	//钳住i2c总线，准备发送或接收
	gpio_set_value(SCL, 0);
	
	return ;
}

//产生i2c停止信号
void IIC_stop(){
	
	//STOP:when CLK is high DATA change form low to high
	s3c_gpio_cfgpin(SDA, S3C_GPIO_OUTPUT);
	s3c_gpio_cfgpin(SCL, S3C_GPIO_OUTPUT);
	
	gpio_set_value(SCL, 0);
	gpio_set_value(SDA, 0);
	
 	mdelay(2);
	
	//结束信号
	gpio_set_value(SDA, 1);
	gpio_set_value(SCL, 1);
	
	mdelay(2);

	return ;
}

//产生ack应答
void IIC_ack(){

	s3c_gpio_cfgpin(SDA, S3C_GPIO_OUTPUT);
	s3c_gpio_cfgpin(SCL, S3C_GPIO_OUTPUT);
	
	gpio_set_value(SCL,0);
	
	gpio_set_value(SDA,0);
	
	mdelay(2);
	
	gpio_set_value(SCL, 1);
	
	mdelay(2);
	
	gpio_set_value(SCL, 0);
	
	return ;
}

//不产生应答
void IIC_nack(){

	s3c_gpio_cfgpin(SDA, S3C_GPIO_OUTPUT);
	s3c_gpio_cfgpin(SCL, S3C_GPIO_OUTPUT);
	
	gpio_set_value(SCL, 0);
	
	gpio_set_value(SDA,1);
	
	mdelay(2);
	
	gpio_set_value(SCL, 1);
	
	mdelay(2);
	
	gpio_set_value(SCL, 0);
	
	return ;
}

//应答信号
//return 1 有应答
//return 0 无应答
unsigned char IIC_waitack(){
	
	unsigned int ucErrTime=0;
	
	//SDA设置为输入
	s3c_gpio_cfgpin(SDA, S3C_GPIO_INPUT);
	s3c_gpio_cfgpin(SCL, S3C_GPIO_OUTPUT);
	
	mdelay(2);	   
	
	gpio_set_value(SCL, 1);
	
	mdelay(2);	 
	
	while( gpio_get_value(SDA)==1 ){
		
		ucErrTime++;
		if(ucErrTime>500){
			
			IIC_stop();
			printk("wait timeout\n");
			return 1;
		}
	}
	
	//时钟输出为0   
	gpio_set_value(SCL,0);
	return 0;  
}

//模拟i2c读一个字节
//ack=1时，发送ACK，ack=0，发送nACK   
unsigned char IIC_read8bit(unsigned char ack){
	
	unsigned char i,receive=0;
	
	//SDA设置为输入
	s3c_gpio_cfgpin(SDA, S3C_GPIO_INPUT);
	s3c_gpio_cfgpin(SCL, S3C_GPIO_OUTPUT);
	
    for(i=0;i<8;i++){
		
		mdelay(1);
		
		gpio_set_value(SCL,0);
		
		mdelay(1);
		
		gpio_set_value(SCL,1);
		
		receive <<= 1;
		
		udelay(10);
		
		if( gpio_get_value(SDA)==1 ){
			receive++;
		}
    }
	
	mdelay(2); 
	
    return (receive);
}

//模拟i2c写一个字节
void IIC_write8bit(unsigned char addr){
	int i;
	
	s3c_gpio_cfgpin(SDA, S3C_GPIO_OUTPUT);
	s3c_gpio_cfgpin(SCL, S3C_GPIO_OUTPUT);
	
	//拉低时钟开始数据传输
	gpio_set_value(SCL, 0);
	
	mdelay(1);
	
    for(i=0;i<8;i++){
		
		if((addr & 0x80)>>7)
			gpio_set_value(SDA, 1);
		else
			gpio_set_value(SDA, 0);
		
		addr <<= 1;
		
		mdelay(1);
		
		gpio_set_value(SCL,1);
		
		mdelay(1);
		
		gpio_set_value(SCL, 0);
		
		mdelay(1);
    }
	return ;
}
