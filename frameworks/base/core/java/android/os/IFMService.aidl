package android.os;  
   
interface IFMService {  
	int getADC();
	int getFreq();
	void setFreq(int freq);
	void searchNextorPreFreq(int enable);
	void setNextorPreFreq(int enable);
	void enableMute(int enable);
	int getIsMute();
	void startAutoSearch();
}  
