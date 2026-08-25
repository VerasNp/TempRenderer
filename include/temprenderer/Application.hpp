#pragma once

#include "ISubsystem.hpp"
#include "pd/editor/EditorManager.hpp"
#include "renderer/RendererManager.hpp"

namespace temprenderer {
class Application : public ISubsystem {
public:
  Application() = default;
  ~Application() override = default;

  void startUp() override;
  void run();
  void shutDown() override;

  [[nodiscard]] renderer::RendererManager getRendererManager();
  [[nodiscard]] renderer::RendererManager getEditorManager();

  void setEditorManager(pd::editor::EditorManager editorManager);
  void setWindowManager(platform::graphics::WindowManager windowManager);

private:
  bool isApplicationInit_ = false;
  platform::graphics::WindowManager windowManager_;
  pd::editor::EditorManager editorManager_;
  core::config::ApplicationConfig config_;
};
} // namespace temprenderer
