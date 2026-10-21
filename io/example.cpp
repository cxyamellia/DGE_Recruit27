#include <opencv2/opencv.hpp>
#include <iostream>

class myCamera
{
public:
    // 公有成员：仅构造、析构、read，符合题目硬性要求
    myCamera();
    ~myCamera();
    bool read(cv::Mat &frame);

private:
    // 私有变量全部以下划线 _ 结尾
    cv::VideoCapture cap_;
    bool is_open_;
};

// 构造函数：自适应 Windows / Ubuntu 打开摄像头
myCamera::myCamera()
{
#ifdef _WIN32
    // Windows平台摄像头
    cap_.open(0, cv::CAP_DSHOW);
#else
    // Ubuntu Linux平台摄像头
    cap_.open(0);
#endif
    if (cap_.isOpened())
    {
        is_open_ = true;
        std::cout << "摄像头打开成功" << std::endl;
    }
    else
    {
        is_open_ = false;
        std::cerr << "摄像头打开失败！" << std::endl;
    }
}

// 析构函数：释放摄像头资源
myCamera::~myCamera()
{
    if (is_open_)
    {
        cap_.release();
    }
}

// read函数：读取一帧图像，成功返回true，失败返回false
bool myCamera::read(cv::Mat &frame)
{
    if (!is_open_)
    {
        return false;
    }
    return cap_.read(frame);
}


int main()
{
    myCamera cam;
    cv::Mat frame;

    while (cam.read(frame))
    {
        cv::imshow("Camera View", frame);
        // ESC键 27 退出程序
        if (cv::waitKey(1) == 27)
        {
            break;
        }
    }

    cv::destroyAllWindows();
    return 0;
}
