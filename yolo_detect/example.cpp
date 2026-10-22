#include <opencv2/opencv.hpp>
#include <iostream>
#include "myYoloDetect.hpp"

int main()
{
    cv::VideoCapture cap;
#ifdef _WIN32
    cap.open(0);
#else
    cap.open(0);
#endif
    if(!cap.isOpened())
    {
        std::cerr << "camera open failed" << std::endl;
        return -1;
    }

    myYoloDetect detector("./armor.onnx", "./classes.txt");
    cv::Mat frame;
    while(true)
    {
        cap >> frame;
        if(frame.empty()) break;
        auto res_list = detector.detect(frame);
        for(auto& res : res_list)
        {
            cv::rectangle(frame, res.box, cv::Scalar(0,0,255),2);
            std::string info = std::to_string(res.class_id)+" "+std::to_string(res.conf);
            cv::putText(frame, info, res.box.tl(), cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(0,0,255));
        }
        cv::imshow("yolo_armor_detect", frame);
        int key = cv::waitKey(30);
        if(key == 27) break;
    }
    cv::destroyAllWindows();
    cap.release();
    return 0;
}
