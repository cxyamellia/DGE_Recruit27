#include <opencv2/opencv.hpp>
#include <iostream>

class GrayWorldCorrect {
public:
    GrayWorldCorrect() = default;
    ~GrayWorldCorrect() = default;
    void correct(const cv::Mat& src, cv::Mat& dst);
};

void GrayWorldCorrect::correct(const cv::Mat& src, cv::Mat& dst)
{
    if (src.empty()) return;
    std::vector<cv::Mat> channels;
    cv::split(src, channels);

    double sumB = cv::sum(channels[0])[0];
    double sumG = cv::sum(channels[1])[0];
    double sumR = cv::sum(channels[2])[0];

    int pixel_cnt = src.rows * src.cols;
    double meanB = sumB / pixel_cnt;
    double meanG = sumG / pixel_cnt;
    double meanR = sumR / pixel_cnt;

    double gray_mean = (meanB + meanG + meanR) / 3.0;

    // 增益系数
    double kb = gray_mean / meanB;
    double kg = gray_mean / meanG;
    double kr = gray_mean / meanR;

    channels[0] = channels[0] * kb;
    channels[1] = channels[1] * kg;
    channels[2] = channels[2] * kr;

    cv::merge(channels, dst);
    dst.convertTo(dst, CV_8UC3);
}

int main()
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cerr << "【任务2】摄像头打开失败！\n";
        return -1;
    }
    GrayWorldCorrect corrector;
    cv::Mat frame, out;

    while (cap.read(frame))
    {
        corrector.correct(frame, out);
        cv::imshow("Task2 Original", frame);
        cv::imshow("Task2 GrayWorld Corrected", out);
        int key = cv::waitKey(1);
        if (key == 27) break;
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}
