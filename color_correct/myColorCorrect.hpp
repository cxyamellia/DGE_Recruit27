#ifndef MYCOLORCORRECT_HPP
#define MYCOLORCORRECT_HPP
#include <opencv2/opencv.hpp>

class myColorCorrect
{
public:
    cv::Mat run(const cv::Mat &src);
};

#endif
