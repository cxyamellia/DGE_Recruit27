#ifndef MYYOLODETECT_HPP
#define MYYOLODETECT_HPP
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <vector>

struct YoloResult
{
    cv::Rect box;
    int class_id;
    float conf;
};

class myYoloDetect
{
public:
    myYoloDetect(const std::string& onnx_path, const std::string& class_txt);
    std::vector<YoloResult> detect(const cv::Mat& src);

private:
    cv::dnn::Net net_;
    std::vector<std::string> class_names_;
    float conf_threshold_;
    float nms_threshold_;
};

#endif
