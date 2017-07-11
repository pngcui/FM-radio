# FM-radio
基于Android4.0.3 Linux3.0，增加Linux驱动，改动Frameworks层，增加APK

----------------
1.tea5767驱动程序
2.Frameworks层增加API接口
3.Android应用程序控制tea5767芯片实现收音机功能      

----------20170711  update    
1.增加驱动代码    
	iTop4412_Kernel_3.0/drivers/char/tea5767.c    
	iTop4412_Kernel_3.0/drivers/char/i2c_tea5767.c    
	iTop4412_Kernel_3.0/drivers/char/i2c_tea5767.h    
   
2.增加HAL层代码    
	hardware/libhardware/modules/hwfm/hwtea5767.c    
	hardware/libhardware/modules/hwfm/Android.mk    
	hardware/libhardware/modules/Android.mk    
	hardware/libhardware/include/hardware/hw_tea5767.h    
	   
3.增加JNI代码    
	frameworks/base/services/jni/com_android_server_FMService.cpp    
	frameworks/base/services/jni/onload.cpp    
	frameworks/base/services/jni/Android.mk    
	   
4.增加Frameworks代码     
	frameworks/base/core/java/android/os/IFMService.aidl   
	frameworks/base/Android.mk   
	需要先编译生成java文件：mmm frameworks/base    
	frameworks/base/services/java/com/android/server/FMService.java
	frameworks/base/services/java/com/android/server/SystemServer.java    
	   
5.增加APP代码     
	注：暂未调用系统的API，而是采用APP->JNI(NDK)->Driver模式，后续有时间会更改为调用系统API，即import以下包获取FMservice服务      
	import android.os.ServiceManager;   
	import android.os.IFMService;     
	private IFMService fmService = null;     
	fmService = IFMService.Stub.asInterface(ServiceManager.getService("fm5767"));    
