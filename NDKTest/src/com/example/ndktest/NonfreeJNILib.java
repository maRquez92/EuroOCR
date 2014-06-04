package com.example.ndktest;

import java.util.ArrayList;
import java.util.List;

import org.opencv.core.Mat;

import android.util.Log;

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
    
	
    //public static native void runDemo();
	public static void drawSiftKeyPoints( Mat image )
	{
		nativeDrawSiftKp( image.nativeObj );
		//return (Mat)nativeDrawSiftKp( image.nativeObj );
	}
/*	
	public static Mat main( Mat talao , Mat logo , Mat photo , StringBuilder out )
	{
		Mat res = new Mat();
		String resText = maine(talao.nativeObj,logo.nativeObj,photo.nativeObj,res.nativeObj);
		out.append(resText);
		return res.clone();
	}
	*/
	
	public static List<String> main( Mat talao , Mat logo , String imagePath , String tessdataPath, Mat res )
	{
		String[] results = maine(talao.nativeObj,logo.nativeObj,imagePath,tessdataPath,res.nativeObj);
		
		List<String> list = new ArrayList<String>();
		if(results != null)
			for (String str : results)
				list.add(str);
		
		return list;
	}
	
	
	public static String getJNIS()
	{
		return "no";//getJniString();
	}
	
	
	public String messageMe(String text)
	{
	    //Log.i(TAG, text);
	   // MainActivity.setNumbers("NATIVE INVOCATION OF JAVA!");
	    return text;
	}
	
	
    //public static native void runDemo(long mat, long rMat);
	//public static native String getJniString();
	public static native String[] maine(long talao, long logo, String photoPath, String tessdataPath, long res);
    public static native void nativeDrawSiftKp( long inputImage );
}