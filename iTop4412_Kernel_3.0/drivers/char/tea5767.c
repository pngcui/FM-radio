/*
 *使用GPIO口模拟I2C
 *author @pngcui 20160414
 *
 *EXYNOS4212_GPJ1(2)----SDA----pin14-CAM_HREF
 *EXYNOS4212_GPJ0(2)----SCL----pin17-CAM_PCLK
 */
 
 
#include <linux/init.h>
#include <linux/module.h>
//#include <linux/types.h>

/*驱动注册的头文件，platform结构体和驱动注册与注销*/
#include <linux/platform_device.h>

/*杂项设备头文件*/
#include <linux/miscdevice.h>

/*设备节点头文件*/
#include <linux/fs.h>


/*Linux中申请GPIO的头文件*/
#include <linux/gpio.h>
/*三星平台的GPIO配置函数头文件*/
/*三星平台EXYNOS系列平台，GPIO配置参数宏定义头文件*/
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
/*三星平台4412平台，GPIO宏定义头文件*/
#include <mach/gpio-exynos4.h>

#include <linux/delay.h>
#include "i2c_tea5767.h"

//设备节点
#define DEVICE_NAME "tea5767"
//匹配项
#define DRIVER_NAME "tea5767"

static int tea5767_gpios[] = {
    EXYNOS4212_GPJ1(2),EXYNOS4212_GPJ0(2),
};

#define GPIO_NUM        ARRAY_SIZE(tea5767_gpios)

#define Search 3
#define AutoSearch 4

#define SETFREQ   5

#define OPENMUTE  8
#define CLOSEMUTE 9
#define SHUTDOWN 10

#define ADC 100
#define FREQ 101

#define max_freq 108000
#define min_freq 87500
unsigned int default_pll=0x299d; //默认存台的pll,87.5MHz

unsigned char write_data[5]={0x29,0x9d,0xa0,0x17,0x00};       //要写入TEA5767的数据--0x2991->87400
//不静音，不自动搜索，Search stop=10(mid; level ADC output = 7),时钟频率=01(32.768 kHz)
unsigned char read_data[5];        //TEA5767读出的状态

int Frequency_Write=87500;
int Frequency_Read=0;
int pll=0;

/*
*读入数据5byte
-----------8-----------------4------------------2-----------------1---------------|----8----------------4---------------2----------------------1----------------
*1st byte：MUTE(静音=1)      SM(自动搜索=1)     PLL13             PLL12          -|-   PLL11            PLL10            PLL9                  PLL8 
*2nd byte：PLL7              PLL6               PLL5              PLL4           -|—   PLL3             PLL2             PLL1                  PLL0
*3rd byte：SUD(SearchUp=1)   SSL1(Search stop)  SSL0(Search stop) HLSI(LO)       -|-   MS(单声道=1)     MR(右声道静音=1) ML(左声道静音=1)      SWP1(端口1为高=1)
*4th byte：SWP2(端口2为高=1) STBY(待机=1)       BL(US/Europe=0)   XTAL(时钟频率) -|-   SMUTE(soft mute) HCC(High Cut)    SNC(立体音噪音消除1)  SI(SWPORT1做ready flag 为1)
*5th byte：PLLREF(时钟频率)  DTC(de-emphasis time) -              -              -|-   -                -                -                -
*/

/*
*读出数据5byte
-----------8-----------------4------------------2-----------------1---------------|----8----------------4---------------2-----------------1----------------
*1st byte：RF(发现电台=1)    BLF(搜索到头=1)    PLL13             PLL12          -|-   PLL11            PLL10            PLL9             PLL8 
*2nd byte：PLL7              PLL6               PLL5              PLL4           -|—   PLL3             PLL2             PLL1             PLL0
*3rd byte：STEREO(立体声=1)  IF6(中频计数结果)  IF5(同前)         IF4(同前)      -|-   IF3(同前)        IF2(同前)        IF1(同前)        IF0(同前)
*4th byte：LEV3(信号ADC)     LEV2(同前)         LEV1(同前)        LEV0(同前)     -|-   CI3(芯片标记) 	CI2(同前)        CI1(同前)  	  0
*5th byte：0  				 0				    0              	  0              -|-   0                0                0                0
*/
/*
void print(unsigned char data[]){
	int i = 0 ;
	for(i = 0 ; i < 4 ; i ++ ){
		printk("0x%x,",data[i]);
	}
	printk("0x%x\n",data[i]);
}
*/

//由频率计算PLL
void get_pll(){
	
    unsigned char hlsi;
    //unsigned int twpll=0;
    hlsi=write_data[2]&0x10;
    if (hlsi)
        pll=(((Frequency_Write*1000)+225000)*4)/32768;
    else
        pll=(((Frequency_Write*1000)-225000)*4)/32768;
	
	printk("frequency=%d ---> pll=%x\n",Frequency_Write,pll);
	
	return ;
}

//由PLL计算频率
void get_frequency(){
	
    unsigned char hlsi;
	
    hlsi=write_data[2]&0x10;
     if (hlsi)
        Frequency_Read=(int)(pll*8193 - 250000)/1000;    //频率单位:KHz
    else
        Frequency_Read=(int)(pll*8193 + 250000)/1000;    //频率单位:KHz
	
	//优化II，消除小数点后两位对频率的影响
	Frequency_Read = (Frequency_Read/100) * 100 ;
	
	return ;
}

//获取ADC 信号强度
int getADC(){
	int adc;
	
	adc = read_data[3]>>4;
	
	return adc;
}


//读TEA5767状态,并转换成频率
void tea5767_read(){
	
	//printk("tea5767 READ:");
    unsigned char i;
    pll=0;
	
    IIC_start();
	udelay(2);
	//address(READ)
	//printk("写读的地址！\n");
	IIC_write8bit(0xC1);        //进入READ模式
	
	//A
    if(!IIC_waitack()){
		//printk("read and wait YES!!!\n");
		
		//DATA
        for(i=0;i<4;i++){
			
            read_data[i]=IIC_read8bit(0);
            IIC_ack();
        }
		//the last byte with ACK.
		read_data[4]=IIC_read8bit(1);
		IIC_ack();
    }
	else{
		printk("wait  timeout...\n");
	}
	
    IIC_stop();
	
	//还原pll
	pll = (read_data[0]&0x3f)*256 + read_data[1] + 1;
	
	return ;
}

//写频率等操作
void tea5767_write(){
	
	//printk("tea5767 WRITE:");
    unsigned char i;
	
    IIC_start();
	
	//printk("写写的地址！\n");
	udelay(2);
	
    IIC_write8bit(0xC0);        //TEA5767写地址
	
    if(!IIC_waitack()){
		//printk("write and wait YES\n");
        for(i=0;i<5;i++){
			
            IIC_write8bit(write_data[i]);
            //IIC_waitack();
			if(IIC_waitack()){
				printk("wait timeout...write error..\n");
				break;
			}
        }
    }
	else{
		printk("write addr err!!!!!!!!!!!!\n");
	}
    IIC_stop(); 
	
	return ;
}


//打开静音
void openMute(){
	
	write_data[0] |= 0x80;
	write_data[3] |= 0x40;
	
	tea5767_write();
	//print(write_data);
	
	tea5767_read();
	//print(read_data);
	
	return ;
}

//打开收音机音量
void closeMute(){
	
	write_data[0] &= 0x7f;
	write_data[3] &= 0xbf;
	
	tea5767_write();
	//print(write_data);
	
	tea5767_read();
	//print(read_data);
	
	return ;
}


//手动搜台，mode=1,正向。mode=0，反向。
static void search(int mode){
	
	int i = 0;
	
	if(mode){
        Frequency_Write += 100;
		
        if(Frequency_Write>max_freq)
            Frequency_Write=min_freq;
    }
    else{
        Frequency_Write -= 100;
		
        if(Frequency_Write<min_freq)
            Frequency_Write=max_freq;
    }
	
    get_pll();
    write_data[0]=pll/256;
    write_data[1]=pll%256;
    //write_data[2]=0xc0;
    write_data[3] &=0xbf;
    //write_data[4]=0x00;
	
    tea5767_write();
	//print(write_data);
	
	tea5767_read();
	//print(read_data);
	//get_frequency();
	
	//analyse();
	
	mdelay(5);
	
	//printk("\n---------------------------------------------------------------------\n");
	
	return ;
	
}

/*自动搜索
*从当前频率开始搜索
*调用search(1)进行循环搜索
*每调用一次都返回数据，若搜到可用电台return 1 否则return 0
*/
//need return float[]
int auto_search(int freq,int mode){
	
	//从传递过来的频率开始搜台
	Frequency_Write = freq;

	search(mode);
	
	//&的优先级小于<符号
	if((read_data[3]>>4) > 5){
		if(read_data[0]>>7 && read_data[2]>>7 ){
			if(((read_data[2]&0x7f) > 0x30) && ((read_data[2]&0x7f) < 0x3f)){
			return 1;
			}
		}
	}
	return 0;
}

//待机，静音模式
//app退出回调函数
void setShutDown(){
	
	write_data[0] |= 0x80;
	write_data[3] |= 0x40;
	
	tea5767_write();
	tea5767_read();
	
	return ;
}

//设置频率,并返回芯片的当前频率
int setFreq(int freq){
	
	Frequency_Write = freq;
	get_pll();
	
	write_data[0]=pll/256;
    write_data[1]=pll%256;
	write_data[2]=0xc0;
	write_data[3]=0x17;
	write_data[4]=0x00;
	
	tea5767_write();
	//print(write_data);
	
	tea5767_read();
	//print(read_data);
	
	get_frequency();
	
	return Frequency_Read;
}


static int tea5767_ioctl( struct file *files, int cmd, int arg){
    int ret;
    printk("Hello tea5767   and  cmd is %d,arg is %d\n",cmd,arg);
    
    switch(cmd){
		
		//打开静音
		case OPENMUTE:
			openMute();
			break;
		
		//关闭静音
		case CLOSEMUTE:
			closeMute();
			break;
			
		//手动搜索
        case Search:
			search(arg);
			break;
			
		//自动搜索
        case AutoSearch:
            return auto_search(arg,1);
			//break;
		//获取信号强度
		case ADC:
			return getADC();
		
		//获取当前频道
		case FREQ:
			get_frequency();
			return Frequency_Read;
			
		//设置频道
		case SETFREQ:
			return setFreq(arg);
			//break;
			
		//待机，静音模式
		case SHUTDOWN:
			setShutDown();
			break;

        default:
			printk("default--cmd=%d,arg=%d\n",cmd,arg);
			//test(cmd,arg);
			//break;
            return -1;
    }
    
    return 0;
}

static int tea5767_release(struct inode *inode, struct file *file){
    
    printk("tea5767 release\n");
    
    return 0;
}


static int tea5767_open(struct inode *inode, struct file *file){
    
    printk("tea5767 open\n");
    
    
    return nonseekable_open(inode,file);
}

static struct file_operations tea5767_ops = {
    .owner = THIS_MODULE,
    .open = tea5767_open,
    .release = tea5767_release,
    .unlocked_ioctl = tea5767_ioctl,
};


static  struct miscdevice tea5767_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &tea5767_ops,
};


static int tea5767_probe(struct platform_device *pdv){
    int ret,i;
    printk("tea5767_probe start..\n");
	
	pll = default_pll;
	
	
    for(i=0; i<GPIO_NUM; i++){
		
        ret = gpio_request(tea5767_gpios[i], "tea5767");
        if (ret < 0) {
            printk("%s: request GPIO %d for tea5767 failed, ret = %d\n", DEVICE_NAME,i, ret);
			goto exit;
		}
        else{
            printk("%s: request GPIO %d for tea5767 success, ret = %d\n", DEVICE_NAME,i, ret);
            s3c_gpio_cfgpin(tea5767_gpios[i], S3C_GPIO_OUTPUT);
            gpio_set_value(tea5767_gpios[i], 0);
            //gpio_free(tea5767_gpios[i]);
        }
		
    }
	
	//使能芯片
	//mute=1&stby=1
	write_data[0]=0xac;
	write_data[1]=0x7a;
	write_data[2]=0xd0;
	write_data[3]=0x57;
	write_data[4]=0x00;
	
	tea5767_write();
	tea5767_read();
	
	//初始化芯片
	//设置为mute=1&freq=87400&stby=0
	write_data[0]=0xa9;
	write_data[1]=0x9d;
	write_data[2]=0xa0;
	write_data[3]=0x17;
	write_data[4]=0x00;
	tea5767_write();
	tea5767_read();
	
    ret = misc_register(&tea5767_dev);
    if(ret<0)
    {
        printk("tea5767:register device failed!\n");
        goto exit;
    }

    return 0;

exit:
    misc_deregister(&tea5767_dev);
    return ret;
}

static int tea5767_remove(struct platform_device *pdv){
    //int i;
    printk("tea5767_remove...\n");

    misc_deregister(&tea5767_dev);
    
    
    return 0;
}

static void tea5767_shutdown(struct platform_device *pdv){
    
    return ;
}

static int tea5767_suspend(struct platform_device *pdv,pm_message_t pmt){
    
    return 0;
}

static int tea5767_resume(struct platform_device *pdv){
    
    return 0;
}

struct platform_driver tea5767_driver = {
    .probe = tea5767_probe,
    .remove = tea5767_remove,
    .shutdown = tea5767_shutdown,
    .suspend  = tea5767_suspend,
    .resume = tea5767_resume,
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
    }
};

/* init the driver */
static int __init tea5767_init(){
	
	int err;
    
    printk("tea5767   init   start...\n");
    
    err = platform_driver_register(&tea5767_driver);
    
    printk("state is %d\n\n",err);
    
    return 0;
}

/* cleanup the driver */
static void __exit tea5767_exit(){
	
	int i;
	
	printk("tea5767  exit ..and free gpio\n");
	gpio_free(SCL);
	gpio_free(SDA);
	
    platform_driver_unregister(&tea5767_driver);
}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("PNGCUI");

module_init(tea5767_init);
module_exit(tea5767_exit);
