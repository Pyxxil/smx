#pragma once

#include <string>
#include <vector>

#include "common.hpp"

namespace smtp {

class Client {
public:
  Client() = default;

  auto sender(std::string_view sender) -> Client &;
  auto add_recipient(std::string_view to) -> Client &;
  auto subject(std::string_view subj) -> Client &;
  auto cc(std::string_view to) -> Client &;

  void send(std::string message = "");

  auto on(smtp::Event, smtp::Listener listener) -> Client &;

private:
  std::string _sender;
  std::string _subject = "";
  std::vector<std::string> recipients;
  std::vector<std::string> copies;

  std::map<smtp::Event, std::vector<smtp::Listener>> listeners;
};

} // namespace smtp