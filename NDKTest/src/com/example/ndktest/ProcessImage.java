package com.example.ndktest;

import java.io.IOException;
import java.io.InputStream;

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Scalar;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.util.Log;

public class ProcessImage extends AsyncTask<String, Void, Void>
{
    
   private String TAG = "PROCCESS IMAGE";

	   
   // Required initialization
   private AssetManager assets;  
   private Bitmap input;
   private Bitmap resultBitmap;
   private Bitmap talaoBitmap;
   private Bitmap logoBitmap;
   
   private String imagePath;
   private String tessdataPath;
   private String resultString;
   private Context context;
   
    public ProcessImage(Context myContext, String imageAbsolutePath)
    {
        super();
        // do stuff
        imagePath = imageAbsolutePath;
        assets = myContext.getAssets();
        context = myContext;
    }
     
    protected void onPreExecute()
    {
    	
    	try
    	{
    		// get input stream
    		InputStream talao = assets.open("talao01.jpg");
    		InputStream logo = assets.open("logo.png");
    		// load image as Bitmap
    		talaoBitmap = BitmapFactory.decodeStream(talao);
    		logoBitmap = BitmapFactory.decodeStream(logo);
        
    	}
        catch(IOException ex)
        {
        	Log.i(TAG, "CANT LOAD TRAINING IMAGES");
      	  	return;
        }
    }

    // Call after onPreExecute method
    protected Void doInBackground(String... urls)
    {
		
    	Mat talaoMat = new Mat ( talaoBitmap.getHeight(), talaoBitmap.getWidth(), CvType.CV_8U, new Scalar(4));
    	Utils.bitmapToMat(talaoBitmap, talaoMat);
		    
	    Mat logoMat = new Mat ( logoBitmap.getHeight(), logoBitmap.getWidth(), CvType.CV_8U, new Scalar(4));
	    Utils.bitmapToMat(logoBitmap, logoMat);
		    
	    //Mat ImageMat = new Mat ( input.getHeight(), input.getWidth(), CvType.CV_8U, new Scalar(4));
	    //Utils.bitmapToMat(input, ImageMat);
		    
	
			
  	  	Log.i(TAG, "INVOKING NATIVE PART");
    	
  	  	tessdataPath = MyUtils.expandDataIfNeeded(context);

        StringBuilder output = new StringBuilder();
  	  	//Mat resultMat = NonfreeJNILib.main(talaoMat,logoMat,ImageMat,output);
  	  	Mat resultMat = NonfreeJNILib.main(talaoMat,logoMat,imagePath,tessdataPath,output);
  	  	//NonfreeJNILib.drawSiftKeyPoints(ImageMat);
  	  	resultString = output.toString();
  	  	Log.i(TAG, "RESULTBITMAP");

        resultBitmap = Bitmap.createBitmap(resultMat.cols(),  resultMat.rows(),Bitmap.Config.ARGB_8888);;
        //resultBitmap = Bitmap.createBitmap(ImageMat.cols(),  ImageMat.rows(),Bitmap.Config.ARGB_8888);;
  	  
  	  	Utils.matToBitmap(resultMat, resultBitmap);
  	  	//Utils.matToBitmap(ImageMat, resultBitmap);

  	  	return null;     		
  		
    }
      
    protected void onPostExecute(Void unused)
    {
  	  Log.i(TAG, "onPostExecute");
  	  MainActivity.setImage(resultBitmap);
  	 // String res = TesseractHandler.getText(resultBitmap, true);
  	  MainActivity.setText("Data="+resultString);
    }
      
}
