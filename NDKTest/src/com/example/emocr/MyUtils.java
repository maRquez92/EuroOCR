package com.example.emocr;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.List;
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

	public static int[][] mynumbers = new int[10][5];
    public static int[][] mystars = new int[10][2];
    
    public static int nBets = 0;
    public static ArrayList<String> numbersList = new ArrayList<String>();
    public static ArrayList<String> starsList = new ArrayList<String>();

    
    public static String year = "????";
    public static String month = "??";
    public static String day = "??";
    
	public static void splitRecognizedStrings(List<String> results)
	{
		boolean isDate = false;
		boolean isNumbers = false;
		int nBet = 0;
		
		// Clean/Reset old values
		numbersList.clear();
		starsList.clear();
		nBets = 0;
		
		int actualYear = Calendar.getInstance().get(Calendar.YEAR);
		int actualMonth = Calendar.getInstance().get(Calendar.MONTH) + 1;	// calendar starts from 0
		int actualDay = Calendar.getInstance().get(Calendar.DAY_OF_MONTH);
		
		Calendar calendar = new GregorianCalendar();

		
		int lastLineType = 0;	// numbers - 1 | stars - 2
		boolean lastDateNoProblem = false;
		
		for(String line : results)
		{
			Log.i("OCR TEST", "line:"+line);
			
			if(line.compareTo("INIT_DATES") == 0)
			{
				isDate = true;
			}
			else if(line.compareTo("INIT_NUMBERS") == 0)
			{
				isNumbers = true;
				isDate = false;
			}
			
			if(isDate && !lastDateNoProblem)
			{
				int errors = 0;
				int iyear = 0;
				int imonth = 0;
				int iday = 0;
				
				String[] dateNumbers = line.split("\\/");
				try
				{
					iyear = Integer.parseInt(dateNumbers[0].trim());
					if(iyear > actualYear)
					{
						iyear = actualYear;
					}
				}
				catch(Exception e)
				{
					iyear = actualYear;
					errors++;
				}
				try
				{
					Log.i("OCR TEST", "imonth ="+imonth);

					imonth = Integer.parseInt(dateNumbers[1].trim());
					Log.i("OCR TEST", "imonth ="+imonth);
					
					if(imonth > 12)
					{
						imonth = actualMonth;
						Log.i("OCR TEST", "Entrou imonth ="+imonth);

					}
				}
				catch(Exception e)
				{
					imonth = actualMonth;
					Log.i("OCR TEST", "Erro imonth ="+imonth);

					errors++;
				}
				try
				{
					iday = Integer.parseInt(dateNumbers[2].trim());//.substring(0, 2));
					if(iday > calendar.getActualMaximum(Calendar.DAY_OF_MONTH))
					{
						iday = actualDay;
					}
				}
				catch(Exception e)
				{
					iday = actualDay;
					errors++;
				}
				
				//set date
				calendar.set(Calendar.YEAR, iyear);
				calendar.set(Calendar.MONTH, imonth - 1);
				calendar.set(Calendar.DAY_OF_MONTH, iday);
				
				int weekDay = calendar.get(Calendar.DAY_OF_WEEK);
				//int weekDay = getWeekday(iyear, imonth, iday);
				

				// If day of week it's not tuesday or friday
				while( weekDay != 3 && weekDay != 6 )
				{
					weekDay--;
					calendar.add(Calendar.DAY_OF_MONTH,-1);
				}
				

				year = Integer.toString(calendar.get(Calendar.YEAR));
				month = format(calendar.get(Calendar.MONTH) + 1);
				day = format(calendar.get(Calendar.DAY_OF_MONTH));
				
//				year = Integer.toString(iyear);
//				month = format(imonth);
//				day = format(iday);
				
				if(errors < 1)
				{
					lastDateNoProblem = true;
				}

				Log.i("OCR TEST", "FINAL DATE ="+year+"/"+month+"/"+day);

			}
			else if (isNumbers)
			{
				
				String[] numbers = line.split("\\s+",5);

				// Numbers
				if(numbers.length > 3)
				{
					if(lastLineType == 1)
					{
						for(int i = 0 ; i < 2 ; i++)
						{
							mystars[nBet][i] = 0;
							//starsList.add("?");
						}
						nBet++;
					}
					
					int i = 0;
					for(String number : numbers)
					{
						try
						{
							int numAux = Integer.parseInt(number.trim());
							mynumbers[nBet][i] = (numAux<=50)?numAux:0;
							//numbersList.add(number.trim());

						}
						catch(Exception e)
						{
							mynumbers[nBet][i] = 0;
							//numbersList.add(number.trim());
						}
						i++;
					}
					
					lastLineType = 1;
				}
				else // stars
				{
					
					int i = 0;
					for(String number : numbers)
					{
						try
						{
							int numAux = Integer.parseInt(number.trim());
							mystars[nBet][i] = (numAux<=11)?numAux:0;
							//starsList.add(number.trim());
						}
						catch(Exception e)
						{
							if(i<2)
							{
								mystars[nBet][i] = 0;
								//starsList.add("?");
							}
								
						}
						i++;
					}
					
					if(lastLineType == 1)
					{
						nBet++;
					}
					lastLineType = 2;
				}
				
				
			}

		}
		
		int nIndex = 0;
		int sIndex = 0;

		for(int bet=0 ; bet < nBet ; bet++)
	    {
 
        	for(int x=0;x<5;x++)
		    {
            	Log.i("MyUtils","bet="+bet+" Numbers: "+mynumbers[bet][x] );
            	numbersList.add(format(mynumbers[bet][x]));
            	//Log.i("MyUtils","bet="+bet+" Numbers: "+numbersList.get(nIndex));
            	//nIndex++;
		    }
			 
        	for(int x=0;x<2;x++)
		    {
            	Log.i("MyUtils","bet="+bet+" Stars: "+mystars[bet][x] );
            	starsList.add(format(mystars[bet][x]));
            	//Log.i("MyUtils","bet="+bet+" Stars: "+starsList.get(sIndex) );
            	//sIndex++;
		    }
	    }
		
		nBets = nBet;
			
	}
	
	public static String format(int x)
	{
		return (x==0)?"?":(((x<10)?"0":"") + x);
	}
	
	public static int getWeekday(int y, int m, int d)
	{
		Calendar calendar = Calendar.getInstance();
		calendar.set(Calendar.YEAR, y);
		calendar.set(Calendar.MONTH, m);
		calendar.set(Calendar.DAY_OF_MONTH, d);
		return calendar.get(Calendar.DAY_OF_WEEK);
	}
	
	
	public static int numberMatch(int[] original, int[] test, int arraySize)
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

public static boolean isNumeric(String str)
{
    for (char c : str.toCharArray())
    {
        if (!Character.isDigit(c)) return false;
    }
    return true;
}
	
	
}
