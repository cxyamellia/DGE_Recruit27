#include "myArmorDetect.hpp"
#include <cmath>

std::vector<LightBlob> myArmorDetect::getLightBlobs(const cv::Mat &src)
{
    std::vector<LightBlob> lights;
    cv::Mat gray, binary;

    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, binary, 120, 255, cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for(auto &cnt : contours)
    {
        if(cv::contourArea(cnt) < 15)
            continue;

        LightBlob blob;
        blob.rect = cv::minAreaRect(cnt);
        lights.push_back(blob);
    }
    return lights;
}

bool myArmorDetect::checkPair(const LightBlob &a, const LightBlob &b)
{
    cv::Point2f c1 = a.rect.center;
    cv::Point2f c2 = b.rect.center;

    float dx = fabs(c1.x - c2.x);
    float dy = fabs(c1.y - c2.y);

    if(dx < dy)
        return false;

    float h1 = a.rect.size.height;
    float h2 = b.rect.size.height;
    if(fabs(h1 - h2) > std::max(h1, h2)*0.45)
        return false;

    return true;
}

std::vector<std::vector<cv::Point>> myArmorDetect::detect(cv::Mat &src)
{
    std::vector<std::vector<cv::Point>> armors;
    auto lights = getLightBlobs(src);

    for(int i=0;i<lights.size();i++)
    {
        for(int j=i+1;j<lights.size();j++)
        {
            if(checkPair(lights[i], lights[j]))
            {
                cv::Point2f p1[4], p2[4];
                lights[i].rect.points(p1);
                lights[j].rect.points(p2);

                std::vector<cv::Point> armor;
                armor.emplace_back(p1[0]);
                armor.emplace_back(p1[1]);
                armor.emplace_back(p2[3]);
                armor.emplace_back(p2[2]);
                armors.push_back(armor);
            }
        }
    }
    return armors;
}
