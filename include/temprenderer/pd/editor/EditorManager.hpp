#pragma once

#include "MainLayoutWidget.hpp"
#include "RenderResultWidget.hpp"
#include "temprenderer/ISubsystem.hpp"
#include "temprenderer/platform/graphics/WindowManager.hpp"

namespace temprenderer::pd::editor {
/**
 * @brief Orchestrate editor widgets
 */
class EditorManager : public ISubsystem {
public:
  /**
   * @brief Setup Dear ImGui context and platform/renderer backends
   */
  void startUp() override;
  /**
   * @brief Shutdown the ImGui
   */
  void shutDown() override;

  /**
   * @brief Start the Dear ImGui frame
   */
  void beginFrame();
  /**
   * @brief Ends the Dear ImGui frame and sends the draw commands to OpenGL
   */
  void endFrame();

  /**
   * @brief Sets WindowManager referênce to be used
   *
   * @param windowManager WindowManager referênce
   */
  void setWindowManager(platform::graphics::WindowManager &windowManager);

  [[nodiscard]] MainLayoutWidget &mainLayout() { return mainLayout_; }
  [[nodiscard]] RenderResultWidget &renderResult() { return renderResult_; }

private:
  platform::graphics::WindowManager *windowManager_ = nullptr;
  bool isEditorInit_ = false;

  MainLayoutWidget mainLayout_;
  RenderResultWidget renderResult_;
};
} // namespace temprenderer::pd::editor
