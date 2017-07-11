package com.pngcui.fm;


public class Jni {
	
	public native int       Open();
    public native int       Close();
    public native int       Ioctl(int num, int en);
	public native int       GetAdc();
	public native int       GetFreq();
	public native int       SetFreq(int arg);
	public native int       OpenMute();
	public native int       CloseMute();
	public native int       HandSearch(int arg);
	public native int       AutoSearch(int arg,int cmd);
	public native int       SetShutDown();
}
