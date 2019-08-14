#pragma once
#include <thread>
#include <sys/epoll.h>
#include "handler.hh"
#include <unordered_map>

int constexpr MAX_EVENTS = 1024;

class EpollThread {
  public:
    explicit EpollThread();
    ~EpollThread();
    
    void register_handler(HandlerPtr handler);
    void resume_handler(HandlerPtr handler);
    void remove_handler(HandlerPtr handler);

    void handle_events();

    void start();
    void stop();
    void join();

  private:
    std::thread worker_;
    struct epoll_event event_;
    int epfd_;
    std::unordered_map<int, HandlerPtr> handlers_;
};
