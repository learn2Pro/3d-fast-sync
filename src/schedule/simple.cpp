#include "Schduler.h"
#include <iostream>

void message(const std::string &s)
{
    std::cout << s << std::endl;
}

// doc: https://github.com/Bosma/Scheduler
int main()
{
    // number of tasks that can run simultaneously
    // Note: not the number of tasks that can be added,
    //       but number of tasks that can be run in parallel
    unsigned int max_n_threads = 12;
    // Make a new scheduling object.
    // Note: s cannot be moved or copied
    Bosma::Scheduler s(max_n_threads);

    // every second call message("every second")
    s.every(std::chrono::seconds(1), message, "every second");

    // destructor of Bosma::Scheduler will cancel all schedules but finish any tasks currently running
    std::this_thread::sleep_for(std::chrono::minutes(10));
}
