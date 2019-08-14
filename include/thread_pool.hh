#pragma once

#include "epoll_thread.hh"
#include <thread>

class ThreadPool {
  public:
    ThreadPool(uint32_t num_threads);
    EpollThread& get();
  private:
    std::vector<EpollThread> workers_;
};
