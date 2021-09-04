#include <iostream>

#include <boost/asio/write.hpp>

#include "response.hpp"

namespace http {

void Response::send(std::string_view content) {
  std::stringstream sstream;

  sstream << "HTTP/1.1 200 Ok\r\nConnection: "
             "closed\r\n\r\n"
          << content << "\r\n";

  boost::asio::write(*(this->socket), boost::asio::buffer(sstream.str()));
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