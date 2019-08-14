#pragma once

#include <unordered_map>
#include <mutex>
#include <memory>

#include <sys/epoll.h>

#include "handler.hh"

int constexpr  MAX_EVENTS = 1024;

class Reactor {
  public:
    ~Reactor() = default;
    Reactor() = default;

    void register_handler(HandlerPtr handler);
    void resume_handler(HandlerPtr handler);
    void remove_handler(HandlerPtr handler);
    void handle_events();

  private;
    int epfd_;

    struct epoll_event event_;
    std::unordered_map<int, HandlerPtr> handlers_;
    std::mutex mutex_;
};
