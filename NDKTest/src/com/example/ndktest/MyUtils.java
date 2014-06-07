package com.example.ndktest;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import android.content.Context;
import android.content.res.AssetManager;
import android.database.Cursor;
import android.net.Uri;
import android.provider.MediaStore;
import android.util.Log;

public class MyUtils {
	
	private static String TAG = "Utils"; //this.getClass().getName();

	
	// Compare numbers with result
    /*
    int numberA[] = new int[] {10,20,30,40,50};
    int numberB[] = new int[] {17,20,30,40,50};
    int starsA[] = new int[] {1,5};
    int starsB[] = new int[] {5,8};
    
    int resNumbers = numberMatch(numberA, numberB, 5);
    int resStars = numberMatch(starsA, starsB, 2);
    */
	
	
	private int numberMatch(int[] original, int[] test, int arraySize)
	  {
		  int matches = 0;
		  
		  for(int i = 0 ; i < arraySize ; i++)
		  {
			  for(int j = 0 ; j < arraySize ; j++)
			  {
				  if(original[i] == test[j])
				  {
					  matches++;
				  }
			  }
		  }
		  
		  
		  return matches;
	  }
	


public static String getRealPathFromURI(Context context, Uri contentUri) {
  Cursor cursor = null;
  try { 
    String[] proj = { MediaStore.Images.Media.DATA };
    cursor = context.getContentResolver().query(contentUri,  proj, null, null, null);
    int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
    cursor.moveToFirst();
    return cursor.getString(column_index);
  } finally {
    if (cursor != null) {
      cursor.close();
    }
  }
}


public static String getFileNameByUri(Context context, Uri uri)
{
    String fileName="unknown";//default fileName
    Uri filePathUri = uri;
    if (uri.getScheme().toString().compareTo("content")==0)
    {      
        Cursor cursor = context.getContentResolver().query(uri, null, null, null, null);
        if (cursor.moveToFirst())
        {
            int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);//Instead of "MediaStore.Images.Media.DATA" can be used "_data"
            filePathUri = Uri.parse(cursor.getString(column_index));
            fileName = filePathUri.getLastPathSegment().toString();
        }
    }
    else if (uri.getScheme().compareTo("file")==0)
    {
        fileName = filePathUri.getLastPathSegment().toString();
    }
    else
    {
        fileName = fileName+"_"+filePathUri.getLastPathSegment();
    }
    return fileName;
}



public static String expandDataIfNeeded(Context context) {

	String lang = "por";
	File path = context.getFilesDir();
	File tessData = new File(path, "tessdata");
	if (!tessData.exists())
		tessData.mkdir();
	
	File trained = new File(tessData, lang+".traineddata");
	if (!trained.exists()) {

		try {
			InputStream is = context.getResources().getAssets()
					.open("tesseract.zip");

			ZipInputStream zin = new ZipInputStream(is);
			ZipEntry ze = null;
			byte[] buffer = new byte[4096];
			while ((ze = zin.getNextEntry()) != null) {

				File out = new File(tessData, ze.getName());
				FileOutputStream fout = new FileOutputStream(out);
				for (int c = zin.read(buffer); c > 0; c = zin.read(buffer))
					fout.write(buffer, 0, c);

				zin.closeEntry();
				fout.close();

			}
			zin.close();
		} catch (Exception e) {
			Log.e("OCR TEST", "Error while expanding tesseract data", e);
		}
	}
	
	return path.getAbsolutePath();
}
	
	
}
