#include "../include/thread_pool.hh"
#include <algorithm>

ThreadPool::ThreadPool(uint32_t num_threads) : workers_(num_threads) {
  for (uint32_t i = 0; i < num_threads; i++)
    workers_.emplace_back(EpollThread());
}

EpollThread& ThreadPool::get() {
  return *std::min_element(workers_.cbegin(), workers_.cend(), 
      [](auto const& a, auto const& b){ return a.num_handlers() < b.num_handlers(); });
}
