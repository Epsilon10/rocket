#pragma once

#include <vector>
#include <string>

int constexpr BUFF_SIZE = 1024;

class SockStream {
  public:
    SockStream();
    ~SockStream();

    int get_handle() { return socket_; }
    int set_handle(int socket) { socket_ = socket; }

    int recv(void* buf, size_t len, uint32_t timeout);
    int send(const void* buf, size_t len);

  private:
    int socket_;
};
