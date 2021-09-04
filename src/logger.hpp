
#include <fmt/color.h>

class Logger {
public:
  Logger(std::string_view context, fmt::color colour);

  auto log(std::string_view info) -> Logger &;

private:
  std::string context;
  fmt::color colour;
};