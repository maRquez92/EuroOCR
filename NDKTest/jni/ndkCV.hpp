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



#include "tesseract/baseapi.h"


using namespace cv;
using namespace std;
using namespace tesseract;

#define  LOG_TAG    "NDKTEST"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

string dataMarker("INIT_DATES");
string numbersMarker("INIT_NUMBERS");

Mat Detect(Mat train, Mat test, Point2i *supLeftOut, Point2i *infRightOut, int *additionalRowsOut, Mat *rotationMatrixOut);
Mat processImage(const Mat& inputImage, bool numeros, bool data);
Mat cutBorderData(cv::Mat & matImage);
char* getText(string tessdataPath, Mat image_with_text, bool data);
string processText(char* inputText);
vector<Rect> findLinesRects(vector<Rect> charsRects, int imgWidth, int imgHeight);
vector<Rect> findCharsRects(cv::Mat matImage);
vector<string> processInformation(vector<string> lines);


// JNI interface functions, be careful about the naming.
#ifdef __cplusplus
extern "C"
{
#endif

	JNIEXPORT jobjectArray JNICALL Java_com_example_emocr_NonfreeJNILib_maine( JNIEnv* env, jobject obj, jlong talao, jlong logo, jlong image, jstring tessdataPath, jlong res);

#ifdef __cplusplus
}
#endif

