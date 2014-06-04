#include "thresholder.hpp"

#include <string.h>
#include <jni.h>
#include <android/log.h>

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>



#include <cmath>
#include <list>
#include <algorithm>
#include <iterator>
#include <map>
#include <utility>
#include <set>
#include "tesseract/baseapi.h"
//#include <baseapi.h>


using namespace cv;
using namespace std;
using namespace tesseract;

#define  LOG_TAG    "NDKTEST"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


Mat Detect(Mat train, Mat test, Point2i *supLeftOut, Point2i *infRightOut, int *additionalRowsOut, Mat *rotationMatrixOut);
Mat processImage(const Mat& inputImage, bool numeros, bool data);
Mat cutBorderData(cv::Mat & matImage);
char* getText(string tessdataPath, Mat image_with_text, bool data);


// JNI interface functions, be careful about the naming.
#ifdef __cplusplus
extern "C"
{
#endif


	JNIEXPORT jstring JNICALL Java_com_example_ndktest_NonfreeJNILib_maine( JNIEnv* env, jobject obj, jlong talao, jlong logo, jstring photoPath, jstring tessdataPath, jlong res);
	JNIEXPORT void JNICALL Java_com_example_ndktest_NonfreeJNILib_nativeDrawSiftKp( JNIEnv* env, jobject obj, jlong mat);
	//jobjectArray Java_com_pjottersstuff_ocrdemo_OCRProcessor_processImage(
	//		JNIEnv *env, jobject object, jstring filename, jstring datapath);
	//jstring Java_com_example_ndktest_NonfreeJNILib_getJniString( JNIEnv* env, jobject obj);


#ifdef __cplusplus
}
#endif

