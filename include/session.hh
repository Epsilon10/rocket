#pragma once

#include <memory>

#include "handler.hh"

using SessionPtr = std::shared_ptr<Session>;

class Session : public Handler {
  public:
    ~Session();
    Session();

    SockStream& stream() { return stream_; };
    int get_handle() const { return stream_.get_handle(); }
    State handle_input();

    std::string read_request(uint32_t max);

  private:
    SockStream stream_;
    uint32_t timeout_;

};
