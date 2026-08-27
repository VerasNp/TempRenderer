#include "temprenderer/core/config/ApplicationConfig.hpp"

#include "temprenderer/core/logging/LoggerManager.hpp"

#include <kwp/kwp_config.hpp>
#include <toml++/toml.hpp>

namespace temprenderer::core::config {
ApplicationConfig
ApplicationConfigLoader::loadFromFile(const std::string &path) {
  toml::table table;
  try {
    LC_LOG(logging::LogLevel::INFO, "Loading config from " + path + "...");
    table = toml::parse_file(path);
  } catch (const toml::parse_error &err) {
    LC_LOG(logging::LogLevel::ERROR, "Fail to parser log file");
    throw std::runtime_error(std::string("Erro no arquivo de config: ") +
                             err.description().data());
  }
  ApplicationConfig config;
  if (auto window = table["window"].as_table()) {
    config.window.title = (*window)["title"].value_or(config.window.title);
    config.window.width = (*window)["width"].value_or(config.window.width);
    config.window.height = (*window)["height"].value_or(config.window.height);
  }
  if (auto render = table["render"].as_table()) {
    config.render.resolutionWidth =
        (*render)["resolution_width"].value_or(config.render.resolutionWidth);
    const auto aspectStr = (*render)["aspect_ratio"].value<std::string>();
    if (!aspectStr) {
      LC_LOG(logging::LogLevel::ERROR,
             "aspect_ratio was not found or is not a string");
      config.render.aspectRatio = AspectRatio::WIDESCREEN;
    } else {
      LC_LOG(logging::LogLevel::INFO, "aspect_ratio = [" + *aspectStr + "]");

      config.render.aspectRatio = RenderConfig::stringToAspectRatio(*aspectStr);
    }
    config.render.resolutionHeight = RenderConfig::calculateResolutionHeight(
        config.render.aspectRatio, config.render.resolutionWidth);
    config.render.viewportHeight =
        (*render)["viewport_height"].value_or(config.render.viewportHeight);
    config.render.viewportWidth = RenderConfig::calculateViewportWidth(
        config.render.resolutionWidth, config.render.resolutionHeight,
        config.render.viewportHeight);
  }

  if (auto camera = table["camera"].as_table()) {
    if (auto eyeTable = (*camera)["eye"].as_table()) {
      kwp::Scalar x = eyeTable->get_as<double>("x")->get();
      kwp::Scalar y = eyeTable->get_as<double>("y")->get();
      kwp::Scalar z = eyeTable->get_as<double>("z")->get();
      config.camera.eye = kwp::Point3{x, y, z};
    }

    config.camera.focalLength =
        (*camera)["focal_length"].value_or(config.camera.focalLength);
  }
  return config;
}

AspectRatio RenderConfig::stringToAspectRatio(const std::string &aspectRatio) {
  if (aspectRatio == "16:9") {
    return AspectRatio::WIDESCREEN;
  }
  if (aspectRatio == "4:3") {
    return AspectRatio::STANDARD;
  }
  if (aspectRatio == "21:9") {
    return AspectRatio::ULTRAWIDE;
  }
  LC_LOG(logging::LogLevel::WARNING, "aspect_ratio desconhecido: '" +
                                         aspectRatio +
                                         "', usando WIDESCREEN como padrao");
  return AspectRatio::WIDESCREEN;
}

float RenderConfig::aspectRatioToScalar(const AspectRatio aspectRatio) {
  switch (aspectRatio) {
  case AspectRatio::WIDESCREEN:
    return 16.0F / 9.0F;
  case AspectRatio::STANDARD:
    return 4.0F / 3.0F;
  case AspectRatio::ULTRAWIDE:
    return 21.0F / 9.0F;
  default:
    return 16.0F / 9.0F;
  }
}

unsigned int
RenderConfig::calculateResolutionHeight(AspectRatio aspectRatio,
                                        unsigned int resolutionWidth) noexcept {
  kwp::Scalar ratio = aspectRatioToScalar(aspectRatio);
  unsigned int height = static_cast<unsigned int>(resolutionWidth / ratio);
  return (height < 1) ? 1 : height;
}

float RenderConfig::calculateViewportWidth(unsigned int resolutionWidth,
                                           unsigned int resolutionHeight,
                                           float viewportHeight) noexcept {
  return viewportHeight *
         (static_cast<float>(resolutionWidth) / resolutionHeight);
}

} // namespace temprenderer::core::config
