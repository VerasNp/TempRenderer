#include "temprenderer/core/config/ApplicationConfig.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"

#include <format>
#include <kwp/kwp_config.hpp>
#include <toml++/toml.hpp>

#include <string>

namespace temprenderer::core::config {

namespace {
void logConfigVerbose(const ApplicationConfig &config) {
  LC_LOG_VERBOSE(
      logging::LogLevel::INFO,
      std::format("\n===== ApplicationConfig (verbose) =====\n"
                  "[window]\n"
                  "  title  = {}\n"
                  "  width  = {}\n"
                  "  height = {}\n"
                  "[render]\n"
                  "  resolution_width  = {}\n"
                  "  resolution_height = {}\n"
                  "  aspect_ratio      = {}\n"
                  "  viewport_width    = {}\n"
                  "  viewport_height   = {}\n"
                  "[camera]\n"
                  "  eye.x        = {}\n"
                  "  eye.y        = {}\n"
                  "  eye.z        = {}\n"
                  "  focal_length = {}\n"
                  "========================================",
                  config.window.title, config.window.width,
                  config.window.height, config.render.resolutionWidth,
                  config.render.resolutionHeight,
                  RenderConfig::aspectRatioToScalar(config.render.aspectRatio),
                  config.render.viewportWidth, config.render.viewportHeight,
                  config.camera.eye.x, config.camera.eye.y, config.camera.eye.z,
                  config.camera.focalLength));
}

} // namespace

ApplicationConfig ApplicationConfig::loadFromFile(const std::string &path) {
  toml::table table;
  try {
    LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading config from " + path);
    table = toml::parse_file(path);
  } catch (const toml::parse_error &err) {
    LC_LOG(logging::LogLevel::ERROR, "Failed to parse config file: " + path);
    throw std::runtime_error(std::string("Erro no arquivo de config: ") +
                             err.description().data());
  }

  ApplicationConfig config;
  try {
    if (auto *const window = table["window"].as_table()) {
      LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading window config");
      config.window.title = (*window)["title"].value_or(config.window.title);
      config.window.width = (*window)["width"].value_or(config.window.width);
      config.window.height = (*window)["height"].value_or(config.window.height);
      LC_LOG_VERBOSE(logging::LogLevel::INFO,
                     "Window config loaded successfully");
    }
    if (auto *const render = table["render"].as_table()) {
      LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading render config");
      config.render.resolutionWidth =
          (*render)["resolution_width"].value_or(config.render.resolutionWidth);
      if (const auto aspectStr = (*render)["aspect_ratio"].value<std::string>();
          !aspectStr) {
        LC_LOG(
            logging::LogLevel::ERROR,
            "aspect_ratio was not found or is not a string, using WIDESCREEN");
        config.render.aspectRatio = AspectRatio::WIDESCREEN;
      } else {
        config.render.aspectRatio =
            RenderConfig::stringToAspectRatio(*aspectStr);
      }
      config.render.resolutionHeight = RenderConfig::calculateResolutionHeight(
          config.render.aspectRatio, config.render.resolutionWidth);
      config.render.viewportHeight =
          (*render)["viewport_height"].value_or(config.render.viewportHeight);
      config.render.viewportWidth = RenderConfig::calculateViewportWidth(
          config.render.resolutionWidth, config.render.resolutionHeight,
          config.render.viewportHeight);
      LC_LOG_VERBOSE(logging::LogLevel::INFO,
                     "Render config loaded successfully");
    }
    if (auto *const camera = table["camera"].as_table()) {
      LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading camera config");
      if (auto *const eyeTable = (*camera)["eye"].as_table()) {
        auto *const xNode = eyeTable->get_as<double>("x");
        auto *const yNode = eyeTable->get_as<double>("y");
        auto *const zNode = eyeTable->get_as<double>("z");
        if (xNode == nullptr || yNode == nullptr || zNode == nullptr) {
          LC_LOG(logging::LogLevel::ERROR,
                 "camera.eye: 'x', 'y' or 'z' missing or not a float, "
                 "keeping default eye position");
        } else {
          config.camera.eye = kwp::Point3{static_cast<float>(xNode->get()),
                                          static_cast<float>(yNode->get()),
                                          static_cast<float>(zNode->get())};
        }
      }
      config.camera.focalLength =
          (*camera)["focal_length"].value_or(config.camera.focalLength);
      LC_LOG_VERBOSE(logging::LogLevel::INFO,
                     "Camera config loaded successfully");
    }
  } catch (const std::exception &err) {
    LC_LOG(logging::LogLevel::ERROR,
           std::string("Unexpected error while loading config '") + path +
               "': " + err.what());
    throw;
  }
  logConfigVerbose(config);
  LC_LOG_VERBOSE(logging::LogLevel::INFO,
                 "Application config loaded successfully");
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

unsigned int RenderConfig::calculateResolutionHeight(
    const AspectRatio aspectRatio,
    const unsigned int resolutionWidth) noexcept {
  const kwp::Scalar ratio = aspectRatioToScalar(aspectRatio);
  const unsigned int height =
      static_cast<unsigned int>(resolutionWidth / ratio);
  return (height < 1) ? 1 : height;
}

float RenderConfig::calculateViewportWidth(
    const unsigned int resolutionWidth, const unsigned int resolutionHeight,
    const float viewportHeight) noexcept {
  return viewportHeight *
         (static_cast<float>(resolutionWidth) / resolutionHeight);
}

} // namespace temprenderer::core::config
