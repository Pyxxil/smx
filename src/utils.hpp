#pragma once

#include <string>

inline auto trim(std::string s) -> std::string {
  s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
  s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
  return s;
}