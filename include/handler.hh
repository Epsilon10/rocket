#pragma once

#include <memory>

enum class State : uint8_t {
  CLOSE,
  RESUME
};

class Reactor;
class Handler : std::enable_shared_from_this<Handler> {
  public:
    virtual ~Handler();
    Handler() = default;
    
    Reactor const& reactor() const { return reactor; }
    void reactor(Reactor const& reactor) { _reactor = reactor; }
    virtual int get_handle() const = 0;
    virtual State handle_input() { }

  private:
    Reactor _reactor;

};

using HandlerPtr = std::shared_ptr<Handler>;
