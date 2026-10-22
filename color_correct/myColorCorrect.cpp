#include "myColorCorrect.hpp"

cv::Mat myColorCorrect::run(const cv::Mat &src)
{
    if (src.empty())
        return cv::Mat();

    cv::Mat img;
    src.convertTo(img, CV_32FC3);

    std::vector<cv::Mat> bgr;
    cv::split(img, bgr);

    cv::Scalar mean_val = cv::mean(src);
    float meanB = mean_val[0];
    float meanG = mean_val[1];
    float meanR = mean_val[2];

    float gray_mean = (meanB + meanG + meanR) / 3.0f;

    bgr[0] = bgr[0] * gray_mean / meanB;
    bgr[1] = bgr[1] * gray_mean / meanG;
    bgr[2] = bgr[2] * gray_mean / meanR;

    cv::Mat dst;
    cv::merge(bgr, dst);
    dst.convertTo(dst, CV_8UC3);

    return dst;
}
