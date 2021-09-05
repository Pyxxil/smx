#pragma once

#include <string>
#include <string_view>

#include "common.hpp"

namespace smtp {

class Response {
public:
  Response() = default;

  Response(std::string_view message);

  auto status(smtp::Status stat) -> Response &;

  auto body(std::string_view message) -> Response &;

  auto response() const -> std::string;

  auto ok() const -> bool;

private:
  smtp::Status _status = smtp::Status::NONE;
  std::string _body;
};

} // namespace smtp