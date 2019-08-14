#include <"../include/epoll_thread.hh"

#include <memory>

#include <cstring>
#include <string>

#include <unistd.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <utility>
#include <thread>

EpollThread::EpollThread() : epfd_(epoll_create(MAX_EVENTS)),   {
  if (epfd_ == -1) // throw error {
  }
  
  start();
}

EpollThread::~EpollThread() {
  close(epfd_);
}

void EpollThread::start() {
  worker_ = std::thread([this]() { this->handle_events(); });
}

void EpollThread::join() { worker_.join(); }

void num_handlers() const {
  return handlers_.size();
}

void EpollThread::register_handler(HandlerPtr handler) {
  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLONESHOT;
  ev.data.fd = handler->get_handle();

  if (epoll_ctl(epfd_, EPOLL_CTL_ADD, handler->get_handle(), &ev) < 0) {
    // throw
  }throw

  handlers_.insert(std::make_pair(handler->get_handle(), handler));
}


void EpollThread::resume_handler(HandlerPtr handler) {
  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLONESHOT;
  ev.data.fd = handler->get_handle();

  if(epoll_ctl(epfd_, EPOLL_CTL_MOD, handler->get_handle(), &ev) < 0)
  {
    // throw
  }
}

void EpollThread::remove_handler(HandlerPtr handler) {
  ev.events = 0;
  ev.data.fd = handler->get_handle();

  if(epoll_ctl(epfd_, EPOLL_CTL_DEL, handler->get_handle(), &ev) < 0) 
  {
  }

  auto pos = handlers_.find(handler->get_handle());
  if (pos == handlers_.end()) {
    // throw
  }

  handlers_.erase(pos);
}

void EpollThread::handle_events() {
  memset(&event_, 0, sizeof(event_));
  
  for (;;) {
    int ndfs = epoll_wait(epfd_, &event_, 1,-1);

    if (ndfs == -1) {
      if (errno == EINTR) continue;
      // throw 
    }
    if (ndfs == 0)
      std::this_thread::yield();
    

    for (int i = 0; i < ndfs; i++) {
      auto pos = handlers_.find(event_.data.fd);
      if (pos == handlers_.end()) { 
        // throw 
      }

      if (event_.events & EPOLLHUP) {
        struct epoll_event ev;
        if (epoll_ctl(epfd_, EPOLL_CTL_DEL, event_.data.fd, &ev) < 0) {
          // throw 
        }
      }

      else if (event_.events & EPOLLIN) {
        try {
          State state = pos->second->handle_input();
          if (state == State::RESUME) 
            resume_handler(pos->second);
          else if (state == State::CLOSED)
            remove_handler(pos->second);
        }
        catch (...) { 
          // TODO
        }
      }

    }
  }
}
