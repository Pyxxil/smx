#pragma once

namespace http {

enum Status {
  OK = 200,
  BadRequest = 400,
  NotFound = 404,
};

using HeaderValue = std::vector<std::string>;
using Headers = std::map<std::string, HeaderValue>;

} // namespace http