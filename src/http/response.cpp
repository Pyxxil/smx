#include <iostream>

#include <boost/asio/write.hpp>

#include "response.hpp"

constexpr auto status(http::Status stat) -> std::string_view {
  switch (stat) {
  case http::Status::BadRequest:
    return "Bad Request";
  default:
    return "Ok";
  }
}

namespace http {

void Response::send(std::string_view response) {
  std::stringstream sstream;

  sstream << "HTTP/1.1 " << this->_status << ' ' << ::status(this->_status)
          << "\r\n"
          << "Connection: closed\r\n\r\n"
          << response << "\r\n";

  boost::asio::write(*(this->socket), boost::asio::buffer(sstream.str()));
}

auto Response::status(Status _status) -> Response & {
  this->_status = _status;

  return *this;
}

auto Response::header(std::string_view header, std::string_view value)
    -> Response & {
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

} // namespace http