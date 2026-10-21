#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

class myCamera
{
public:
    myCamera();
    ~myCamera();
    bool read(cv::Mat &frame);
private:
    cv::VideoCapture cap_;
    bool is_open_;
};

myCamera::myCamera()
{
#ifdef _WIN32
    cap_.open(0, cv::CAP_DSHOW);
#else
    cap_.open(0);
#endif
    is_open_ = cap_.isOpened();
    if(!is_open_) std::cerr<<"摄像头打开失败\n";
}

myCamera::~myCamera()
{
    if(is_open_) cap_.release();
}

bool myCamera::read(cv::Mat &frame)
{
    if(!is_open_) return false;
    return cap_.read(frame);
}


class myArmorDetect
{
public:
    myArmorDetect();
    ~myArmorDetect();
    void detect(const cv::Mat &src, cv::Mat &draw_out);

private:
    cv::Mat binary_;
    std::vector<std::vector<cv::Point>> contours_;
    std::vector<cv::Vec4i> hierarchy_;
    double light_min_area_;
    double light_max_ratio_;
};

myArmorDetect::myArmorDetect()
{
    light_min_area_ = 20;
    light_max_ratio_ = 8.0;
}

myArmorDetect::~myArmorDetect(){}

void myArmorDetect::detect(const cv::Mat &src, cv::Mat &draw_out)
{
    draw_out = src.clone();
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, binary_, 180, 255, cv::THRESH_BINARY);

    contours_.clear();
    hierarchy_.clear();
    cv::findContours(binary_, contours_, hierarchy_, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::RotatedRect> lights;
    for(auto &cnt : contours_)
    {
        double area = cv::contourArea(cnt);
        if(area < light_min_area_) continue;
        cv::RotatedRect r = cv::minAreaRect(cnt);
        float w = r.size.width;
        float h = r.size.height;
        if(h <= 0 || w <=0) continue;
        double ratio = std::max(w,h)/std::min(w,h);
        if(ratio > light_max_ratio_) continue;
        lights.push_back(r);
    }

    for(int i=0;i<(int)lights.size();i++)
    {
        for(int j=i+1;j<(int)lights.size();j++)
        {
            cv::Point2f p1 = lights[i].center;
            cv::Point2f p2 = lights[j].center;
            double dist = cv::norm(p1-p2);
            if(dist <20 || dist>300) continue;

            std::vector<cv::Point2f> pts;
            cv::Point2f buf[4];
            lights[i].points(buf);
            pts.push_back(buf[0]);
            pts.push_back(buf[1]);
            lights[j].points(buf);
            pts.push_back(buf[2]);
            pts.push_back(buf[3]);

            std::vector<cv::Point> draw_pts;
            for(auto &pt:pts) draw_pts.emplace_back((cv::Point)pt);
            cv::polylines(draw_out, draw_pts, true, cv::Scalar(0,0,255),2);
        }
    }
}


int main()
{
    myCamera cam;
    myArmorDetect detector;
    cv::Mat frame;

    while(cam.read(frame))
    {
        cv::Mat out;
        detector.detect(frame, out);
        cv::imshow("Armor Detect", out);
        if(cv::waitKey(1) == 27) break;
    }
    cv::destroyAllWindows();
    return 0;
}
