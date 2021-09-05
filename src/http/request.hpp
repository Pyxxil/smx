#pragma once

#include <optional>

#include <boost/asio/ip/tcp.hpp>
using boost::asio::ip::tcp;

#include "common.hpp"

namespace http {

class Request {
public:
  Request(tcp::socket &socket);

  auto header(std::string_view _header) const -> std::optional<HeaderValue>;
  auto body() const -> const std::string &;

  auto path() const -> const std::string &;

private:
  auto add_header(std::string_view header, std::string_view value) -> Request &;
  auto add_body(std::string_view) -> Request &;

  http::Headers headers;
  std::string mBody;
  std::string _path;
};

} // namespace http