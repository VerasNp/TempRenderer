#pragma once
#include "temprenderer/core/logging/LoggerManager.hpp"

#include <fstream>
#include <sstream>
#include <string>

namespace temprenderer::platform::linux {
struct FileSystem {
  static std::string loadFileSource(const std::string &filePath) {
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
      LC_LOG(core::logging::LogLevel::ERROR, "Failed to open shader file");
      return "";
    }
    std::stringstream buffer;
    buffer << fileStream.rdbuf(); // Read the entire file buffer
    return buffer.str();
  }
};
} // namespace temprenderer::platform::linux
