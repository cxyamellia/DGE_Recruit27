#include "myYoloDetect.hpp"
#include <fstream>

myYoloDetect::myYoloDetect(const std::string &onnx_path, const std::string &class_txt)
{
    conf_threshold_ = 0.25f;
    nms_threshold_ = 0.45f;
    net_ = cv::dnn::readNetFromONNX(onnx_path);
    net_.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net_.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    std::ifstream ifs(class_txt);
    std::string line;
    while(std::getline(ifs, line))
    {
        class_names_.push_back(line);
    }
}

std::vector<YoloResult> myYoloDetect::detect(const cv::Mat &src)
{
    std::vector<YoloResult> output;
    cv::Mat blob;
    cv::dnn::blobFromImage(src, blob, 1.0/255.0, cv::Size(640,640), cv::Scalar(), true);
    net_.setInput(blob);
    std::vector<cv::Mat> outs;
    net_.forward(outs, net_.getUnconnectedOutLayersNames());

    std::vector<int> classIds;
    std::vector<float> confs;
    std::vector<cv::Rect> boxes;

    float x_factor = (float)src.cols / 640.0f;
    float y_factor = (float)src.rows / 640.0f;

    float *data = (float*)outs[0].data;
    const int rows = 8400;
    for(int i = 0; i < rows; i++)
    {
        float conf = data[4];
        if(conf < conf_threshold_)
        {
            data += 84;
            continue;
        }
        float cx = data[0];
        float cy = data[1];
        float w = data[2];
        float h = data[3];

        int x = (cx - w/2) * x_factor;
        int y = (cy - h/2) * y_factor;
        int width = w * x_factor;
        int height = h * y_factor;

        int cid = cv::max_element(data+5, data+84) - (data+5);
        classIds.push_back(cid);
        confs.push_back(conf);
        boxes.emplace_back(x,y,width,height);
        data += 84;
    }

    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confs, conf_threshold_, nms_threshold_, indices);
    for(auto idx : indices)
    {
        YoloResult res;
        res.box = boxes[idx];
        res.class_id = classIds[idx];
        res.conf = confs[idx];
        output.push_back(res);
    }
    return output;
}
