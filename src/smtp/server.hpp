#pragma once

#include "request.hpp"
#include "response.hpp"

namespace smtp {

class Server {
public:
  Server() = default;

  void run();
};

} // namespace smtp