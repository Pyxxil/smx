#include <iostream>
#include <sstream>

#include <boost/asio/read_until.hpp>

#include "../utils.hpp"
#include "request.hpp"

namespace http {

Request::Request(tcp::socket &socket) {
  std::string request;

  auto n = boost::asio::read_until(
      socket, boost::asio::dynamic_string_buffer(request), "\r\n\r\n");

  std::size_t position = 0;

  auto &&pos = request.find("\r\n");
  std::string status_line(request.substr(0, pos));

  auto &&start = status_line.find(' ') + 1;
  auto &&end = status_line.rfind(' ');

  this->_path = trim(status_line.substr(start, end - start));

  request = request.substr(pos + 1, request.length());

  while (std::string::npos != (position = request.find("\r\n"))) {
    std::string line(request.substr(0, position));

    request = request.substr(position + 1, request.length());

    if (line.empty())
      // We've reached the message body
      break;

    auto &&colon = line.find(':');

    if (colon == std::string::npos)
      continue;

    this->add_header(trim(line.substr(0, colon)),
                     trim(line.substr(colon + 2, line.length())));
  }

  auto &&body = trim(request);

  if (!body.empty())
    this->add_body(body);
}

auto Request::add_header(std::string_view header, std::string_view value)
    -> Request & {
  const auto position = this->headers.find(std::string{header});

  if (position != this->headers.cend()) {
    position->second.emplace_back(value);
  } else {
    std::vector<std::string> values{};
    values.emplace_back(value);
    this->headers.emplace(header, values);
  }

  return *this;
}

auto Request::body() const -> const std::string & { return this->mBody; }

auto Request::path() const -> const std::string & { return this->_path; }

auto Request::add_body(std::string_view _body) -> Request & {
  this->mBody = _body;

  return *this;
}

auto Request::header(std::string_view _header) const
    -> std::optional<HeaderValue> {
  auto &&head = this->headers.find(std::string{_header});

  if (head == this->headers.cend())
    return std::nullopt;

  return std::make_optional(head->second);
}

} // namespace http