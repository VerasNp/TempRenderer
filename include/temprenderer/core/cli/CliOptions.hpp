#pragma once
#include <string>

namespace temprenderer::core::cli {

struct CliOptions {
  std::string configPath = "config.toml";
  bool verbose = false;
};

[[nodiscard]] CliOptions parse(int argc, char **argv);

} // namespace temprenderer::core::cli
