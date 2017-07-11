#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>
#include <android/log.h>
#include <sys/ioctl.h>

#include "com_pngcui_fm.h"

#undef TCSAFLUSH
#define TCSAFLUSH TCSETSF
#ifndef _TERMIOS_H_
#define _TERMIOS_H_
#endif

#define SEARCH 3
#define AUTOSEARCH 4

#define SETFREQ   5

#define OPENMUTE  8
#define CLOSEMUTE 9
#define SHUTDOWN 10

#define ADC 100
#define FREQ 101

int fd = 0;


/*open()*/
JNIEXPORT jint JNICALL Java_com_pngcui_fm_Jni_Open
	(JNIEnv *env , jobject obj){

		if(fd<=0)
			fd = open("/dev/tea5767",O_RDWR|O_NDELAY);
		if(fd<=0)
			__android_log_print(ANDROID_LOG_INFO,"serial","open /dev/tea5767 Error.. fd =  %d",fd);
		else
			__android_log_print(ANDROID_LOG_INFO,"serial","open /dev/tea5767 Sucess fd = %d",fd);

		return 0;
	}
	
/*Close()*/
JNIEXPORT jint JNICALL Java_com_pngcui_fm_Jni_Close
	(JNIEnv *env,jobject obj){
		
		if(fd > 0)
			close(fd);

		return 0;
	}
	
/*Ioctl()*/
JNIEXPORT jint JNICALL Java_com_pngcui_fm_Jni_Ioctl
	(JNIEnv *env,jobject obj,jint cmd , jint arg){
		
		ioctl(fd,cmd,arg);
		//__android_log_print(ANDROID_LOG_INFO,"serial","Ioctl and cmd =  %d,arg = %d",cmd,arg);
		return 0;
	}

/*getADC()*/
JNIEXPORT jint JNICALL Java_com_pngcui_fm_Jni_GetAdc
	(JNIEnv *env,jobject obj){
		jint adc = 0;

		adc = ioctl(fd,ADC,0);
		//__android_log_print(ANDROID_LOG_INFO,"serial","Ioctl and ADC =  %d",adc);
		return adc;
	}

/*getFREQ()*/
JNIEXPORT jint JNICALL Java_com_pngcui_fm_Jni_GetFreq
	(JNIEnv *env,jobject obj){
		jint freq = 0;

		freq = ioctl(fd,FREQ,0);
		//__android_log_print(ANDROID_LOG_INFO,"serial","Ioctl and freq =  %d",freq);
		return freq;
}

/*setFreq()*/
JNIEXPORT jint JNICALL Java_com_pngcui_fm_Jni_SetFreq
	(JNIEnv *env,jobject obj,jint arg){

	jint curFreq = 0;

	curFreq = ioctl(fd,SETFREQ,arg);

	return curFreq;
}

/*OpenMute()*/
JNIEXPORT jint JNICALL Java_com_pngcui_fm_Jni_OpenMute
	(JNIEnv *env,jobject obj){

	ioctl(fd,OPENMUTE,0);

	return 0;
}

/*CloseMute()*/
JNIEXPORT jint JNICALL Java_com_pngcui_fm_Jni_CloseMute
	(JNIEnv *env,jobject obj){

	ioctl(fd,CLOSEMUTE,0);

	return 0;
}

/*Search()*/
JNIEXPORT jint JNICALL Java_com_pngcui_fm_Jni_HandSearch
	(JNIEnv *env,jobject obj,jint arg){

	//jint getIt = 0;

	ioctl(fd,SEARCH,arg);
	//__android_log_print(ANDROID_LOG_INFO,"serial","Ioctl and arg =  %d",arg);
	return 0;
}

/*autoSearch()*/
JNIEXPORT jint JNICALL Java_com_pngcui_fm_Jni_AutoSearch
	(JNIEnv *env,jobject obj,jint arg,jint cmd){

	jint getIt = 0;

	getIt = ioctl(fd,cmd,arg);
	//__android_log_print(ANDROID_LOG_INFO,"serial","Ioctl and getIt =  %d",getIt);
	return getIt;
}

/*SetShutDown()*/
JNIEXPORT jint JNICALL Java_com_pngcui_fm_Jni_SetShutDown
	(JNIEnv *env,jobject obj){

	ioctl(fd,SHUTDOWN,0);

	return 0;
}


