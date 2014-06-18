package com.example.emocr;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;


public class GetResult  extends AsyncTask<String, Void, Void> {
      
    // Required initialization
     
    private String content;
    String data ="";
    String urlString;
    Context context;
    
    public GetResult(Context mcontext, String i_url)
    {
        super();
        context = mcontext;
        
        urlString = i_url;
    }
    
    public GetResult(Context mcontext, String year, String month, String day)
    {
        super();
        context = mcontext;

        urlString = "https://nunofcguerreiro.com/api-euromillions?result="+year+"-"+month+"-"+day;
        Log.i("GetResult", urlString);
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

	        Log.d("MainActivity","Res=" + res);
	        content = res;

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
    	MenuNumbers.finalProcess(content);
    }
      
}