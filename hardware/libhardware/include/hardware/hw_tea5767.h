#ifndef ANDROID_TEA5767_INTERFACE_H  
#define ANDROID_TEA5767_INTERFACE_H  
#include <hardware/hardware.h>  
  
__BEGIN_DECLS  
  
/*定义模块ID*/  
#define tea5767_HARDWARE_MODULE_ID "tea5767"  
  
/*硬件模块结构体*/  
struct tea5767_module_t {  
    struct hw_module_t common;  
};  
  
/*硬件接口结构体*/  
struct tea5767_device_t {  
    struct hw_device_t common;  
    int fd;   
	int (*getADC)(struct tea5767_device_t* dev);
	int (*getFreq)(struct tea5767_device_t* dev);
	void (*setFreq)(struct tea5767_device_t* dev, int freq);
	void (*searchNextorPreFreq)(struct tea5767_device_t* dev, int enable);
	void (*setNextorPreFreq)(struct tea5767_device_t* dev, int enable);
	void (*enableMute)(struct tea5767_device_t* dev, int enable);
	int (*getIsMute)(struct tea5767_device_t* dev);
	void (*autoSearch)(struct tea5767_device_t* dev);
};
  
__END_DECLS  
  
#endif 
