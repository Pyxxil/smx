#include <fmt/core.h>

#include "logger.hpp"

Logger::Logger(std::string_view _context, fmt::color _colour)
    : context{_context}, colour{_colour} {}

auto Logger::log(std::string_view info) -> Logger & {
  fmt::print("[{}] {}\n", fmt::format(fmt::fg(this->colour), this->context),
             info);
  return *this;
}