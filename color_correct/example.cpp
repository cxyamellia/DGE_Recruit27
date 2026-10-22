#include <opencv2/opencv.hpp>
#include <iostream>
#include "myColorCorrect.hpp"

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

    myColorCorrect corrector;
    cv::Mat frame, result;

    while (true)
    {
        cap >> frame;
        if (frame.empty()) break;

        result = corrector.run(frame);

        cv::imshow("raw_image", frame);
        cv::imshow("color_corrected", result);

        int key = cv::waitKey(30);
        if(key == 27) break;
    }

    cv::destroyAllWindows();
    cap.release();
    return 0;
}
