#pragma once
#include <string>

namespace temprenderer::core::cli {

/**
 * @brief CLI options
 */
struct CliOptions {
  /**
   * @brief Application config file path
   */
  std::string applicationConfigFilePath = "config.toml";
  /**
   * @brief Project config file path
   */
  std::string projectConfigFilePath;
  /**
   * @brief Application verbose
   */
  bool verbose = false;
};

/**
 * @brief Parses command-line arguments.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return Parsed command-line options.
 */
[[nodiscard]] CliOptions parse(int argc, char **argv);

} // namespace temprenderer::core::cli
