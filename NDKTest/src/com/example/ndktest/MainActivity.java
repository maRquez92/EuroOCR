package com.example.ndktest;
 
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.net.URLEncoder;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.ResponseHandler;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.BasicResponseHandler;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;
import org.json.JSONException;


import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.widget.EditText;
import android.widget.TextView;
 
public class MainActivity extends Activity {
   
  // load the library - name matches jni/Android.mk
  static {
    System.loadLibrary("ndkfoo");
  }
   
  // declare the native code function - must match the name in C Files
  private native String invokeNativeFunction();
   
  @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
         
        // this is where we call the native code
        String hello = invokeNativeFunction();
                
       
        //Create instance for Getting The Numbers (Async)
        LongOperation task = new LongOperation();
        //Call execute 
        task.execute();
        
        new AlertDialog.Builder(this).setMessage(hello).show();
    }
  
  
//Class with extends AsyncTask class
  
  private class LongOperation  extends AsyncTask<String, Void, Void> {
        
      // Required initialization
       
      private final HttpClient Client = new DefaultHttpClient();
      private String Content;
      private String Error = null;
      //private ProgressDialog Dialog = new ProgressDialog(RestFulWebservice.this);
      String data ="";
      
      //TextView uiUpdate = (TextView) findViewById(R.id.output);
      //TextView jsonParsed = (TextView) findViewById(R.id.jsonParsed);
      int sizeData = 0; 
      //EditText serverText = (EditText) findViewById(R.id.serverText);
       
       
      protected void onPreExecute() {
          // NOTE: You can call UI Element here.
            
          //Start Progress Dialog (Message)
          
          //Dialog.setMessage("Please wait..");
          //Dialog.show();
           
    	  /*
          try{
              // Set Request parameter
              data +="&" + URLEncoder.encode("data", "UTF-8") + "="+"http://www.omdbapi.com/?t=fringe";
                   
          } catch (UnsupportedEncodingException e) {
              // TODO Auto-generated catch block
              e.printStackTrace();
          }
          */
           
      }

      // Call after onPreExecute method
      protected Void doInBackground(String... urls) {
           
    			URL url;

    	        String res = new String();
    	              
    			try {
    				// get URL content
    				url = new URL("http://www.omdbapi.com/?i=7&t=breaking+bad");
    				URLConnection conn = url.openConnection();

    				// open the stream and put it into BufferedReader
    				BufferedReader br = new BufferedReader(
    	                             new InputStreamReader(conn.getInputStream()));

    				String inputLine;


    				while ((inputLine = br.readLine()) != null)
    				{
    					res = res.concat(inputLine );
    				}

    				br.close();

    				//System.out.println("Res="+res);
    		        Log.d("MainActivity","Res=" + res);
    		        Content = res;

    			} catch (MalformedURLException e) {
    				e.printStackTrace();
    			} catch (IOException e) {
    				e.printStackTrace();
    			}
				return null;

    		
      }
        
      protected void onPostExecute(Void unused) {
          // NOTE: You can call UI Element here.
            
          //new AlertDialog.Builder(this).setMessage("").show();
          EditText t=(EditText)findViewById(R.id.numbers);
          t.setText(Content);
          // Close progress dialog
          //Dialog.dismiss();
            /*
          if (Error != null) {
                
             // uiUpdate.setText("Output : "+Error);
                
          } else {
             
              // Show Response Json On Screen (activity)
             // uiUpdate.setText( Content );
               
           /****************** Start Parse Response JSON Data *************/
              
                
           
      
      }
        
  }
  
}