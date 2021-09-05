#pragma once

#include <string>
#include <string_view>

#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;

#include "response.hpp"

namespace smtp {

class Request {
public:
  Request(tcp::socket &socket) : socket{socket} {}

  auto receive() -> Request &;

  auto send(std::string_view message, bool wait_for_response = true)
      -> Response;

  auto message() const -> const std::string &;

private:
  std::string from;
  std::string to;

  std::string _message;

  tcp::socket &socket;
};

} // namespace smtp