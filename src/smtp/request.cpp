#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <fmt/core.h>

#include "../utils.hpp"
#include "request.hpp"

namespace smtp {

auto Request::receive() -> Request & {
  std::string message;

  auto n = boost::asio::read_until(
      socket, boost::asio::dynamic_string_buffer(message), "\r\n");

  this->_message = message;

  return *this;
}

auto Request::send(std::string_view message, bool wait_for_response)
    -> Response {
  boost::asio::write(this->socket,
                     boost::asio::buffer(fmt::format("{}\r\n", message)));

  if (wait_for_response)
    this->receive();

  return Response().body(this->message());
}

auto Request::message() const -> const std::string & { return this->_message; }

} // namespace smtp