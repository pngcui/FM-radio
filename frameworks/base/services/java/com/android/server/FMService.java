package com.android.server;  

import android.content.Context;  
import android.os.IFMService;  
import android.util.Slog; 
 
public class FMService extends IFMService.Stub {  

    private static final String TAG = "FMService";  

    FMService() {  
		Slog.d(TAG,"FM init...");
        init_native();
    }

	public int getADC(){
		
		return getADC_native();
	}

	public int getFreq(){
		return getFreq_native();
	}

	public void setFreq(int freq){
		
		setFreq_native(freq);
		return ;
	}

	public void searchNextorPreFreq(int enable){
		
		searchNextorPreFreq_native(enable);
		return ;
	}
      
	public void setNextorPreFreq(int enable){
		
		setNextorPreFreq_native(enable);
		return ;
	}

	public void enableMute(int enable){
		
		enableMute_native(enable);
		return ;
	}

	public int getIsMute(){
		return getIsMute_native();
	}

	public void startAutoSearch(){
		
		startAutoSearch_native();
		return ;
	}

    private static native boolean init_native();  
	private static native int getADC_native();
	private static native int getFreq_native();  
	private static native void setFreq_native(int freq);
	private static native void searchNextorPreFreq_native(int enable);  
	private static native void setNextorPreFreq_native(int enable);
	private static native void enableMute_native(int enable);  
	private static native int getIsMute_native();
	private static native void startAutoSearch_native(); 

};  
