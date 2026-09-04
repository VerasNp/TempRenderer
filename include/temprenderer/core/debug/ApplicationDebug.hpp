#pragma once

#include <iostream>
#include <ostream>
#include <string>

namespace temprenderer::core::debug {
/**
 * @brief Debug application methods
 */
struct ApplicationDebug {
  /**
   * @brief Outputs a message on terminal
   *
   * @param message Message to be displayed
   */
  static void dump(const std::string &message) {
    std::cout << message << std::endl;
  };

  /**
   * @brief Outputs a message on terminal and exit the application
   *
   * @param message Message to be displayed
   */
  static void dumpAndDie(const std::string &message = "Died here") {
    std::cout << message << std::endl;
    exit(1);
  };
};
} // namespace temprenderer::core::debug
#define LC_DUMP(msg) temprenderer::core::debug::ApplicationDebug::dump(msg)
#define LC_DUMP_DIE(msg)                                                       \
  temprenderer::core::debug::ApplicationDebug::dumpAndDie(msg)
