#define LOG_TAG "FMService_jni"  
#include "jni.h"  
#include "JNIHelp.h"  
#include "android_runtime/AndroidRuntime.h"  
#include <utils/misc.h>  
#include <utils/Log.h>  
#include <hardware/hardware.h>  
#include <hardware/hw_tea5767.h>  
#include <stdio.h> 

namespace android  
{  
    /*在硬件抽象层中定义的硬件访问结构体，参考<hardware/tea5767.h>*/  
    struct tea5767_device_t* tea5767_device = NULL;  

	//访问硬件的接口
	static jint tea5767_getADC(JNIEnv* env, jobject clazz){
		int adc = 0;
		if(!tea5767_device){
			LOGE("FM jni is not open..");
			return adc;
		}

		adc = tea5767_device->getADC(tea5767_device);
		LOGI("get fm adc = %d",adc);

		return adc;
	}

	static jint tea5767_getFreq(JNIEnv* env, jobject clazz){
		int freq = 0;
		if(!tea5767_device){
			LOGE("FM jni is not open..");
			return freq;
		}

		freq = tea5767_device->getFreq(tea5767_device);
		LOGI("get fm freq = %d",freq);

		return freq;
	}

	static void tea5767_setFreq(JNIEnv* env, jobject clazz, jint freq){

		if(!tea5767_device){
			LOGE("FM jni is not open..");
			return ;
		}

		tea5767_device->setFreq(tea5767_device,freq);
		LOGI("set fm freq = %d",freq);

		return ;
	}

	
	static void tea5767_searchNextorPreFreq(JNIEnv* env, jobject clazz, jint enable){

		if(!tea5767_device){
			LOGE("FM jni is not open..");
			return ;
		}

		tea5767_device->searchNextorPreFreq(tea5767_device,enable);
		LOGI("searchNextorPreFreq state = %d",enable);

		return ;
	}

	
	static void tea5767_setNextorPreFreq(JNIEnv* env, jobject clazz, jint enable){

		if(!tea5767_device){
			LOGE("FM jni is not open..");
			return ;
		}

		tea5767_device->setNextorPreFreq(tea5767_device,enable);
		LOGI("setNextorPreFreq state = %d",enable);

		return ;
	}

	
	static void tea5767_enableMute(JNIEnv* env, jobject clazz, jint enable){

		if(!tea5767_device){
			LOGE("FM jni is not open..");
			return ;
		}

		tea5767_device->enableMute(tea5767_device,enable);
		LOGI("enableMute state = %d",enable);

		return ;
	}

	static jint tea5767_getIsMute(JNIEnv* env, jobject clazz){
		int enable = 0;
		if(!tea5767_device){
			LOGE("FM jni is not open..");
			return enable;
		}

		enable = tea5767_device->getIsMute(tea5767_device);
		LOGI("getIsMute state = %d",enable);

		return enable;
	}

	
	static void tea5767_autoSearch(JNIEnv* env, jobject clazz){

		if(!tea5767_device){
			LOGE("FM jni is not open..");
			return ;
		}

		tea5767_device->autoSearch(tea5767_device);
		LOGI("fm start autoSearch");

		return ;
	}


   /*通过硬件抽象层定义的硬件模块打开接口打开硬件设备*/  
    static inline int tea5767_device_open(const hw_module_t* module, struct tea5767_device_t** device) { 
 
        return module->methods->open(module, tea5767_HARDWARE_MODULE_ID, (struct hw_device_t**)device);  
    }  
    
	/*通过硬件模块ID来加载指定的硬件抽象层模块并打开硬件*/  
    static jboolean tea5767_init(JNIEnv* env, jclass clazz) {  
        tea5767_module_t* module;  
          
		LOGI("tea5767 JNI: initializing......");  
        if(hw_get_module(tea5767_HARDWARE_MODULE_ID, (const struct hw_module_t**)&module) == 0) {  
            LOGI("tea5767 JNI: tea5767 Stub found.");  
            if(tea5767_device_open(&(module->common), &tea5767_device) == 0) {  
                LOGI("tea5767 JNI: tea5767 device is open.");  
                return 0;  
            }  
            LOGE("tea5767 JNI: failed to open tea5767 device.");  
            return -1;  
        }  
        LOGE("tea5767 JNI: failed to get tea5767 stub module.");  
        return -1;        
    }  

/*

*/
        /*JNI方法表*/  
    static const JNINativeMethod method_table[] = {  
        {"init_native", "()Z", (void*)tea5767_init},
		{"getADC_native", "()I", (void*)tea5767_getADC},
		{"getFreq_native", "()I", (void*)tea5767_getFreq},
		{"setFreq_native", "(I)V", (void*)tea5767_setFreq},
		{"searchNextorPreFreq_native", "(I)V", (void*)tea5767_searchNextorPreFreq},
		{"setNextorPreFreq_native", "(I)V", (void*)tea5767_setNextorPreFreq},
		{"enableMute_native", "(I)V", (void*)tea5767_enableMute},
		{"getIsMute_native", "()I", (void*)tea5767_getIsMute},
		{"startAutoSearch_native", "()V", (void*)tea5767_autoSearch},
    };  
        /*注册JNI方法*/  
    int register_android_server_FMService(JNIEnv *env) {  
    	return jniRegisterNativeMethods(env, "com/android/server/FMService", method_table, NELEM(method_table));  
    }  
}; 
