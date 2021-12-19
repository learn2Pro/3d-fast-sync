#include <iostream>
#include <vector>
#include "opencv2/core/utility.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "json.hpp"

using namespace std;
using namespace cv;
using json = nlohmann::json;

int main()
{
    // create object from string literal
    auto j = "{ \"happy\": true, \"pi\": 3.141 }"_json;
    std::cout << j.dump() << '\n';
    json received = R"({"rgb":[0,0,255,0,0,255,0,0,255,0,0,255],"depth":[],"time":"0"})"_json;
    auto rgb_array = received["rgb"].get<std::vector<int>>();
    // std::cout << rgb_array << '\n';

    // json jj = R"(
    // {
    //     "my_list" : [1,2,3]
    // }
    // )"_json;

    // auto v = jj["my_list"].get<std::vector<int>>();

    // for (auto i : v)
    // {
    //     std::cout << i << std::endl;
    // }

    // Mat I = Mat::eye(4, 4, CV_64F);
    // I.at<double>(1, 1) = CV_PI;
    // for (auto &number : rgb_array)
    // {
    //     vec.push_back(number);
    //     std::cout << number << " ";
    // }
    for (auto number : rgb_array)
    {
        std::cout << number << " ";
    }
    Mat M(1, rgb_array.size(), CV_8UC1, rgb_array.data());
    Mat reshaped = M.reshape(2, 2);
    cout
        << "reshaped = \n"
        << reshaped << ";" << endl
        << endl;
    std::string greyImgFromVecWindow = "123";
    cv::namedWindow(greyImgFromVecWindow);
    cv::imshow(greyImgFromVecWindow, M);
    cv::waitKey(0);
}