#pragma once

#include "temprenderer/ISubsystem.hpp"
#include "temprenderer/platform/graphics/WindowManager.hpp"

namespace temprenderer::pd::editor {
class EditorManager : public ISubsystem {
public:
  EditorManager() = default;
  ~EditorManager() override = default;

  void startUp() override;
  void shutDown() override;

  void beginFrame();
  void endFrame();

  void showSceneEditor(bool *open);

  void setWindowManager(platform::graphics::WindowManager &windowManager);

private:
  platform::graphics::WindowManager *windowManager_ = nullptr;
  bool isEditorInit_ = false;
};
} // namespace temprenderer::pd::editor
