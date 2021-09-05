#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <fmt/color.h>

using boost::asio::ip::tcp;

#include "../logger.hpp"
#include "../utils.hpp"
#include "client.hpp"
#include "request.hpp"
#include "response.hpp"

namespace smtp {

void Client::send(std::string message) {
  Logger logger("SMTP Client", fmt::color::coral);

  boost::asio::io_context io_context;
  auto resolver = tcp::resolver(io_context);
  auto endpoint =
      resolver.resolve(tcp::resolver::query(tcp::v4(), "127.0.0.1", "25"));
  auto socket = tcp::socket(io_context);
  boost::asio::connect(socket, endpoint);

  std::string request;
  auto n = boost::asio::read_until(
      socket, boost::asio::dynamic_string_buffer(request), "\r\n");

  Response response(request);

  if (response.ok()) {
    // All of these should have error handling, but I'm just going to assume
    // that that's not the case for now

    logger.log("EHLO localhost");
    response = Request(socket).send("EHLO localhost");

    std::string from = fmt::format("MAIL FROM: {}", this->_sender);
    logger.log(from);
    response = Request(socket).send(from);

    for (auto &&recipient : this->recipients) {
      std::string message = fmt::format("RCPT TO: {}", recipient);
      logger.log(message);
      response = Request(socket).send(message);
    }

    for (auto &&cc : this->copies) {
      std::string message = fmt::format("RCPT TO: {}", cc);
      logger.log(message);
      response = Request(socket).send(message);
    }

    logger.log("DATA");
    response = Request(socket).send("DATA");

    logger.log(message);
    // This should be chunked up into several messages I think, but I'm not sure
    response = Request(socket).send(message, false);

    logger.log(".");
    response = Request(socket).send("\r\n.");

    logger.log("QUIT");
    auto &&resp = Request(socket).send("QUIT");

    if (resp.ok()) {
      socket.close();
    }
  } else {
    logger.log(response.response(), LogLevel::ERROR);
  }

  auto &&listeners = this->listeners.find(smtp::Event::ConnectionClosed);

  if (listeners != this->listeners.cend()) {
    for (auto &&listener : listeners->second) {
      listener(response);
    }
  }
}

auto Client::sender(std::string_view _sender) -> Client & {
  this->_sender = _sender;

  return *this;
}

auto Client::add_recipient(std::string_view recipient) -> Client & {
  this->recipients.emplace_back(recipient);

  return *this;
}

auto Client::cc(std::string_view to) -> Client & {
  this->copies.emplace_back(to);

  return *this;
}

auto Client::subject(std::string_view subj) -> Client & {
  this->_subject = subj;

  return *this;
}

auto Client::on(smtp::Event event, smtp::Listener listener) -> Client & {
  const auto position = this->listeners.find(event);

  if (position != this->listeners.cend()) {
    position->second.emplace_back(listener);
  } else {
    std::vector<smtp::Listener> listeners{};
    listeners.emplace_back(listener);
    this->listeners.emplace(event, listeners);
  }

  return *this;
}

} // namespace smtp