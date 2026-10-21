#include <opencv2/opencv.hpp>
#include <iostream>

class MyCamera
{
public:
    MyCamera();
    ~MyCamera();
    bool read(cv::Mat &frame);

private:
    cv::VideoCapture cap_;
    bool is_open_;
};

MyCamera::MyCamera()
{
#ifdef _WIN32
    // Windows 平台使用 DSHOW
    cap_.open(0, cv::CAP_DSHOW);
#else
    // Linux / Ubuntu 平台
    cap_.open(0);
#endif
    is_open_ = cap_.isOpened();
    if (!is_open_)
    {
        std::cerr << "摄像头打开失败！" << std::endl;
    }
}

MyCamera::~MyCamera()
{
    if (is_open_)
    {
        cap_.release();
    }
}

bool MyCamera::read(cv::Mat &frame)
{
    if (!is_open_)
        return false;
    return cap_.read(frame);
}

int main()
{
    MyCamera cam;
    cv::Mat frame;

    while (cam.read(frame))
    {
        cv::imshow("Task1 Cross Platform Camera", frame);
        int key = cv::waitKey(1);
        if (key == 27) // ESC 退出
        {
            break;
        }
    }
    cv::destroyAllWindows();
    return 0;
}
