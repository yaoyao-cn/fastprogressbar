/*
 * A simple progress bar for console application
 *
 * inspired by https://github.com/gipert/progressbar
 *
 * improvements:
 *  1. thread safe (almost lock-free)
 *  2. extremely fast with minimal overhead
 */

#pragma once
#include <iostream>
#include <sstream>
#include <atomic>
#include <mutex>

class fastprogressbar
{
    static constexpr const char* s_bar_style0[51]{
        "[                                                  ]", // 0%
        "[=                                                 ]", // 2%
        "[==                                                ]", // 4%
        "[===                                               ]", // 6%
        "[====                                              ]", // 8%
        "[=====                                             ]", // 10%
        "[======                                            ]", // 12%
        "[=======                                           ]", // 14%
        "[========                                          ]", // 16%
        "[=========                                         ]", // 18%
        "[==========                                        ]", // 20%
        "[===========                                       ]", // 22%
        "[============                                      ]", // 24%
        "[=============                                     ]", // 26%
        "[==============                                    ]", // 28%
        "[===============                                   ]", // 30%
        "[================                                  ]", // 32%
        "[=================                                 ]", // 34%
        "[==================                                ]", // 36%
        "[===================                               ]", // 38%
        "[====================                              ]", // 40%
        "[=====================                             ]", // 42%
        "[======================                            ]", // 44%
        "[=======================                           ]", // 46%
        "[========================                          ]", // 48%
        "[=========================                         ]", // 50%
        "[==========================                        ]", // 52%
        "[===========================                       ]", // 54%
        "[============================                      ]", // 56%
        "[=============================                     ]", // 58%
        "[==============================                    ]", // 60%
        "[===============================                   ]", // 62%
        "[================================                  ]", // 64%
        "[=================================                 ]", // 66%
        "[==================================                ]", // 68%
        "[===================================               ]", // 70%
        "[====================================              ]", // 72%
        "[=====================================             ]", // 74%
        "[======================================            ]", // 76%
        "[=======================================           ]", // 78%
        "[========================================          ]", // 80%
        "[=========================================         ]", // 82%
        "[==========================================        ]", // 84%
        "[===========================================       ]", // 86%
        "[============================================      ]", // 88%
        "[=============================================     ]", // 90%
        "[==============================================    ]", // 92%
        "[===============================================   ]", // 94%
        "[================================================  ]", // 96%
        "[================================================= ]", // 98%
        "[==================================================]"  // 100%
    };

    static constexpr const char* s_bar_style1[51]{
        "[                                                  ]", // 0%
        "[#                                                 ]", // 2%
        "[##                                                ]", // 4%
        "[###                                               ]", // 6%
        "[####                                              ]", // 8%
        "[#####                                             ]", // 10%
        "[######                                            ]", // 12%
        "[#######                                           ]", // 14%
        "[########                                          ]", // 16%
        "[#########                                         ]", // 18%
        "[##########                                        ]", // 20%
        "[###########                                       ]", // 22%
        "[############                                      ]", // 24%
        "[#############                                     ]", // 26%
        "[##############                                    ]", // 28%
        "[###############                                   ]", // 30%
        "[################                                  ]", // 32%
        "[#################                                 ]", // 34%
        "[##################                                ]", // 36%
        "[###################                               ]", // 38%
        "[####################                              ]", // 40%
        "[#####################                             ]", // 42%
        "[######################                            ]", // 44%
        "[#######################                           ]", // 46%
        "[########################                          ]", // 48%
        "[#########################                         ]", // 50%
        "[##########################                        ]", // 52%
        "[###########################                       ]", // 54%
        "[############################                      ]", // 56%
        "[#############################                     ]", // 58%
        "[##############################                    ]", // 60%
        "[###############################                   ]", // 62%
        "[################################                  ]", // 64%
        "[#################################                 ]", // 66%
        "[##################################                ]", // 68%
        "[###################################               ]", // 70%
        "[####################################              ]", // 72%
        "[#####################################             ]", // 74%
        "[######################################            ]", // 76%
        "[#######################################           ]", // 78%
        "[########################################          ]", // 80%
        "[#########################################         ]", // 82%
        "[##########################################        ]", // 84%
        "[###########################################       ]", // 86%
        "[############################################      ]", // 88%
        "[#############################################     ]", // 90%
        "[##############################################    ]", // 92%
        "[###############################################   ]", // 94%
        "[################################################  ]", // 96%
        "[################################################# ]", // 98%
        "[##################################################]"  // 100%
    };

    // Array of pointers to each style
    static constexpr const char* const* s_bar_styles[] = {
        s_bar_style0,
        s_bar_style1,
    };

    static constexpr int s_bar_style_count = sizeof(s_bar_styles) / sizeof(s_bar_styles[0]);

  public:
    // default destructor
    ~fastprogressbar() = default;

    // delete everything else
    fastprogressbar(fastprogressbar const&)            = delete;
    fastprogressbar& operator=(fastprogressbar const&) = delete;
    fastprogressbar(fastprogressbar&&)                 = delete;
    fastprogressbar& operator=(fastprogressbar&&)      = delete;

    // default constructor, must call set_niter later
    inline fastprogressbar();
    inline fastprogressbar(int n, bool showbar = true, std::ostream& out = std::cerr);

    // reset bar to use it again
    inline void reset();
    // set bar style
    inline void set_bar_style(int style);
    // set number of loop iterations
    inline void set_niter(int iter);
    // to show only the percentage
    inline void show_bar(bool flag = true) { m_showBar = flag; }
    // set the output stream
    inline void set_output_stream(const std::ostream& stream) { m_output.rdbuf(stream.rdbuf()); }
    // main function
    inline void update();

  private:
    int               m_cycles;
    std::atomic<int>  m_progress;
    std::atomic<int>  m_lastPerc;
    bool              m_showBar;
    std::mutex        m_mtx;
    std::ostream&     m_output;
    std::stringstream m_outputCache;
    int               m_barStyle = 0;
};

inline fastprogressbar::fastprogressbar()
    : m_progress(0),
      m_cycles(0),
      m_lastPerc(0),
      m_showBar(true),
      m_output(std::cerr) {}

inline fastprogressbar::fastprogressbar(int n, bool showbar, std::ostream& out)
    : m_progress(0),
      m_cycles(n),
      m_lastPerc(0),
      m_showBar(showbar),
      m_output(out) {}

inline void fastprogressbar::reset()
{
    m_progress = 0,
    m_lastPerc = 0;
}

inline void fastprogressbar::set_bar_style(int style)
{
    m_barStyle = style;
    if (m_barStyle < 0)
    {
        m_barStyle = 0;
    }
    else if (m_barStyle >= s_bar_style_count)
    {
        m_barStyle = s_bar_style_count - 1;
    }
}

inline void fastprogressbar::set_niter(int niter)
{
    m_cycles = niter;
}

inline void fastprogressbar::update()
{
    if (0 >= m_cycles)
    {
        return;
    }

    int old_progress = m_progress.fetch_add(1, std::memory_order::memory_order_relaxed);

    // compute percentage, return if it is not changed
    int perc = (old_progress + 1) * 100.0f / m_cycles;
    if (perc > 100)
    {
        perc = 100;
    }

    int old_last_perc = m_lastPerc.load(std::memory_order::memory_order_relaxed);
    if (perc <= old_last_perc)
        return;

    // update percentage by 1% only
    if (m_lastPerc.compare_exchange_weak(old_last_perc, perc, std::memory_order::memory_order_relaxed, std::memory_order::memory_order_relaxed))
    {
        auto fn_update = [this, perc]() {
            // clear line
            m_outputCache.clear();
            m_outputCache.str("");
            m_outputCache << "\r";

            if (m_showBar)
            {
                // use precomputed bar from lookup table
                int lut_index = perc / 2;
                m_outputCache << s_bar_styles[m_barStyle][lut_index] << " ";
            }

            // percentage
            m_outputCache << perc << "%";

            m_output << m_outputCache.str();
            m_output << std::flush;
        };

        if (perc >= 100)
        {
            // ensure the last update
            std::lock_guard<std::mutex> lock(m_mtx);
            fn_update();
        }
        else
        {
            // update only if the lock is available
            if (m_mtx.try_lock())
            {
                fn_update();
                m_mtx.unlock();
            }
        }
    }
}
