#include <string.h>
#include <jni.h>

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>

using namespace cv;
using namespace std;

// JNI interface functions, be careful about the naming.
extern "C"
{
    JNIEXPORT void JNICALL Java_com_example_ndktest_MainActivity_nativeDrawSiftKp( JNIEnv*, jclass, jlong );
}
