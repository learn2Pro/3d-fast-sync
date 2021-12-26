#include <iostream>
#include <chrono>
#include <curl/curl.h>
#include "http/chunk.hpp"
#include "schedule/Schduler.h"
#include "json/JsonDataParser.hpp"
#include "opencv2/core/utility.hpp"
#include <map>
#include <iterator>
#include <queue>
#include <deque>

#define CPPHTTPLIB_OPENSSL_SUPPORT

using namespace cv;
using json = nlohmann::json;

unsigned int running_threads_num = 32;
unsigned int iterval_in_millis = 50;
unsigned int running_time_in_minutes = 10;
unsigned int cli_conn_time_out_in_ms = 300000;
std::string host = "192.168.1.65";
// std::string host = "127.0.0.1";
std::string port = "15000";
std::string request_url = "http:/" + host + ":" + port + "/recent/image";
std::string rgb_window_name = "rgb";
std::string depth_window_name = "depth";
std::map<long, std::pair<cv::Mat, cv::Mat>> img_buffer_sort_of_ts;
std::deque<std::pair<cv::Mat, cv::Mat>> img_queue;
long max_buffer_size = 10;

std::mutex mtx; // mutex insert to buffer
Bosma::Scheduler s(running_threads_num);

void doShow3dImage(const std::string &input)
{
    cv::Mat rgb, depth;
    long ts;
    parser::parse(input, rgb, depth, ts);
    // parser::parseCommaSplit(input, rgb, depth, ts);
    //insert to map buffer
    if (ts != 0)
    {
        cv::imshow(rgb_window_name, rgb);
        cv::imshow(depth_window_name, depth);
        cv::waitKey(1);
    }
}
// request the remote image by url
std::string doRequestImage(const std::string &s)
{
    try
    {
        // auto begin = std::chrono::system_clock::now();
        // auto tt = std::chrono::system_clock::to_time_t(begin);
        CURL *curl;
        CURLcode res;
        std::string readBuffer;
        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, request_url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_NOPROXY, host.c_str());
            curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip,deflate");
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            // auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - begin).count();
            // print the result
            // std::cout << "timstamp:" << std::ctime(&tt) << '\n'
            //           << "cost:" << cost << "ms\n"
            //           << "res:" << res << "\n"
            //           << "response size:" << readBuffer.size() << '\n'
            //           << "request:" << s << '\n';
            return readBuffer;
        }
        else
        {
            return "";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Request failed, error: " << e.what() << '\n';
        return "error response";
    }
}

void doShowJobExecution(const std::string &msg)
{
    //step1. request the image from remote
    std::cout << "<<<<<<<<<<<<<<<start show<<<<<<<<<<<<<<<\n";
    auto begin = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(begin);
    std::string response = doRequestImage(msg);
    //step2. show the image in 3d mode
    if (!response.empty())
    {
        doShow3dImage(response);
        auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - begin).count();
        std::cout << "timstamp:" << std::ctime(&tt) << '\n'
                  << "cost:" << cost << "ms\n"
                  << "response size:" << response.size() << '\n'
                  << "request:" << msg << '\n';
    }
}
void scheduleWithMainThread(int sleep)
{
    cv::namedWindow(rgb_window_name, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(depth_window_name, cv::WINDOW_AUTOSIZE);
    while (true)
    {
        doShowJobExecution(request_url);
    }
    cv::destroyAllWindows();
}
void scheduleWithFramework()
{
    // number of tasks that can run simultaneously
    // Note: not the number of tasks that can be added,
    //       but number of tasks that can be run in parallel
    unsigned int max_n_threads = running_threads_num;
    // Make a new scheduling object.
    // Note: s cannot be moved or copied
    Bosma::Scheduler s(max_n_threads);

    // every second call message("every second")
    s.every(std::chrono::milliseconds(iterval_in_millis), doShowJobExecution, request_url);

    // destructor of Bosma::Scheduler will cancel all schedules but finish any tasks currently running
    std::this_thread::sleep_for(std::chrono::minutes(running_time_in_minutes));
    // std::this_thread::yield();
}
std::pair<int, long> doInsertToBuffer(const std::string &input)
{
    cv::Mat rgb, depth;
    long ts;
    parser::parse(input, rgb, depth, ts);
    //insert to map buffer
    // std::cout << "insert buffer size: " << img_queue.size() << "\n";
    if (ts != 0)
    {
        // std::unique_lock<std::mutex> lck(mtx, std::defer_lock);
        // lck.lock();
        // auto ans = img_buffer_sort_of_ts.insert(std::make_pair(ts, std::make_pair(rgb, depth))).second;
        // lck.unlock();
        img_queue.push_back(std::make_pair(rgb, depth));
        if (img_queue.size() > max_buffer_size)
        {
            img_queue.pop_front();
        }
        return make_pair(0, ts);
    }
    else
    {
        return make_pair(-1, ts);
    }
}

void doInsertJobExecution(const std::string &msg)
{
    //step1. request the image from remote
    // std::cout << "<<<<<<<<<<<<<<<start request<<<<<<<<<<<<<<<\n";
    auto begin = std::chrono::system_clock::now();
    std::string response = doRequestImage(msg);
    // std::cout << "<<<<<<<<<<<<<<<end request<<<<<<<<<<<<<<<\n";
    //step2. show the image in 3d mode
    if (!response.empty())
    {
        // std::cout << "<<<<<<<<<<<<<<<start show<<<<<<<<<<<<<<<\n";
        auto p = doInsertToBuffer(response);
        auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - begin).count();
        if (img_queue.size() > 0)
        {
            std::cout << "insert result: " << p.first
                      << " | with key: " << p.second
                      << " | and size: " << img_queue.size()
                      << " cost: " << cost << "ms\n";
        }
        // std::cout << "<<<<<<<<<<<<<<<end show<<<<<<<<<<<<<<<\n";
    }
}

void showImgFromBufferInMain()
{
    cv::namedWindow(rgb_window_name, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(depth_window_name, cv::WINDOW_AUTOSIZE);
    while (true)
    {
        try
        {
            if (img_queue.size() > 0)
            {
                auto p = img_queue[0];
                cv::imshow(rgb_window_name, p.first);
                cv::imshow(depth_window_name, p.second);
                cv::waitKey(1);
                img_queue.pop_front();
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "image show failed in main thread, error: " << e.what() << '\n';
        }
    }
}
void putImgToBufferInMultiThread()
{
    // every second call message("every second")
    s.every(std::chrono::milliseconds(iterval_in_millis), doInsertJobExecution, request_url);
    // destructor of Bosma::Scheduler will cancel all schedules but finish any tasks currently running
    // std::this_thread::sleep_for(std::chrono::minutes(running_time_in_minutes));
    // std::this_thread::sleep_for(std::chrono::seconds(2));
    // std::this_thread::yield();
}
// the logic for schedule request image in every 1 seconds
int main()
{
    // scheduleWithMainThread(10);
    putImgToBufferInMultiThread();
    showImgFromBufferInMain();
}