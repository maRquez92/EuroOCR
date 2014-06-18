package com.example.emocr;

import java.util.ArrayList;
import java.util.List;

import org.opencv.core.Mat;


public class NonfreeJNILib {
    
	private String TAG = "NonfreeJNILib"; //this.getClass().getName();

	static 
    {
	    try
	    { 
	    // Load necessary libraries.
	    	System.loadLibrary("lept");
	    	System.loadLibrary("tess");
	    	System.loadLibrary("opencv_java");
            System.loadLibrary("nonfree");
            System.loadLibrary("ndkCV");
	    }
	    catch( UnsatisfiedLinkError e )
	    {
	        System.err.println("Native code library failed to load.\n" + e);
	    }
    }
    
	
	
	public static List<String> main( Mat talao , Mat logo , Mat image , String tessdataPath, Mat res )
	{
		String[] results = maine(talao.nativeObj,logo.nativeObj,image.nativeObj,tessdataPath,res.nativeObj);
		
		List<String> list = new ArrayList<String>();
		if(results != null)
			for (String str : results)
				list.add(str);
		
		return list;
	}
	
	
	
	public static native String[] maine(long talao, long logo, long image, String tessdataPath, long res);
}