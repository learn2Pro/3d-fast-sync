#include <iostream>
#include <vector>
#include "opencv2/core/utility.hpp"
#include <opencv2/highgui.hpp>
#include "json.hpp"
#include "base64.hpp"
#include <sstream>
#include <iostream>
#ifndef IMAGE_PARSER_DEFINE_HPP_
#define IMAGE_PARSER_DEFINE_HPP_

using namespace std;
using namespace cv;
using json = nlohmann::json;

namespace parser
{
    inline void parse(const std::string &input, cv::Mat &rgb, cv::Mat &depth, long &ts)
    {
        try
        {
            // json received = R"({"rgb":[0,0,255,0,0,255,0,0,255,0,0,255],"depth":"","time":"0"})"_json;
            json received = json::parse(input);
            auto jpg_binary_base64 = received["rgb"].get<std::string>();
            auto depth_binary_base64 = received["depth"].get<std::string>();
            ts = received["time"].get<long>();
            if (ts == 0)
            {
                return;
            }
            // Mat M(1, rgb_array.size(), CV_8UC1, rgb_array.data());
            // M.copyTo(rgb);
            rgb = Base2Mat(jpg_binary_base64);
            depth = Base2Mat(depth_binary_base64);
            // cout << "rgb base64 -> " << jpg_binary_base64.size() << '\n'
            //      << "depth base64 -> " << depth_binary_base64.size() << '\n'
            //      << "ts -> " << ts << '\n';
        }
        catch (const std::exception &e)
        {
            std::cerr << "Request failed, error: " << e.what() << '\n';
        }
    }

    inline void parseCommaSplit(const std::string &input, cv::Mat &rgb, cv::Mat &depth, long &ts)
    {
        try
        {
            // json received = R"({"rgb":[0,0,255,0,0,255,0,0,255,0,0,255],"depth":"","time":"0"})"_json;
            // json received = json::parse(input);
            istringstream f(input);
            std::string part;
            getline(f, part, ';');
            rgb = Base2Mat(part);
            getline(f, part, ';');
            depth = Base2Mat(part);
            getline(f, part, ';');
            ts = std::stol(part);
            if (ts == 0)
            {
                return;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Request failed, error: " << e.what() << '\n';
        }
    }
}

#endif