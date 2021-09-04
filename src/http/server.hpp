#pragma once

#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "request.hpp"
#include "response.hpp"

namespace http {

using Handler = std::function<void(const Request &, Response &)>;

class Server {
public:
  Server() = default;

  /**
   * Attach a handler to a particular path
   */
  auto attach(std::string_view path, Handler handler) -> Server &;

  /**
   * Bind to a particular port
   */
  auto bind(int port) -> Server &;

  /**
   * Run the server
   */
  auto run() -> int;

private:
  int port = 3000;
  std::map<std::string, Handler> handlers;
};

} // namespace http