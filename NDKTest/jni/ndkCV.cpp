#include "ndkCV.hpp"


//#include <tesseract/baseapi.h>


using namespace cv;
using namespace std;



JNIEXPORT jstring JNICALL Java_com_example_ndktest_NonfreeJNILib_maine( JNIEnv* env, jobject obj, jlong talao, jlong logo, jstring photoPath, jstring tessdataPath, jlong res)
{
	Mat logoT = *((cv::Mat*)logo);
	Mat talaoT = *((cv::Mat*)talao);
	//Mat photoT = *((cv::Mat*)photo);
	const char * str = env->GetStringUTFChars(photoPath, nullptr);
	cv::Mat photorgb = cv::imread(str, CV_LOAD_IMAGE_COLOR);

	env->ReleaseStringUTFChars(photoPath, str);

	const char * cstr = env->GetStringUTFChars(tessdataPath, nullptr);
	std::string tessDataPath(cstr);

	env->ReleaseStringUTFChars(tessdataPath, cstr);

	//cv::cvtColor(*((cv::Mat*)talao), train, cv::COLOR_RGB2GRAY);
	//cv::Mat res = *((cv::Mat*)mat);

	cv::Mat trainLogo = cv::Mat(logoT.cols, logoT.rows, CV_8UC3);
	cv::cvtColor(logoT, trainLogo, cv::COLOR_RGBA2RGB );

	cv::Mat trainTalao = cv::Mat(talaoT.cols, talaoT.rows, CV_8UC3);
	cv::cvtColor(talaoT, trainTalao, cv::COLOR_RGBA2RGB );

	//cv::Mat photorgb = cv::Mat(photoT.cols, photoT.rows, CV_8UC3);
	//cv::cvtColor(photoT, photorgb, cv::COLOR_RGBA2RGB );

	float desiredWidth = 500;

	float widthProportion = desiredWidth/(photorgb.size().width);

	Mat photoResized;
	cv::resize(photorgb, photorgb, Size(photorgb.size().width*widthProportion,photorgb.size().height*widthProportion));


	// Deteção do talão completo-----------------------------------------------------------------------

	Mat img_matches;
	Point2i superioresquerdo;
	Point2i inferiordireito;
	int toSubtract;
	Mat rotationMatrix;

	img_matches = Detect(trainTalao,photorgb,&superioresquerdo,&inferiordireito,&toSubtract,&rotationMatrix);

	// Rodar e corrigir a prespectiva

	Mat recortResultA(cv::Size(img_matches.size().width-toSubtract,img_matches.size().height),img_matches.type());

	warpPerspective(photorgb,recortResultA,rotationMatrix,img_matches.size());

	Mat recortResult = recortResultA(Rect(0,0,trainTalao.size().width,trainTalao.size().height));

	//*((cv::Mat*)res) = processImage(img_matches,true,false);


	// Deteção do logo---------------------------------------------------------------------------------

	Mat img_matchesLogo;
	Point2i superioresquerdoLogo;
	Point2i inferiordireitoLogo;
	Mat rotationMatrixUnused;

	int aux;
	img_matchesLogo = Detect(trainLogo,recortResult,&superioresquerdoLogo,&inferiordireitoLogo,&aux,&rotationMatrixUnused);

	Rect recorteLogo(superioresquerdoLogo,inferiordireitoLogo);

	Mat recortResultLogo = img_matchesLogo(recorteLogo);

	superioresquerdoLogo.x = superioresquerdoLogo.x - trainLogo.cols;
	inferiordireitoLogo.x = inferiordireitoLogo.x - trainLogo.cols;


	//Recortar zona da Data----------------------------------------------------------------------------
	Point2i superioresquerdoData = inferiordireitoLogo + Point2i(0,-recortResultLogo.size().height/8);
	Point2i inferiordireitoData =  Point2i(recortResult.size().width, inferiordireitoLogo.y + recortResultLogo.size().height/4);
	Rect recorteData(superioresquerdoData,inferiordireitoData);

	Mat recortDataResult = recortResult(recorteData);

	recortDataResult = cutBorderData(recortDataResult);

	Mat auxDR;
	cvtColor( recortDataResult, auxDR, CV_BGR2GRAY );
	//equalizeHist(auxDR,auxDR);
	Mat processedDataImage = processImage(auxDR,false,true);


	*((cv::Mat*)res) = processedDataImage;



	img_matches.release();
	img_matchesLogo.release();
	recortResultLogo.release();
	recortDataResult.release();
	trainTalao.release();
	trainLogo.release();
	/*

	//Recordar zona dos Números------------------------------------------------------------------------

	Point2i superioresquerdoNumeros = Point2i(0, superioresquerdoLogo.y+recortResultLogo.rows+recortResult.rows/15);
	Point2i inferiordireitoNumeros =  Point2i(inferiordireitoLogo.x, recortResult.rows);

	Rect recorteNumeros(superioresquerdoNumeros,inferiordireitoNumeros);

	Mat recortNumerosResult = recortResult(recorteNumeros);


	Mat processedNumerosImage = processImage(recortNumerosResult,true,false);


	*((cv::Mat*)res) = processedNumerosImage;
	//*((cv::Mat*)res) = recortDataResult;


*/

	char* resStr = getText(tessDataPath,processedDataImage,true);


	return env->NewStringUTF(resStr);
	//return env->NewStringUTF("Processment Complete!");

}


char* getText(string tessDataPath, Mat image_with_text, bool data = false)
{
	__android_log_print(ANDROID_LOG_DEBUG, "OCR-DEMO",
					"Looking for tesseract in %s", tessDataPath.c_str());

	const char* lang = "eng";


		// Pass it to Tesseract API
	    tesseract::TessBaseAPI tess;

		if(tess.Init( (tessDataPath + "/tessdata").c_str(), lang, tesseract::OEM_DEFAULT))
		{

		}
	    //tess.SetVariable("tessedit_char_whitelist", "0123456789ehilmuorsõAEILMNOPRSTUVG/.=€:,-");
	    if(data)
		{
			tess.SetVariable("tessedit_char_whitelist", "0123456789/");
		}
		else
		{
		    tess.SetVariable("tessedit_char_whitelist", "0123456789EN.");
		}

		tess.SetVariable("tessedit_fix_hyphens", "false");
		tess.SetVariable("user_words_suffix", " user-words");
		//tess.SetVariable("load_system_dawg", "false");
		//tess.SetVariable("load_freq_dawg", "false");
		tess.SetVariable("tessedit_test_adaption", "true");
		tess.SetVariable("textord_force_make_prop_words", "false");
		tess.SetVariable("tessedit_fix_fuzzy_spaces", "true");

		tess.SetPageSegMode( tesseract::PSM_AUTO);
		tess.SetImage( (uchar*) image_with_text.data, image_with_text.cols, image_with_text.rows, 1, image_with_text.cols );

	    // Get the text
	    char* out = tess.GetUTF8Text();


	    delete &tess;

	    LOGI( "Complete! \n");
	    LOGI( "Res = %s! \n",out);

	return out;
}


Mat Detect(Mat train, Mat test, Point2i *supLeftOut, Point2i *infRightOut, int *additionalRowsOut, Mat *rotationMatrixOut)
{
	//Detect the keypoints using SURF Detector
    int minHessian = 500;

    SurfFeatureDetector detector( minHessian );
    std::vector<KeyPoint> kp_object;

	detector.detect( train, kp_object );

    //Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;
    Mat des_object;

	extractor.compute( train, kp_object, des_object );


	//namedWindow("des");
	//cv::resize(recortResult, recortResult, Size(recortResult.size().width/4,recortResult.size().height/4));
	//imshow( "des", des_object );

    FlannBasedMatcher matcher;


    std::vector<Point2f> obj_corners(4);

    //Get the corners from the train
    obj_corners[0] = cvPoint(0,0);
    obj_corners[1] = cvPoint( train.cols, 0 );
    obj_corners[2] = cvPoint( train.cols, train.rows );
    obj_corners[3] = cvPoint( 0, train.rows );

    int framecount = 0;
	Mat frame = test.clone();



    Mat des_image, img_matches;
    std::vector<KeyPoint> kp_image;
    std::vector<vector<DMatch > > matches;
    std::vector<DMatch > good_matches;
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;
    std::vector<Point2f> scene_corners(4);
    Mat H;
    Mat image;

    //cvtColor(frame, image, CV_RGB2GRAY);
    image = frame;

    detector.detect( image, kp_image );
    extractor.compute( image, kp_image, des_image );

    matcher.knnMatch(des_object, des_image, matches, 2);

    for(int i = 0; i < min(des_image.rows-1,(int) matches.size()); i++) //THIS LOOP IS SENSITIVE TO SEGFAULTS
    {
        if((matches[i][0].distance < 0.6*(matches[i][1].distance)) && ((int) matches[i].size()<=2 && (int) matches[i].size()>0))
        {
            good_matches.push_back(matches[i][0]);
        }
    }

    //Draw only "good" matches
	drawMatches( train, kp_object, image, kp_image, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    if (good_matches.size() >= 4)
    {
        for( int i = 0; i < good_matches.size(); i++ )
        {
            //Get the keypoints from the good matches
            obj.push_back( kp_object[ good_matches[i].queryIdx ].pt );
            scene.push_back( kp_image[ good_matches[i].trainIdx ].pt );
        }

        H = findHomography( obj, scene, CV_RANSAC );

        perspectiveTransform( obj_corners, scene_corners, H);

        //Draw lines between the corners (the mapped train in the scene image )
        line( img_matches, scene_corners[0] + Point2f( train.cols, 0), scene_corners[1] + Point2f( train.cols, 0), Scalar(0, 255, 0), 4 );
        line( img_matches, scene_corners[1] + Point2f( train.cols, 0), scene_corners[2] + Point2f( train.cols, 0), Scalar( 0, 255, 0), 4 );
        line( img_matches, scene_corners[2] + Point2f( train.cols, 0), scene_corners[3] + Point2f( train.cols, 0), Scalar( 0, 255, 0), 4 );
        line( img_matches, scene_corners[3] + Point2f( train.cols, 0), scene_corners[0] + Point2f( train.cols, 0), Scalar( 0, 255, 0), 4 );
    }



    supLeftOut->x = scene_corners[0].x + train.cols;
	supLeftOut->y = scene_corners[0].y;
	infRightOut->x = scene_corners[2].x + train.cols;
	infRightOut->y = scene_corners[2].y;

	*additionalRowsOut = train.cols;
	*rotationMatrixOut = H.inv();
	return img_matches;
}



Mat cutBorderData(cv::Mat & matImage)
{

	int thresh = 100;
	int width = matImage.cols;

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	Canny( matImage, canny_output, thresh, thresh*2, 3 );
	/// Find contours
	findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	/// Draw contours
	for( int i = 0; i< contours.size(); i++ )
	{
		int height = abs(contours[i][0].y - contours[i][2].y);
		if(height >= matImage.rows * 0.8)
		{
			if(contours[i][0].x > matImage.cols * 0.85)
			{
				width = contours[i][0].x;
			}
		}
	}

	Mat res = matImage(Rect(0,0,width,matImage.rows));
	return res;
}




Mat processImage(const Mat& inputImage, bool numeros = false, bool data = false)
{
	Mat result;
	Mat grayLevel;

    if(inputImage.channels() == 3)
    {
		cvtColor( inputImage, grayLevel, CV_BGR2GRAY );
	}
	else
	{
		grayLevel = inputImage.clone();
	}
	//else
	{



		float desiredWidth = 1000;

		if(data)
		{
			desiredWidth = 700;
		}

		float widthProportion = (float)desiredWidth/grayLevel.size().width;

		cv::resize(grayLevel, grayLevel, Size(grayLevel.size().width*widthProportion,grayLevel.size().height*widthProportion));


		Mat grayLevelA;
		Mat resultA;




		//unsharpMask(grayLevel);

		//grayLevelA = grayLevel;


		cv::adaptiveBilateralFilter(grayLevel,grayLevelA,cv::Size(3, 3),3);
		//cv::bilateralFilter(grayLevel,grayLevelA,cv::Size(3, 3),3);
		cv::addWeighted(grayLevel, 1.5, grayLevelA, -0.5, 0, grayLevelA);

		//cv::namedWindow( "LINESA", CV_WINDOW_NORMAL );
		//cv::imshow( "LINESA", grayLevelA );




		Mat resultC;

		if(data)
		{
			BhThresholder as;

			resultA = Mat(grayLevelA.size(),grayLevelA.type());

			as.doThreshold(grayLevelA,resultA,WOLFJOLION);

			resultC = resultA;

		}
		else
		{
			BhThresholder as;

			resultA = Mat(grayLevelA.size(),grayLevelA.type());

			int N = 250;

			// Split original image in N images to threshold adaptatively
			for (int r = 0; r < grayLevelA.rows; r += N)
			{
				for (int c = 0; c < grayLevelA.cols; c += N)
				{
					cv::Mat tile = grayLevelA(cv::Range(r, min(r + N, grayLevelA.rows)),
								 cv::Range(c, min(c + N, grayLevelA.cols)));//no data copying here
					//cv::Mat tileCopy = img(cv::Range(r, min(r + N, img.rows)),
								 //cv::Range(c, min(c + N, img.cols))).clone();//with data copying

					//tile can be smaller than NxN if image size is not a factor of N

					cv::Mat resultTile = resultA(cv::Range(r, min(r + N, resultA.rows)),
						cv::Range(c, min(c + N, resultA.cols)));

					//your_function_processTile(tile);
					as.doThreshold(tile,resultTile,WOLFJOLION);

				}
			}

			// Cortar pelas linhas tracejadas inferiores


			Mat grayLevel1 = resultA;


			Mat linesR(grayLevel1.size(),grayLevel1.type());
			vector<Vec4i> lines;
			HoughLinesP( grayLevel1, lines, 1, CV_PI/180, 150, 200, 15 );
			int yMax = 0;
			for( size_t i = 0; i < lines.size(); i++ )
			{

				float rho = lines[i][0], theta = lines[i][1];

				//if( theta>CV_PI/180*80 || theta<CV_PI/180*100)
				if(abs(lines[i][1] - lines[i][3]) <= 20)
				{
					line( linesR, Point(lines[i][0], lines[i][1]),
						Point(lines[i][2], lines[i][3]), Scalar(255,255,255), 3, 8 );


					if(lines[i][1] > yMax)
					{
						yMax = lines[i][1];
					}
					if(lines[i][3] > yMax)
					{
						yMax = lines[i][3];
					}
				}

			}

			//cv::namedWindow( "LINES", CV_WINDOW_NORMAL );
			//cv::imshow( "LINES", linesR );

			Mat resultB = grayLevelA(Rect(0,0,grayLevelA.size().width,yMax));

			resultC = Mat(resultB.size(),resultB.type());

			N = 250;


			// Split original image in N images to threshold adaptatively
			for (int r = 0; r < resultB.rows; r += N)
			{
				for (int c = 0; c < resultB.cols; c += N)
				{
					cv::Mat tile = resultB(cv::Range(r, min(r + N, resultB.rows)),
								 cv::Range(c, min(c + N, resultB.cols)));//no data copying here
					//cv::Mat tileCopy = img(cv::Range(r, min(r + N, img.rows)),
								 //cv::Range(c, min(c + N, img.cols))).clone();//with data copying

					//tile can be smaller than NxN if image size is not a factor of N

					cv::Mat resultTile = resultC(cv::Range(r, min(r + N, resultC.rows)),
						cv::Range(c, min(c + N, resultC.cols)));

					//your_function_processTile(tile);
					as.doThreshold(tile,resultTile,WOLFJOLION);

				}
			}


		}



		int kernel_size = resultC.size().width/250;
		if(data)
		{
			kernel_size = 8;
		}

		//int kernel_size = 4;
		//std::cout << "kernelsize = " << kernel_size << std::endl;
		cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(kernel_size, kernel_size));
		//cv::Mat element(kernel_size,kernel_size,CV_8UC1);


		morphologyEx( resultA, resultA, cv::MORPH_CLOSE, element );
		morphologyEx( resultA, resultA, cv::MORPH_CLOSE, element );
		//morphologyEx( resultA, resultA, cv::MORPH_OPEN, element );
		//morphologyEx( result, result, cv::MORPH_CLOSE, element );


		//morphologyEx( result, result, cv::MORPH_DILATE, element );
		//morphologyEx( result, result, cv::MORPH_ERODE, element );



		result = resultC;



	}


	threshold(result,result,20,255,0);

	return result;

}



cv::Mat findBiggestBlob(cv::Mat & matImage){
    int largest_area=0;
    int largest_contour_index=0;

    vector< vector<Point> > contours; // Vector for storing contour
    vector<Vec4i> hierarchy;

    findContours( matImage, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image

    for( int i = 0; i< contours.size(); i++ ) {// iterate through each contour.
        double a=contourArea( contours[i],false);  //  Find the area of contour
        if(a>largest_area){
            largest_area=a;
            largest_contour_index=i;                //Store the index of largest contour
            //bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
        }
    }

    drawContours( matImage, contours, largest_contour_index, Scalar(255), CV_FILLED, 8, hierarchy ); // Draw the largest contour using previously stored index.
    return matImage;
}

vector<Rect> findCharsRects(cv::Mat & matImage, int *meanHeight = NULL)
{
	int min_area = matImage.size().width/30 * matImage.size().height/30;
    int largest_contour_index=0;

	vector<Rect> charsBounds;			// Vector for storing Rects
    vector< vector<Point> > contours; // Vector for storing contour
    vector<Vec4i> hierarchy;

    findContours( matImage, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image

	int sum = 0;

    for( int i = 0; i< contours.size(); i++ )
	{// iterate through each contour.

		double a=contourArea( contours[i],false);  //  Find the area of contour
		if(a > min_area)
		{
			Rect toAdd = boundingRect(contours[i]);
			charsBounds.push_back(toAdd);
			sum += toAdd.height;
		}
    }

	if(meanHeight)
	{
		int mediaHeight = sum/charsBounds.size();
		*meanHeight = mediaHeight;
	}

	//namedWindow("mat");
	//cv::resize(recortResult, recortResult, Size(recortResult.size().width/4,recortResult.size().height/4));
	//imshow( "mat", matImage );
    //drawContours( matImage, contours, largest_contour_index, Scalar(255), CV_FILLED, 8, hierarchy ); // Draw the largest contour using previously stored index.
	return charsBounds;
}


bool SortRects(const Rect& d1, const Rect& d2)
{
	return d1.y < d2.y;
}

vector<Rect> findLinesRects(vector<Rect> charsRects, int imgWidth, int imgHeight, int meanHeight)
{

	vector<Rect> linesBounds;			// Vector for storing Rects
    vector<Vec4i> hierarchy;

	//int maxDif = meanHeight;
	//int maxDif = 20;
	for(int i = 0 ; i < charsRects.size() ; i++)
	{
		Rect charRect = charsRects.at(i);
		int maxDif = charRect.height/2;

		bool newLine = true;
		for(int j = 0 ; j < linesBounds.size() ; j++)
		{
			Rect lineRect = linesBounds.at(j);

			// se o ponto superior do Rect que estamos a testar está ligeiramente acima de uma linha já existente
			// adequar a linha em questão para conter este rect
			if(lineRect.y > charRect.y && abs( lineRect.y - charRect.y) <= maxDif)
			{
				lineRect.y = charRect.y;
				newLine = false;
			}

			// se o ponto inferior do Rect que estamos a testar está ligeiramente abaixo de uma linha já existente
			// adequar a linha em questão para conter este rect
			if( (lineRect.y + lineRect.height) <= (charRect.y + charRect.height) && abs( (lineRect.y + lineRect.height) - (charRect.y + charRect.height)) <= maxDif)
			{
				lineRect.height = charRect.y + charRect.height - lineRect.y;
				newLine = false;
			}

			if(lineRect.contains(cv::Point2i(charRect.x,charRect.y)) && lineRect.contains(cv::Point2i(charRect.x + charRect.width,charRect.y+charRect.height)))
			{
				newLine = false;
			}
			else if(charRect.contains(cv::Point2i(lineRect.x,lineRect.y)) && charRect.contains(cv::Point2i(lineRect.x + lineRect.width,lineRect.y+lineRect.height)))
			{
				// Nao altero e crio outro novo só para testar
				newLine = true;
			}
		}

		if(newLine)
		{
			int yPos = std::max( 0 , (int)(charRect.y * 0.99) );
			//int rectHeight = min(imgHeight - yPos,max(charRect.height*1.05,meanHeight));
			int rectHeight = std::min( imgHeight - yPos , (int)(charRect.height * 1.01));

			Rect toAdd = Rect(0, yPos, imgWidth, rectHeight);

			//Rect toAdd = Rect(0,charRect.y , imgWidth, charRect.height);
			linesBounds.push_back(toAdd);
		}

    }
	// Sort the vector using SortRects and std::sort
	std::sort(linesBounds.begin(), linesBounds.end(), SortRects);
	return linesBounds;
}


string processText(char* inputText)
{
	//std::cout << "processText" << std::endl;

	//std::cout << inputText << std::endl;

	string res("FAIL");
	char * line = strtok (inputText,"\n");

	while (line != NULL)
	{
		string lineS(line);

		if(lineS.length() >= 2)
		{
			std::size_t foundS = lineS.find("Surface");
			std::size_t foundE = lineS.find("Empty");

			//std::cout << "lineS=" << lineS << std::endl;

			if (foundS==std::string::npos && foundE==std::string::npos)
			{
				res = string(lineS);
			}
		}

		line = strtok (NULL, " ");
	}

	return res;
}


string removeChar(string text, char c)
{
	for(std::string::iterator iter = text.begin() ; iter != text.end() ; )
	{
		if( *iter == c )
		{
			iter = text.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	return text;
}


vector<string> processInformation(vector<string> lines)
{
	vector<string> res;
	int maxLineSize = 25;

	bool startedNumbers = false;

	for( int i=0 ; i < lines.size() ; i++ )
	{
		string line = lines.at(i);

		bool firstOfLine = true;

		if(line.length() < maxLineSize)
		{
			string toAdd;
			int wordsInString = 0;

			char* lineC = (char*)line.c_str();

			char * word = strtok (lineC ," ");

			while (word != NULL)
			{
				string wordS(word);

				std::size_t foundN = wordS.find("N");

				if(foundN != std::string::npos)
				{
					wordS.erase(0,foundN+1);
					startedNumbers = true;
				}
				else
				{
					std::size_t foundDot = wordS.find(".");

					if(foundDot != std::string::npos)
					{
						wordS.erase(0,foundDot+1);
						startedNumbers = true;
					}
				}

				wordS = removeChar( wordS , 'E');

				if(firstOfLine && startedNumbers)
				{
					wordS.erase(0,wordS.length()-2);
					firstOfLine = false;
				}

				if(startedNumbers && wordS.length() > 3)
				{

				}
				else
				{
					// Ao apagar conteudo, a palavra resultante pode ficar vazia
					if(wordS.length()>0)
					{
						toAdd = toAdd + wordS + string(" ");
						wordsInString++;
					}
				}
				word = strtok (NULL, " ");

			}

			// se for só uma palavra/numero, provavelmente é lixo
			if(wordsInString > 1)
			{
				res.push_back(toAdd);
			}

		}
	}

	return res;
}


Mat Detect(Mat train, Mat test, cv::Point2f *supLeftOut, cv::Point2f *infRightOut, int *additionalRowsOut, Mat *rotationMatrixOut)
{
	//Detect the keypoints using SURF Detector
    int minHessian = 500;

    SurfFeatureDetector detector( minHessian );
    std::vector<KeyPoint> kp_object;

	detector.detect( train, kp_object );

    //Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;
    Mat des_object;

    //
	extractor.compute( train, kp_object, des_object );

    FlannBasedMatcher matcher;


    std::vector<Point2f> obj_corners(4);

    //Get the corners from the train
    obj_corners[0] = cvPoint(0,0);
    obj_corners[1] = cvPoint( train.cols, 0 );
    obj_corners[2] = cvPoint( train.cols, train.rows );
    obj_corners[3] = cvPoint( 0, train.rows );

    int framecount = 0;
	Mat frame = test.clone();



    Mat des_image, img_matches;
    std::vector<KeyPoint> kp_image;
    std::vector<vector<DMatch > > matches;
    std::vector<DMatch > good_matches;
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;
    std::vector<Point2f> scene_corners(4);
    Mat H;
    Mat image;

    cvtColor(frame, image, CV_RGB2GRAY);

    detector.detect( image, kp_image );
    extractor.compute( image, kp_image, des_image );

    matcher.knnMatch(des_object, des_image, matches, 2);

    for(int i = 0; i < min(des_image.rows-1,(int) matches.size()); i++) //THIS LOOP IS SENSITIVE TO SEGFAULTS
    {
        if((matches[i][0].distance < 0.6*(matches[i][1].distance)) && ((int) matches[i].size()<=2 && (int) matches[i].size()>0))
        {
            good_matches.push_back(matches[i][0]);
        }
    }

    //Draw only "good" matches
	drawMatches( train, kp_object, image, kp_image, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    if (good_matches.size() >= 4)
    {
        for( int i = 0; i < good_matches.size(); i++ )
        {
            //Get the keypoints from the good matches
            obj.push_back( kp_object[ good_matches[i].queryIdx ].pt );
            scene.push_back( kp_image[ good_matches[i].trainIdx ].pt );
        }

        H = findHomography( obj, scene, CV_RANSAC );

        perspectiveTransform( obj_corners, scene_corners, H);

        //Draw lines between the corners (the mapped train in the scene image )
        line( img_matches, scene_corners[0] + Point2f( train.cols, 0), scene_corners[1] + Point2f( train.cols, 0), Scalar(0, 255, 0), 4 );
        line( img_matches, scene_corners[1] + Point2f( train.cols, 0), scene_corners[2] + Point2f( train.cols, 0), Scalar( 0, 255, 0), 4 );
        line( img_matches, scene_corners[2] + Point2f( train.cols, 0), scene_corners[3] + Point2f( train.cols, 0), Scalar( 0, 255, 0), 4 );
        line( img_matches, scene_corners[3] + Point2f( train.cols, 0), scene_corners[0] + Point2f( train.cols, 0), Scalar( 0, 255, 0), 4 );
    }



	Point2f superioresquerdo(scene_corners[0] + Point2f( train.cols, 0));
	Point2f inferiordireito(scene_corners[2] + Point2f( train.cols, 0));
	//printf("%d - %d \n",superioresquerdo.x,superioresquerdo.y);
	//printf("%d - %d \n",inferiordireito.x,inferiordireito.y);
	supLeftOut->x = superioresquerdo.x;
	supLeftOut->y = superioresquerdo.y;
	infRightOut->x = inferiordireito.x;
	infRightOut->y = inferiordireito.y;

	*additionalRowsOut = train.cols;
	*rotationMatrixOut = H.inv();
	return img_matches;
}




int drawSiftKp( cv::Mat image )
{
	cv::Mat rgb = cv::Mat(image.cols, image.rows, CV_8UC3);
	cv::cvtColor(image, rgb, cv::COLOR_RGBA2RGB );

	std::vector< cv::KeyPoint > keypoints;

	cv::SiftFeatureDetector detector;
	detector.detect( rgb, keypoints );

	cv::drawKeypoints( rgb, keypoints, rgb, cv::Scalar(255,0,0,255),
			cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

	cv::cvtColor(rgb, image, cv::COLOR_RGB2RGBA);

	return 0;
}





JNIEXPORT void JNICALL Java_com_example_ndktest_NonfreeJNILib_nativeDrawSiftKp(JNIEnv* env, jobject obj, jlong mat)
{
	//cv::cvtColor(*((cv::Mat*)mat), *((cv::Mat*)mat), cv::COLOR_RGB2GRAY);
	cv::Mat res = *((cv::Mat*)mat);

	float widthProportion = (float)1000/res.size().width;

	cv::resize(res, res, Size(res.size().width*widthProportion,res.size().height*widthProportion));

	drawSiftKp( res );

	*((cv::Mat*)mat) = res;
	//return (jobject)res;
}
/*
JNIEXPORT void JNICALL Java_com_example_ndktest_NonfreeJNILib_nativeDrawSiftKp(JNIEnv* env, jobject obj, jlong mat)
{
	//cv::cvtColor(*((cv::Mat*)mat), *((cv::Mat*)mat), cv::COLOR_RGB2GRAY);
	cv::Mat* res = ((cv::Mat*)mat);
	cv::Mat res = *((cv::Mat*)mat);

	float widthProportion = (float)1000/res->size().width;

	cv::resize(res, res, Size(res->size().width*widthProportion,res->size().height*widthProportion));

	drawSiftKp( res );

	//return (jobject)res;
}
*/

