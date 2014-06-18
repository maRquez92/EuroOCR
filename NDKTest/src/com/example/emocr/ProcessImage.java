package com.example.emocr;

import java.io.IOException;
import java.io.InputStream;
import java.util.List;

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Scalar;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.os.SystemClock;
import android.util.Log;

public class ProcessImage extends AsyncTask<String, Void, Void>
{
    
   private String TAG = this.getClass().getName();

	   
   // Required initialization
   private AssetManager assets;  
   private Bitmap input;
   private Bitmap resultBitmap;
   private Bitmap talaoBitmap;
   private Bitmap logoBitmap;
   
   private String imagePath;
   private String tessdataPath;
   private Context context;
   private List<String> results;
   
  // public ProcessImage(Context myContext, String imageAbsolutePath)
   public ProcessImage(Context myContext, Bitmap image)
    {
        super();
        // do stuff
        input = image;
        assets = myContext.getAssets();
        context = myContext;
        //imagePath = imageAbsolutePath;
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
    	// Timer
    	long startTime = SystemClock.elapsedRealtime();

    	
    	Mat talaoMat = new Mat ( talaoBitmap.getHeight(), talaoBitmap.getWidth(), CvType.CV_8U, new Scalar(4));
    	Utils.bitmapToMat(talaoBitmap, talaoMat);
		    
	    Mat logoMat = new Mat ( logoBitmap.getHeight(), logoBitmap.getWidth(), CvType.CV_8U, new Scalar(4));
	    Utils.bitmapToMat(logoBitmap, logoMat);
		    
	    Mat ImageMat = new Mat ( input.getHeight(), input.getWidth(), CvType.CV_8U, new Scalar(4));
	    Utils.bitmapToMat(input, ImageMat);
		        	
	    // Copy tessdata if not copied yet
  	  	tessdataPath = MyUtils.expandDataIfNeeded(context);

  	  	
  	  	
  	  	Mat resultMat = new Mat();

  	  	results = NonfreeJNILib.main(talaoMat,logoMat,ImageMat,tessdataPath,resultMat);

  	  	//Log.i(TAG, "RESULTBITMAP");

        //resultBitmap = Bitmap.createBitmap(resultMat.cols(),  resultMat.rows(),Bitmap.Config.ARGB_8888);;
  	  
  	  	//Utils.matToBitmap(resultMat, resultBitmap);

  	  	
  	  	// Timer
  	  	long endTime = SystemClock.elapsedRealtime();
		long elapsedMilliSeconds = endTime - startTime;
		double elapsedSeconds = elapsedMilliSeconds / 1000.0;
  	  	Log.i(TAG, "elapsedTime= "+ elapsedSeconds);

  	  	return null;     		
  		
    }
      
    protected void onPostExecute(Void unused)
    {
  	  /*
  	  for (String str : results)
  	  {
  		  MainActivity.appendText(str+"\n");
  	  }
  	  */
  	  
  	  MyUtils.splitRecognizedStrings(results);

  	  MainActivity.setImage(resultBitmap);
  	  
  	  MainActivity.finishProcess();
  	  
    }
      
}
