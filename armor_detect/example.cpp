#include <opencv2/opencv.hpp>
#include <iostream>
#include "myArmorDetect.hpp"

int main()
{
    cv::VideoCapture cap;
#ifdef _WIN32
    cap.open(0);
#else
    cap.open(0);
#endif
    if (!cap.isOpened())
    {
        std::cerr << "camera open failed!" << std::endl;
        return -1;
    }

    myArmorDetect detector;
    cv::Mat frame;

    while(true)
    {
        cap >> frame;
        if(frame.empty()) break;

        auto armor_list = detector.detect(frame);

        for(auto &armor : armor_list)
        {
            cv::polylines(frame, armor, true, cv::Scalar(0,255,0),2);
        }

        cv::imshow("armor_detect", frame);
        int key = cv::waitKey(30);
        if(key == 27) break;
    }

    cv::destroyAllWindows();
    cap.release();
    return 0;
}
