#include "Klein.h"

namespace Klein
{
    
    /*Inizializza le variabili per il multithreading*/
    static const int         s_maxThreads = static_cast<int>(std::thread::hardware_concurrency());
    static std::thread* const s_threadPool = new std::thread[s_maxThreads];

    int          getMaxThreads() { return s_maxThreads; }
    std::thread* getThreadPool() { return s_threadPool; }

    std::chrono::steady_clock::time_point Benchmark::s_start;
    std::chrono::steady_clock::time_point Benchmark::s_end;
} //namespace Klein