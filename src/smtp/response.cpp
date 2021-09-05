#include <fmt/core.h>

#include "../utils.hpp"
#include "response.hpp"

namespace smtp {

Response::Response(std::string_view message) {
  auto &&pos = message.find(' ');

  if (std::string::npos != pos) {
    auto &&status = message.substr(0, pos);
    if (status == "220") {
      this->status(smtp::Status::ServiceReady);
    } else if (status == "221") {
      this->status(smtp::Status::Goodbye);
    } else if (status == "250") {
      this->status(smtp::Status::Ok);
    } else if (status == "421") {
      this->status(smtp::Status::Unavailable);
    }

    auto &&body = message.substr(pos + 1);
    this->body(body);
  }
}

auto Response::status(smtp::Status stat) -> Response & {
  this->_status = stat;

  return *this;
}

auto Response::body(std::string_view message) -> Response & {
  this->_body = message;

  return *this;
}

auto Response::response() const -> std::string {
  if (this->_status == smtp::Status::NONE) {
    return this->_body;
  }

  return fmt::format("{} {}", this->_status, trim(this->_body));
}

// This could be expanded to include other types of errors, however this is
// fine for now
auto Response::ok() const -> bool {
  return this->_status != smtp::Status::Unavailable &&
         this->_status != smtp::Status::InvalidCommandSequence;
}

} // namespace smtp