#include <iostream>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <fmt/color.h>

using boost::asio::ip::tcp;

#include "../logger.hpp"
#include "server.hpp"

namespace smtp {

void Server::run() {
  Logger logger("SMTP Server", fmt::color::violet);

  logger.log("listening on http://localhost:25");

  const boost::asio::ip::port_type port(25);

  boost::asio::io_context io_context;

  auto acceptor = tcp::acceptor(io_context, {tcp::v4(), port});

  while (true) {
    auto socket = acceptor.accept();

    std::string request;

    auto n = boost::asio::read_until(
        socket, boost::asio::dynamic_string_buffer(request), "\r\n");

    logger.log("Incoming message:").log(request);

    boost::asio::write(socket, boost::asio::buffer("250"));

    socket.close();
  }
}

} // namespace smtp
