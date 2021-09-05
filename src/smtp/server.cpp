#include <iostream>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <fmt/color.h>
#include <fmt/core.h>

using boost::asio::ip::tcp;

#include "../logger.hpp"
#include "../utils.hpp"
#include "common.hpp"
#include "server.hpp"

namespace smtp {

void Server::run() {
  Logger logger("SMTP Server", fmt::color::violet);

  logger.log("listening on http://localhost:25");

  const boost::asio::ip::port_type port(25);

  boost::asio::io_context io_context;

  auto acceptor = tcp::acceptor(io_context, {tcp::v4(), port});

  std::string initiation_message =
      fmt::format("{} localhost", smtp::Status::ServiceReady);

  while (true) {
    auto socket = acceptor.accept();

    logger.log(initiation_message);
    auto &&res = Request(socket).send(initiation_message);

    std::string message = res.response();

    /**
     * In reality, I should probably store the information I'm about to gather
     * here, but I'm not going to be doing anything with it, so I decided
     * against it.
     */

    bool is_data = false;

    while (true) {
      if (is_data) {
        if (message.rfind("\r\n.\r\n") != std::string::npos) {
          is_data = false;
          // This should probably be an actual queue number, but no need for it
          // at the moment
          std::string response =
              fmt::format("{} Ok: queued as 123", smtp::Status::Ok);
          logger.log(response);
          message = Request(socket).send(response).response();
        } else {
          message += Request(socket).receive().message();
        }
      } else {
        auto &&pos = message.find(' ');

        auto &&command = trim(message.substr(0, pos));
        auto &&arguments =
            pos == std::string::npos ? "" : trim(message.substr(pos + 1));

        if (command == "DATA") {
          is_data = true;

          std::string response =
              fmt::format("{} End data with <CR><LF>.<CR><LF>",
                          smtp::Status::StartMailInpit);

          logger.log(response);

          message = Request(socket).send(response).response();
        } else if (command == "EHLO") {
          std::string response =
              fmt::format("{} Hello {}, nice to meet you my friend",
                          smtp::Status::Ok, arguments);
          logger.log(response);
          message = Request(socket).send(response).response();
        } else if (command == "MAIL") {
          std::string response = fmt::format("{} Ok", smtp::Status::Ok);
          logger.log(response);
          message = Request(socket).send(response).response();
        } else if (command == "RCPT") {
          std::string response = fmt::format("{} Ok", smtp::Status::Ok);
          logger.log(response);
          message = Request(socket).send(response).response();
        } else if (command == "QUIT") {
          std::string response = fmt::format("{} Bye", smtp::Status::Goodbye);
          logger.log(response);
          Request(socket).send(response, false);
          break;
        } else {
          std::string response =
              fmt::format("{} Invalid sequence of commands",
                          smtp::Status::InvalidCommandSequence);
          logger.log(response);
          Request(socket).send(response, false);
          break;
        }
      }
    }
  }
}

} // namespace smtp
