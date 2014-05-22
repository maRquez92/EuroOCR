#include "ndkCV.hpp"
#include "thresholder.hpp"

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
		grayLevel = inputImage;
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

		// Used but not found in Android
		//cv::adaptiveBilateralFilter(grayLevel,grayLevelA,cv::Size(3, 3),3);


		//cv::GaussianBlur(grayLevel, grayLevelA, cv::Size(0, 0), 3);
		//cv::bilateralFilter(grayLevel,grayLevelA,cv::Size(3, 3),3);
		//cv::medianBlur(grayLevel, grayLevelA, 21);
		//cv::addWeighted(grayLevel, 1.5, grayLevelA, -0.5, 0, grayLevelA);
		cv::addWeighted(grayLevel, 1.5, grayLevelA, -0.5, 0, grayLevelA);
		//medianBlur(grayLevel, grayLevel, 9);

		/*
		cv::namedWindow( "gray", CV_WINDOW_NORMAL );
		cv::imshow( "gray", grayLevel );
		cv::namedWindow( "sharp", CV_WINDOW_NORMAL );
		cv::imshow( "sharp", grayLevelA );
		*/


		if(data)
		{
			BhThresholder as;

			resultA = Mat(grayLevelA.size(),grayLevelA.type());

			as.doThreshold(grayLevelA,resultA,WOLFJOLION);



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
		}



		int kernel_size = resultA.size().width/250;
		if(data)
		{
			kernel_size = 8;
		}

		//int kernel_size = 4;
		cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(kernel_size, kernel_size));
		//cv::Mat element(kernel_size,kernel_size,CV_8UC1);


		morphologyEx( resultA, resultA, cv::MORPH_CLOSE, element );
		morphologyEx( resultA, resultA, cv::MORPH_CLOSE, element );
		//morphologyEx( resultA, resultA, cv::MORPH_OPEN, element );
		//morphologyEx( result, result, cv::MORPH_CLOSE, element );


		//morphologyEx( result, result, cv::MORPH_DILATE, element );
		//morphologyEx( result, result, cv::MORPH_ERODE, element );



		if(numeros)
		{
			// Cortar pelas linhas tracejadas inferiores

			Mat linesR(resultA.size(),resultA.type());
			vector<Vec4i> lines;
			HoughLinesP( resultA, lines, 1, CV_PI/180, 150, 200, 15 );
			int yMax = 0;
			for( size_t i = 0; i < lines.size(); i++ )
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


			//printf("yMax=%d\n",yMax);
			//result = resultA;
			result = resultA(Rect(0,0,resultA.size().width,yMax));

		}
		else
		{
			result = resultA;
		}


	}


	threshold(result,result,20,255,0);

	return result;

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

JNIEXPORT void JNICALL Java_com_example_ndktest_MainActivity_nativeDrawSiftKp(JNIEnv * env, jclass claz, jlong mat)
{
	drawSiftKp( *((cv::Mat*)mat) );
}
