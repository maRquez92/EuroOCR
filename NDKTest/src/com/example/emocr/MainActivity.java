package com.example.emocr;
 
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Parcelable;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
//OpenCV
import android.widget.Toast;
 
public class MainActivity extends Activity {
   
	protected static final String TAG = "MainActivity"; 

	public static final String DATA_PATH = Environment
			.getExternalStorageDirectory().toString() + "/EuroOCR/";
	
	private static final int IMAGE_REQUEST = 1888;

	public static final String LANG = "por";
	
	
	private static ProgressDialog progressDialog;
	private static ImageView imageView;
    //public static EditText editText;
    
	private Bitmap bitmap;
   
	
	
    public static Context mainContext;
    public static boolean onProcess = false;
    public static boolean imageLoaded = false;
    
    
    
	private BaseLoaderCallback  mLoaderCallback = new BaseLoaderCallback(this)
	{
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");

                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };
    
	
    /** Call on every application resume **/
    @Override
    protected void onResume()
    {
        Log.i(TAG, "Called onResume");
        super.onResume();

        Log.i(TAG, "Trying to load OpenCV library");
        if (!OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_8, this, mLoaderCallback))
        {
            Log.e(TAG, "Cannot connect to OpenCV Manager");
        }
    }
    
 
  @Override
    public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	    setContentView(R.layout.activity_main);

	    mainContext = this; 	    
        //editText =(EditText)findViewById(R.id.numbers);

        MainActivity.imageView = (ImageView)this.findViewById(R.id.imageView1);
        Button photoButton = (Button) this.findViewById(R.id.processButton);
        
        photoButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

            	if(onProcess)
            	{
              	  	Toast.makeText(mainContext, "Processing Image...",Toast.LENGTH_LONG).show();
            	}
            	else
            	{
                	openImageIntent();
            	}
            }
        });
        
        Button photoButton2 = (Button) this.findViewById(R.id.button2);
        
        photoButton2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {            	

            	if(onProcess)
            	{
              	  	Toast.makeText(mainContext, "Processing Image...",Toast.LENGTH_LONG).show();
            	}
            	else if(!imageLoaded)
            	{
              	  	Toast.makeText(mainContext, "Load an Image First!",Toast.LENGTH_LONG).show();
            	}
            	else
            	{
            		progressDialog = ProgressDialog.show(mainContext, "Please Wait", "Processing Image");

            		onProcess = true;
              	  	Toast.makeText(mainContext, "Please Wait...",Toast.LENGTH_LONG).show();

            		//ProcessImage procIm = new ProcessImage(getApplicationContext(),(String)(MyUtils.getRealPathFromURI(getApplicationContext(), outputFileUri)));
                    ProcessImage procIm = new ProcessImage(getApplicationContext(),bitmap);
                    //Call execute 
                    procIm.execute();
            	}
            	 
            }
        });
        
        Button menuNumbersButton = (Button) this.findViewById(R.id.button3);

        menuNumbersButton.setOnClickListener(new View.OnClickListener() {
	        @Override
	        public void onClick(View v)
	        {        
		        //define a new Intent for the second Activity
		  		Intent intent = new Intent(MainActivity.this,MenuNumbers.class);
		  		
		  		//start the second Activity
		  		startActivity(intent);
	        }
        });
        
    }
  
  /*
  public static void setText(String text)
  {
	  editText.setText(text);
  }
  
  public static void appendText(String text)
  {
	  editText.append(text);
  }
  */
  
  public static void setImage(Bitmap image)
  {
  	  imageView.setImageBitmap(image);
  }
  
  
  
  private Uri outputFileUri;

  private void openImageIntent()
  {

	  // Determine Uri of camera image to save.
	  final File root = new File(Environment.getExternalStorageDirectory() + File.separator + "MushTac" + File.separator);
	  root.mkdirs();
	  //final String fname = Utils.getUniqueImageFilename();
	  final String fname = "img_"+ System.currentTimeMillis() + ".jpg";
	  final File sdImageMainDirectory = new File(root, fname);
	  outputFileUri = Uri.fromFile(sdImageMainDirectory);

      // Camera.
      final List<Intent> cameraIntents = new ArrayList<Intent>();
      final Intent captureIntent = new Intent(android.provider.MediaStore.ACTION_IMAGE_CAPTURE);
      final PackageManager packageManager = getPackageManager();
      final List<ResolveInfo> listCam = packageManager.queryIntentActivities(captureIntent, 0);
      for(ResolveInfo res : listCam)
      {
          final String packageName = res.activityInfo.packageName;
          final Intent intent = new Intent(captureIntent);
          intent.setComponent(new ComponentName(res.activityInfo.packageName, res.activityInfo.name));
          intent.setPackage(packageName);
          intent.putExtra(MediaStore.EXTRA_OUTPUT, outputFileUri);
          cameraIntents.add(intent);
      }

      // Filesystem.
      final Intent galleryIntent = new Intent();
      galleryIntent.setType("image/*");
      galleryIntent.setAction(Intent.ACTION_GET_CONTENT);

      // Chooser of filesystem options.
      final Intent chooserIntent = Intent.createChooser(galleryIntent, "Select Source");

      // Add the camera options.
      chooserIntent.putExtra(Intent.EXTRA_INITIAL_INTENTS, cameraIntents.toArray(new Parcelable[]{}));

      startActivityForResult(chooserIntent, IMAGE_REQUEST);
  }

  @Override
  protected void onActivityResult(int requestCode, int resultCode, Intent data)
  {
      if(resultCode == RESULT_OK)
      {
          if(requestCode == IMAGE_REQUEST)
          {
              final boolean isCamera;
              if(data == null)
              {
                  isCamera = true;
              }
              else
              {
                  final String action = data.getAction();
                  if(action == null)
                  {
                      isCamera = false;
                  }
                  else
                  {
                      isCamera = action.equals(android.provider.MediaStore.ACTION_IMAGE_CAPTURE);
                  }
              }

              Uri selectedImageUri;
              if(isCamera)
              {
                  selectedImageUri = outputFileUri;
              }
              else
              {
                  selectedImageUri = data == null ? null : data.getData();
              }
                            
              outputFileUri = selectedImageUri;
              
              try
              {
            	  bitmap = MediaStore.Images.Media.getBitmap(this.getContentResolver(), outputFileUri);
                  imageView.setImageURI(outputFileUri);
                  imageLoaded = true;
              }
              catch (IOException e)
              {
            	  // TODO Auto-generated catch block
            	  e.printStackTrace();
                  //imageView.setImageURI(outputFileUri);
              }
              
              

          }
      }
  }
 
	public static void finishProcess()
	{
		onProcess = false;
		progressDialog.dismiss();
		
		//define a new Intent for the second Activity
		Intent intent = new Intent(mainContext,MenuNumbers.class);
		
		//start the second Activity
		mainContext.startActivity(intent);
	}
  
  
  @Override
  public void onConfigurationChanged(Configuration newConfig)
  {
    super.onConfigurationChanged(newConfig);
    setContentView(R.layout.activity_main);
  }
  
  
  
  
}