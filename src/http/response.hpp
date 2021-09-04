#pragma once

#include <string_view>

#include <boost/asio/ip/tcp.hpp>
using boost::asio::ip::tcp;

#include "common.hpp"

namespace http {

class Response {
public:
  Response(tcp::socket &socket) : headers{}, socket{&socket} {}

  /**
   * Send a response back to the client
   */
  void send(std::string_view content);

  auto header(std::string_view header, std::string_view value) -> Response &;

private:
  tcp::socket *socket;
  http::Headers headers;
};

} // namespace http