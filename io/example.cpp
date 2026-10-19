#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
class myCamera
{
 private:
     VideoCaputure cap_;
     int cameraIndex_;
     bool isOpen_;
 public:
     myCerma();
     ~myCamera();
     bool read(Mat& frame);
};
myCamera::myCamera()
{
    cameraIndex_=0;
    isOpen_=false;
    cap_.open(cameraIndex_);
    if(cap_.isOpend())
    {
        isOpen_=true;
        cout<<"摄像头打开成功"<<endl;
    }
    else
    {
        cout<<"摄像头打开失败"<<endl;
    }
}
bool myCamera::read(Mat& frame)
{
    if(!isOpen_)
        return false;
    cap_>> frame;
    return !frame.empty();
}
myCamera::~myCamera()
{
    if(cap_.isOpend())
    {
        cap_.release();
    }
}
int main()
{
    myCamera cam;
    Mat frame;
    while(true)
    {
        if(cam.read(frame))
        {
            imshow("camera", frame);
        }
        if(waitKey(1)=='q')
            break;
    }
    destroyALLWindows();
    return 0;
}
