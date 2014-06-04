#ifndef _THRESHOLDER
#define _THRESHOLDER

#include <opencv\cv.h>

using namespace cv;

enum class BhThresholdMethod{OTSU,NIBLACK,SAUVOLA,WOLFJOLION};


class BhThresholder
{
public :
    void doThreshold(InputArray src ,OutputArray dst,const BhThresholdMethod &method);
private:
};

#endif //_THRESHOLDER
