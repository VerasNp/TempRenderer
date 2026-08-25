#pragma once

#include <string>

namespace temprenderer::core::config {
struct WindowConfig {
  std::string title = "TempRenderer Engine";
  unsigned int width = 1280;
  unsigned int height = 720;
};

struct ApplicationConfig {
  WindowConfig window;
};

class ApplicationConfigLoader {
public:
  static ApplicationConfig loadFromFile(const std::string &path);
};
} // namespace temprenderer::core::config
