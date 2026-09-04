#include "temprenderer/pd/editor/EditorManager.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "temprenderer/core/logging/LoggerManager.hpp"

namespace temprenderer::pd::editor {
void EditorManager::startUp() {
  if (this->isEditorInit_) {
    return;
  }
  LC_LOG_VERBOSE(core::logging::LogLevel::INFO, "Starting up editor manager");
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::GetIO().IniFilename = nullptr;
  ImGui_ImplGlfw_InitForOpenGL(this->windowManager_->getWindowContext(), true);
  ImGui_ImplOpenGL3_Init("#version 330");
  this->isEditorInit_ = true;
}

// NOLINTNEXTLINE(readability-make-member-function-const)
void EditorManager::beginFrame() {
  if (!this->isEditorInit_) {
    return;
  }
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  this->mainLayout_.draw();
  this->renderResult_.draw();
}

// NOLINTNEXTLINE(readability-make-member-function-const)
void EditorManager::endFrame() {
  if (!this->isEditorInit_) {
    return;
  }
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorManager::shutDown() {
  if (!this->isEditorInit_) {
    return;
  }
  LC_LOG_VERBOSE(core::logging::LogLevel::INFO, "Shutting down editor manager");
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  this->isEditorInit_ = false;
}

void EditorManager::setWindowManager(
    platform::graphics::WindowManager &windowManager) {
  this->windowManager_ = &windowManager;
}
} // namespace temprenderer::pd::editor
