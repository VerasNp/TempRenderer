#pragma once

#include "ISubsystem.hpp"
#include "core/config/ApplicationConfig.hpp"
#include "pd/editor/EditorManager.hpp"
#include "platform/graphics/WindowManager.hpp"
#include "renderer/RendererManager.hpp"

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

private:
  bool isApplicationInit_ = false;
  platform::graphics::WindowManager windowManager_;
  pd::editor::EditorManager editorManager_;
  core::config::ApplicationConfig config_;
  renderer::RendererManager renderManager_;
};
} // namespace temprenderer
