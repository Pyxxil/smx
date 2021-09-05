#include <iostream>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <fmt/color.h>
#include <fmt/core.h>

using boost::asio::ip::tcp;

#include "../logger.hpp"
#include "./server.hpp"

namespace http {

auto Server::attach(std::string_view path, Handler handler) -> Server & {
  handlers.emplace(path, handler);

  return *this;
}

auto Server::bind(int port) -> Server & {
  this->port = port;

  return *this;
}

auto Server::run() -> int {
  Logger logger("HTTP Server", fmt::color::cadet_blue);
  logger.log(fmt::format("listening on http://localhost:{}", port));

  const boost::asio::ip::port_type port(this->port);

  boost::asio::io_context io_context;

  auto acceptor = tcp::acceptor(io_context, {tcp::v4(), port});

  while (true) {
    auto socket = acceptor.accept();

    const auto request = Request(socket);

    auto response = Response(socket);

    auto &&handler = this->handlers.find(request.path());

    if (handler != this->handlers.cend()) {
      handler->second(request, response);
    } else {
      response.status(http::Status::NotFound).send("Invalid route");
    }

    auto status = response.get_status();
    logger.log(fmt::format("{} {}", request.path(), status),
               response.get_status() == http::Status::OK ? LogLevel::INFO
                                                         : LogLevel::ERROR);
  }
}

} // namespace http