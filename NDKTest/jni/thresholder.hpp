#ifndef _THRESHOLDER
#define _THRESHOLDER

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>

using namespace cv;

enum BhThresholdMethod{OTSU,NIBLACK,SAUVOLA,WOLFJOLION};


class BhThresholder
{
public :
    void doThreshold(InputArray src ,OutputArray dst,const BhThresholdMethod &method);
private:
};

#endif //_THRESHOLDER
