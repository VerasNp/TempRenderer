#include "temprenderer/pd/editor/EditorManager.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace temprenderer::pd::editor {
void EditorManager::startUp() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(this->windowManager_->getWindowContext(), true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void EditorManager::beginFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::ShowDemoWindow();
}

void EditorManager::endFrame() {
  ImGui::Render(); // só monta os draw commands, ainda não desenha na GPU
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // isso sim desenha
}

void EditorManager::shutDown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void EditorManager::setWindowManager(
    platform::graphics::WindowManager &windowManager) {
  this->windowManager_ = &windowManager;
}
} // namespace temprenderer::pd::editor
