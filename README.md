# FastProgressBar

A simple, thread-safe, and extremely fast progress bar inspired by [gipert/progressbar](https://github.com/gipert/progressbar).

![progressbar](./progressbar.gif)

## Features

- Header-only
- Thread-safe (almost lock-free)
- Minimal overhead
- Easy to customize

## BuildExample

```cmake
mkdir build
cd build
cmake ..
```

## Usage

### single thread example:
```cpp
int main()
{
    fastprogressbar bar;
    bar.set_bar_style(0);
    bar.set_niter(100);
    for (int i = 0; i < 100; i++)
    {
        bar.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
```

### multi-thread example:
```cpp
int main()
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

    return 0;
}
```