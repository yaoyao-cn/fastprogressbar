#include "fastprogressbar.h"
#include <thread>
#include <chrono>
#include <vector>
using namespace std;

static void test_single_thread()
{
    fastprogressbar bar;
    bar.set_bar_style(0);
    bar.set_niter(100);
    for (int i = 0; i < 100; i++)
    {
        bar.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

static void test_multi_thread()
{
    int nthreads = 10;

    fastprogressbar bar;
    bar.set_niter(1000);
    bar.set_bar_style(1);

    // create threads
    vector<thread> threads;
    for (int i = 0; i < nthreads; i++)
    {
        threads.emplace_back([&bar]() {
            for (int i = 0; i < 100; i++)
            {
                bar.update();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            });
    }

    // join threads
    for (auto& t : threads)
    {
        t.join();
    }
}

int main()
{
    cout << "Test single thread:" << endl;
    test_single_thread();

    cout << endl;

    cout << "Test multi thread:" << endl;
    test_multi_thread();

    return 0;
}