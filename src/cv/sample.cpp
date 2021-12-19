#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
int main()
{
    Mat img = cv::imread("./apple.jpg");
    cv::namedWindow("simple demo", cv::WINDOW_AUTOSIZE);
    cv::imshow("simple demo", img);
    cv::waitKey(0);
    return 0;
}