package com.example.ndktest;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

import android.os.AsyncTask;
import android.util.Log;


//Class with extends AsyncTask class

public class GetResult  extends AsyncTask<String, Void, Void> {
      
    // Required initialization
     
    private String Content;
    String data ="";
    String urlString;
     
    public GetResult(String i_url)
    {
        super();
        urlString = i_url;
    }
     
    protected void onPreExecute()
    {

         
    }

    // Call after onPreExecute method
    protected Void doInBackground(String... urls) {
         
  			URL url;

  	        String res = new String();
  	              
  			try 
  			{
  				// get URL content
  				url = new URL(urlString);
  				URLConnection conn = url.openConnection();

  				// open the stream and put it into BufferedReader
  				BufferedReader br = new BufferedReader(new InputStreamReader(conn.getInputStream()));

  				String inputLine;


  				while ((inputLine = br.readLine()) != null)
  				{
  					res = res.concat(inputLine );
  				}

  				br.close();

  				//System.out.println("Res="+res);
  		        Log.d("MainActivity","Res=" + res);
  		        Content = res;

  			}
  			catch (MalformedURLException e)
  			{
  				e.printStackTrace();
  			}
  			catch (IOException e)
  			{
  				e.printStackTrace();
  			}
				
  			return null;

  		
    }
      
    protected void onPostExecute(Void unused)
    {       
  	  	MainActivity.setText(Content);
    }
      
}