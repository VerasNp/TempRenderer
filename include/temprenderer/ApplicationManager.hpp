#pragma once

#include "ISubsystem.hpp"
#include "pd/editor/EditorManager.hpp"
#include "renderer/RendererManager.hpp"

namespace temprenderer {
class ApplicationManager : public ISubsystem {
public:
  void startUp() override;
  void run();
  void shutDown() override;

  void setApplicationConfig(const core::config::ApplicationConfig &config);

  static void setConfigFilePath(const std::string &path) {
    configFilePath = path;
  };

private:
  bool isApplicationInit_ = false;
  platform::graphics::WindowManager windowManager_;
  pd::editor::EditorManager editorManager_;
  core::config::ApplicationConfig config_;
  renderer::RendererManager renderManager_;

  static inline std::string configFilePath;
};
} // namespace temprenderer
