#pragma once

#include <string>
#include <stdexcept>

class exception : public std::runtime_error {
  public:
    explicit exception(std::string const& msg) : std::runtime_error(msg) { }
    explicit exception(std::string const& msg1, std::string const& msg2)
      : std::runtime_error(msg1 + " " + msg2) { }
};

class connection_closed : public exception {
  public:
    explicit connection_closed(std::string const& msg) : exception(msg) { }
};

class transport_error : public exception {
  public:
    transport_error(std::string const& msg) 
      : exception("Transport error: " + msg) { }
};

class protocol_error : public exception {
  public:
    explicit protocol_error(std::string const& retmsg) 
      :exception("Protocol Error [" + retmsg + "]") { }
    explicit protocol_error(std::string const& cmd, std::string const& retmsg) 
      : exception("Protocol Error [" + cmd + "] [" + retmesg + "]") { }
}; 

