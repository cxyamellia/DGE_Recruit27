#include <opencv2/opencv.hpp>
#include <iostream>

class myColorCorrect
{
public:
    myColorCorrect() = default;
    ~myColorCorrect() = default;
    void correct(const cv::Mat &src, cv::Mat &dst);

private:
    cv::Scalar mean_bgr_;
};

void myColorCorrect::correct(const cv::Mat &src, cv::Mat &dst)
{
    if(src.empty())
    {
        std::cerr << "输入图像为空！" << std::endl;
        return;
    }
    cv::Mat img;
    src.copyTo(img);
    std::vector<cv::Mat> channels;
    cv::split(img, channels);
    cv::Mat &b = channels[0];
    cv::Mat &g = channels[1];
    cv::Mat &r = channels[2];

    cv::Scalar mean_b = cv::mean(b);
    cv::Scalar mean_g = cv::mean(g);
    cv::Scalar mean_r = cv::mean(r);

    double mb = mean_b[0];
    double mg = mean_g[0];
    double mr = mean_r[0];

    double gray_avg = (mb + mg + mr) / 3.0;

    double kb = gray_avg / mb;
    double kg = gray_avg / mg;
    double kr = gray_avg / mr;

    b = b * kb;
    g = g * kg;
    r = r * kr;

    cv::merge(channels, dst);
}


int main()
{
    myColorCorrect corrector;

    cv::Mat src = cv::imread("test.jpg");
    if(src.empty())
    {
        std::cout << "读取图片失败，切换为摄像头模式" << std::endl;
        cv::VideoCapture cap(0);
        if(!cap.isOpened())
        {
            std::cerr << "摄像头打开失败\n";
            return -1;
        }
        cv::Mat frame, result;
        while(cap.read(frame))
        {
            corrector.correct(frame, result);
            cv::imshow("原始画面", frame);
            cv::imshow("灰度世界校正后", result);
            if(cv::waitKey(1) == 27) break;
        }
        cap.release();
    }
    else
    {
        cv::Mat dst;
        corrector.correct(src, dst);
        cv::imshow("原图", src);
        cv::imshow("色彩校正结果", dst);
        cv::waitKey(0);
    }

    cv::destroyAllWindows();
    return 0;
}
