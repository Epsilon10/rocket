#include "../include/reactor.hh"

#include <memory>
#include <cstring>
#include <string>

#include <unistd.h>
#include <netdb.h>
#include <sys/epoll.h>

Reactor::Reactor() {
  epfd_ = epoll_create(MAX_EVENTS);
  if (epfd_ = -1)
      // error
}

Reactor::~Reactor() {
  close(epfd_);
}

void register_handler(HandlerPtr handler) {
  std::lock_gaurd<std::mutex> gaurd(mutex_);

  struct epoll_event ev;

  ev.events = EPOLLIN | EPOLLONESHOT;
  ev.data.fd = handler->get_handle();

  if (epoll_ctl(epfd_, EPOLL_CTL_ADD, handler->get_handle(), &v) < 0) {
      // error
      // 
  }

  handler->reactor(*this); // deduced to reference
  handlers_.insert(std::make_pair(handler->get_handle(), handler));
  
}

void Reactor::resume_handler(HandlerPtr handle) {
  std::lock_gaurd<std::mutex> gaurd(mutex_);
  
  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLONESHOT;
  ev.data.fd = handler->get_handle();
  
  if (epoll_ctl(epfd_, EPOLL_CTL_MOD, handler->get_handle(), &v) < 0) {
      // error
      // 
  }

}

void Reactor::remove_handler(HandlerPtr handle) {
  std::lock_gaurd<std::mutex> gaurd(mutex_);

}
