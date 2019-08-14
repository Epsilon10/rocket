#include "../include/sock_stream.hh"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

SockStream::SockStream() : socket_(0) { }

SockStream::~SockStream() {
  ::close(socket_);
}

int SockStream::recv(void* buf, size_t len, uint32_t timeout) {
  struct timeval tv;

  tv.tv_sec = timeout;
  tv.tv_usec = 0;

  ::setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, 
      reinterpret_cast<char*>(&tv), sizeof(struct timeval));
  return ::recv(socket_, buf, len, 0);
}

int SockStream::send(const void* buf, size_t len) {
  return ::send(socket_, buf, len);
}
