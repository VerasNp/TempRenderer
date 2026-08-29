#include "temprenderer/core/cli/CliOptions.hpp"
#include <cli11/CLI11.hpp>

namespace temprenderer::core::cli {

CliOptions parse(int argc, char **argv) {
  CLI::App app{"TempRenderer, renderizador"};

  CliOptions options{};

  app.add_option("-c,--config", options.configPath,
                 "Caminho do arquivo de configuracao TOML")
      ->default_val(options.configPath);

  app.add_flag("-v,--verbose", options.verbose, "Roda com logs detalhados");
  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    std::exit(app.exit(e));
  }
  return options;
}

} // namespace temprenderer::core::cli
