#pragma once
#include "temprenderer/core/logging/LoggerManager.hpp"

#include <fstream>
#include <sstream>
#include <string>

namespace temprenderer::platform::linux {
/**
 * @brief Provides utility functions for interacting with the file system
 */
struct FileSystem {
  /**
   * @brief Loads the contents of a file into a string.
   *
   * @param filePath Path to the file to be loaded.
   * @return The complete contents of the file as a string. Returns an empty string if the file cannot be opened.
   */
  static std::string loadFileSource(const std::string &filePath) {
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
      LC_LOG(core::logging::LogLevel::ERROR, "Failed to open shader file");
      return "";
    }
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
  }
};
} // namespace temprenderer::platform::linux
