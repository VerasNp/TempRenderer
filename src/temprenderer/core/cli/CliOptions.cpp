#include "temprenderer/core/cli/CliOptions.hpp"
#include <cli11/CLI11.hpp>

namespace temprenderer::core::cli {

CliOptions parse(int argc, char **argv) {
  CLI::App app{"TempRenderer, render(!?)"};

  CliOptions options{};

  app.add_option("-c,--config", options.applicationConfigFilePath,
                 "Path to TOML application config file")
      ->default_val(options.applicationConfigFilePath);

  app.add_option("-p,--project", options.projectConfigFilePath,
                 "Path to JSON project config file")
      ->default_val(options.projectConfigFilePath);

  app.add_flag("-v,--verbose", options.verbose, "Roda com logs detalhados");
  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    std::exit(app.exit(e));
  }
  return options;
}

} // namespace temprenderer::core::cli
