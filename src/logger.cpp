#include <fmt/core.h>

#include "logger.hpp"

Logger::Logger(std::string_view _context, fmt::color _colour)
    : context{_context}, colour{_colour} {}

auto Logger::log(std::string_view info, LogLevel level) -> Logger & {
  fmt::print(
      "[{}] {}\n", fmt::format(fmt::fg(this->colour), this->context),
      fmt::format(fmt::fg(level == LogLevel::ERROR     ? fmt::color::red
                          : level == LogLevel::WARNING ? fmt::color::yellow
                                                       : fmt::color::white),
                  info));
  return *this;
}