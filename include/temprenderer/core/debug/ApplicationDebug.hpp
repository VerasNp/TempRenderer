#pragma once

#include <iostream>
#include <ostream>
#include <string>

namespace temprenderer::core::debug {
struct ApplicationConfig {
  static void dump(const std::string &message) {
    std::cout << message << std::endl;
  };

  static void dumpAndDie(const std::string &message = "Died here") {
    std::cout << message << std::endl;
    exit(1);
  };
};
} // namespace temprenderer::core::debug
#define LC_DUMP(msg) temprenderer::core::debug::ApplicationConfig::dump(msg)
#define LC_DUMP_DIE(msg)                                                       \
  temprenderer::core::debug::ApplicationConfig::dumpAndDie(msg)
