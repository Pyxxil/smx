#pragma once

#include <functional>

namespace smtp {

class Response;

enum Status {
  ServiceReady = 220,
  Goodbye = 221,
  Ok = 250,
  StartMailInpit = 354,
  Unavailable = 421,
  InvalidCommandSequence = 503,
  NONE,
};

using Listener = std::function<void(smtp::Response &)>;

enum Event {
  ConnectionClosed,
};

} // namespace smtp