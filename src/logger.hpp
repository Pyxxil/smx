#include <string_view>

#include <fmt/color.h>

enum LogLevel { INFO, WARNING, ERROR };

class Logger {
public:
  Logger(std::string_view context, fmt::color colour);

  auto log(std::string_view info, LogLevel level = LogLevel::INFO) -> Logger &;

private:
  std::string context;
  fmt::color colour;
};