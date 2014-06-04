package com.example.ndktest;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

//import com.googlecode.tesseract.android.TessBaseAPI;

//import com.googlecode.tesseract.android.TessBaseAPI;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.os.AsyncTask;
import android.os.Environment;
import android.util.Log;

public class TesseractHandler extends AsyncTask<Void, Void, List<String>>
{
	   
	private static String TAG = "TesseractHandler"; //this.getClass().getName();


	@Override
	protected List<String> doInBackground(Void... params) {
		// TODO Auto-generated method stub
		return null;
	}
	
	
}
