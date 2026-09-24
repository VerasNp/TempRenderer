#pragma once

#include "ISubsystem.hpp"
#include "core/config/ApplicationConfig.hpp"
#include "core/config/ProjectConfig.hpp"
#include "pd/editor/EditorManager.hpp"
#include "platform/graphics/WindowManager.hpp"
#include "renderer/RendererManager.hpp"
#include "scene/Scene.hpp"

#include <optional>

namespace temprenderer {
/**
 * @brief Orchestrate the life cycle of all subsystems
 */
class ApplicationManager : public ISubsystem {
public:
  /**
   * @brief Initialize in order each of subsystem
   */
  void startUp() override;
  /**
   * @brief Execute the main application loop
   *
   * @pre startUp() must be called before
   */
  void run();
  /**
   * @brief Ends the application
   *
   * @pre startUp() must be called before
   */
  void shutDown() override;

  /**
   * @brief Defines the application config
   *
   * @param config Loaded configs
   */
  void setApplicationConfig(const core::config::ApplicationConfig &config);

  /**
  * @brief Defines the application config
  *
  * @param config Loaded configs
  */
  void setProjectConfig(const core::config::ProjectConfig &config) {
    this->projectConfig_ = config;
  }

private:
  void renderScene();

  bool isApplicationInit_ = false;
  platform::graphics::WindowManager windowManager_;
  pd::editor::EditorManager editorManager_;
  core::config::ApplicationConfig applicationConfig_;
  core::config::ProjectConfig projectConfig_;
  renderer::RendererManager renderManager_;
  scene::Scene scene_;
};
} // namespace temprenderer
