#include <iostream>
#include <thread>

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;

#include "http/request.hpp"
#include "http/response.hpp"
#include "http/server.hpp"
#include "smtp/server.hpp"

auto main(int argc, char **argv) -> int {
  auto smtpServer = smtp::Server();

  auto httpServer =
      http::Server()
          // I'm not actually sure what port this should run on ...
          .bind(3000)
          // I don't think we actually need path's in this format, but it
          // was fun setting this up
          .attach("/",
                  [](const http::Request &request, http::Response &response) {
                    const auto &content_type = request.header("Content-Type");

                    boost::asio::io_context io_context;
                    auto resolver = tcp::resolver(io_context);
                    auto endpoint = resolver.resolve(
                        tcp::resolver::query(tcp::v4(), "127.0.0.1", "25"));
                    auto socket = tcp::socket(io_context);
                    boost::asio::connect(socket, endpoint);

                    socket.send(boost::asio::buffer("EHLO localhost\r\n"));

                    response.header("Content-Type", content_type->at(0))
                        .send(request.body());
                  });

  auto httpThread = std::thread(&http::Server::run, &httpServer);
  auto smtpThread = std::thread(&smtp::Server::run, &smtpServer);

  httpThread.join();

  return 0;
}