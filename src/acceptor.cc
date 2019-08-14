#include <memory>

#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../include/epoll_thread.hh"

Acceptor::~Acceptor() {
  ::close(socket_);
}

Acceptor::Acceptor() : timeout_(3) { }

auto Acceptor::open(EpollThread const& worker) -> void {
  if ((socket_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    // throw
  }

  int yes;
  if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    // throw
  }

  sockaddr_in serveraddr;
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = INADDR_ANY;
  serveraddr.sin_port = htons(PORT);
  
  memset(&(serveraddr.sin_zero), '\0', 8);
  
  if (bind(socket_, SOL_SOCKET, SO_RCVTIMEO, 
        reinterpret_cast<struct socketaddr*>(&serveraddr), sizeof(serveraddr)) == -1) { 
    // throw 
  }

  if (listen(socket_, SOMAXCONN) == -1) { }

  struct timeval tv;
  tv.tv_sec = timeout_;
  tv.tv_usec = 0;

  setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&tv),sizeof(struct timeval));
  
  worker.register_handler(shared_from_this());
}

void Acceptor::accept(SocketStream& stream) {
  int socket;
  struct sockaddr_in clientaddr;
  uint32_t addr_len = sizeof(clientaddr);
  
  if ((socket = ::accept(socket_, reinterpret_cast<struct sockaddr*>(&clientaddr), &addr_len)) == -1) {
    // throw
  }

  stream.set_handle(socket);
}

void Acceptor::close() {
  
}

State Acceptor::handle_input() {
  SessionPtr session(new Session());
  accept(session->stream());
  
  EpollThread& worker = thread_pool_.get();
  worker.register_handler(session);
}
