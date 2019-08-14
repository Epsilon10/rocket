
#include "../include/session.hh"

#include <string>
#include <sstream>
#include <vector>

#include <sys/sendfile.h>  // sendfile
#include <fcntl.h>         // open
#include <unistd.h>        // close
#include <sys/stat.h>      // fstat
#include <sys/types.h>     // fstat

#include "exception.hpp"
#include "response.hpp"

Session::Session() : timeout_(3) { }

Session::~Session() { }

std::string Session::read_request(uint32_t max) {
  std::vector<uint8_t> buff(max);

  uint32_t read_bytes = 0;
  uint32_t match_from = 0;

  while (read_bytes < max) {
    int received = stream_.recv(buff.data() + read_bytes, max - read_bytes, timeout_);
    if (received == -1 || received == 0)
      throw transport_error("Can't recieve data");
    
    uint32_t line_start = read_bytes;
    for(uint32_t i = 0; i < read_bytes + received; i++) {
      // TODO: process
    }

    read_bytes += received;
  }

  throw protocol_error("Broken request");
}

State Session::handle_input() {
  try {
    std::string request = read_request(4096);

    std::string method;
    std::string url;

    std::stringstream ss(request);
    ss >> method;
    ss >> url;

    if (!url.empty())
      url.erase(0,1);
    
    int source = open(url.c_str(), O_RDONLY, 0);
    if (source < 0)
      throw exception("gay");

    
    struct stat stat_source;
    fstat(source, &stat_source);

    std::stringstream reply;
    reply << "HTTP/1.1 200 OK" << "\r\n";
    reply << "Content-Type: text/plain; charset=utf-8" << "\r\n";
    reply << "Content-Length: " << stat_source.st_size << "\r\n";
    reply << "\r\n";

    std::string reply_string(reply.str());
    stream_.send(reply_string.c_str(), reply_string.length());
    
    sendfile(stream_.get_handle(), source, 0, stat_source.st_size);

    close(source);
  } catch (protocol_error& e) {
    
  }

  return State::CLOSE;
}




