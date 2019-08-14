#pragma once

#include "handler.hh"
#include "thread_pool.hh"

class Acceptor;
using std::shared_ptr<Acceptor> AcceptorPtr;

class SockStream;
class Acceptor : public Handler {
  public:
    ~Acceptor();
    Acceptor& operator=(const Accetpr&) = delete;
    Acceptor(const Acceptor&) = delete;
    explicit Acceptor(ThreadPool const& thread_pool);

    auto open(EpollThread& thread) -> void;
    void accept(SockStream& stream);
    void close();
    State handle_input();

  private:
    ThreadPool const& thread_pool_;
};
