#define LOG_TAG "FM5767_Stub"  
  
#include <hardware/hardware.h>  
#include <hardware/hw_tea5767.h>  
#include <fcntl.h>  
#include <errno.h>  
#include <cutils/log.h>  
#include <cutils/atomic.h>  
  
#define DEVICE_NAME "/dev/tea5767"  
#define MODULE_NAME "tea5767"  
#define MODULE_AUTHOR "pngcui"

#define Search 3
#define AutoSearch 4
#define SETFREQ   5
#define OPENMUTE  8
#define CLOSEMUTE 9
#define SHUTDOWN 10
#define ADC 100
#define FREQ 101

/*设备打开和关闭接口*/  
static int tea5767_device_open(const struct hw_module_t* module, const char* name, struct hw_device_t** device);  
static int tea5767_device_close(struct hw_device_t* device);   

/*设备访问接口*/
static int fm_getADC(struct tea5767_device_t* dev);
static int fm_getFreq(struct tea5767_device_t* dev);
static void fm_setFreq(struct tea5767_device_t* dev, int freq);
static void fm_searchNextorPreFreq(struct tea5767_device_t* dev, int enable);
static void fm_setNextorPreFreq(struct tea5767_device_t* dev, int enable);
static void fm_enableMute(struct tea5767_device_t* dev, int enable);
static int  fm_getIsMute(struct tea5767_device_t* dev);
static void fm_autoSearch(struct tea5767_device_t* dev);
  
/*模块方法表*/  
static struct hw_module_methods_t tea5767_module_methods = {  
    open: tea5767_device_open  
};  

/*
*
*实例变量名必须为HAL_MODULE_INFO_SYM，tag也必须为HARDWARE_MODULE_TAG，这是Android硬件抽象层规范规定的。
*/

/*模块实例变量*/  
struct tea5767_module_t HAL_MODULE_INFO_SYM = {  
    common: {  
        tag: HARDWARE_MODULE_TAG,  
        version_major: 1,  
        version_minor: 0,  
        id: tea5767_HARDWARE_MODULE_ID,  
        name: MODULE_NAME,  
        author: MODULE_AUTHOR,  
        methods: &tea5767_module_methods,  
    }
};  

static int tea5767_device_open(const struct hw_module_t* module, const char* name, struct hw_device_t** device) {  
    struct tea5767_device_t* dev;
	dev = (struct tea5767_device_t*)malloc(sizeof(struct tea5767_device_t));  
      
    if(!dev){  
        LOGE("tea5767 Stub: failed to malloc space");  
        return -EFAULT;
    }  
  
    memset(dev, 0, sizeof(struct tea5767_device_t));  
    dev->common.tag = HARDWARE_DEVICE_TAG;  
    dev->common.version = 0;  
    dev->common.module = (hw_module_t*)module;  
    dev->common.close = tea5767_device_close;

	dev->getADC = fm_getADC;
	dev->getFreq = fm_getFreq;
	dev->setFreq = fm_setFreq;
	dev->searchNextorPreFreq = fm_searchNextorPreFreq;
	dev->setNextorPreFreq = fm_setNextorPreFreq;
	dev->enableMute = fm_enableMute;
	dev->autoSearch = fm_autoSearch;
  	dev->getIsMute = fm_getIsMute;

    if((dev->fd = open(DEVICE_NAME, O_RDWR)) == -1) {  
        LOGE("tea5767 Stub: failed to open /dev/tea5767 -- %s.", strerror(errno));
		free(dev);  
        return -EFAULT;  
    }  
  
    *device = &(dev->common);  
    LOGI("tea5767 Stub: open /dev/tea5767 successfully.");  
  
    return 0;  
}

static int tea5767_device_close(struct hw_device_t* device) {  
    struct tea5767_device_t* tea5767_device = (struct tea5767_device_t*)device;  
  
    if(tea5767_device) {  
        close(tea5767_device->fd);  
        free(tea5767_device);  
    }  
      
    return 0;  
}

static int fm_getADC(struct tea5767_device_t* dev){
	LOGI("fm get ADC....");
	
	int ret = ioctl(dev->fd,ADC, 0);
	LOGI("ret = %d",ret);

	return ret;
}

static int fm_getFreq(struct tea5767_device_t* dev){
	LOGI("fm get fm_getFreq....");
	
	int ret = ioctl(dev->fd,100, 0);
	LOGI("ret = %d",ret);

	return ret;
}

static void fm_setFreq(struct tea5767_device_t* dev, int freq){
	LOGI("fm get fm_setFreq....");
	
	int ret = ioctl(dev->fd,FREQ, 0);
	LOGI("ret = %d",ret);

	return ret;
}
static void fm_searchNextorPreFreq(struct tea5767_device_t* dev, int enable){
	LOGI("fm get fm_searchNextorPreFreq....");
	
	int ret = ioctl(dev->fd,Search, enable);

	LOGI("ret = %d",ret);

	return ret;
}

static void fm_setNextorPreFreq(struct tea5767_device_t* dev, int enable){
	LOGI("fm get fm_setNextorPreFreq....");
	
	int ret = ioctl(dev->fd,Search, enable);
	LOGI("ret = %d",ret);

	return ret;
}

static void fm_enableMute(struct tea5767_device_t* dev, int enable){
	LOGI("fm get fm_enableMute....");
	
	int ret;
	if(enable){
		ret = ioctl(dev->fd,OPENMUTE, 0);
	}else{
		ret = ioctl(dev->fd,CLOSEMUTE, 0);
	}
	LOGI("ret = %d",ret);

	return ret;
}

static int fm_getIsMute(struct tea5767_device_t* dev){
	LOGI("fm get fm_getIsMute....");
	
	int ret = ioctl(dev->fd,CLOSEMUTE, 0);
	LOGI("ret = %d",ret);

	return ret;
}

static void fm_autoSearch(struct tea5767_device_t* dev){
	LOGI("fm get fm_autoSearch....");
	
	int ret = ioctl(dev->fd,AutoSearch, 87500);
	LOGI("ret = %d",ret);

	return ret;
}
