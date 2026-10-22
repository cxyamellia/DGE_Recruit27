#ifndef MYARMORDETECT_HPP
#define MYARMORDETECT_HPP
#include <opencv2/opencv.hpp>
#include <vector>

struct LightBlob
{
    cv::RotatedRect rect;
};

class myArmorDetect
{
public:
    std::vector<std::vector<cv::Point>> detect(cv::Mat &src);

private:
    std::vector<LightBlob> getLightBlobs(const cv::Mat &src);
    bool checkPair(const LightBlob &a, const LightBlob &b);
};

#endif
