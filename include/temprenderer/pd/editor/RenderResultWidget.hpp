#pragma once
#include "temprenderer/pd/editor/IWidget.hpp"
#include <imgui.h>

namespace temprenderer::pd::editor {
class RenderResultWidget : public IWidget {
public:
  void open() { isOpen_ = true; }

  void setTexture(unsigned int textureId, int width, int height) {
    textureId_ = textureId;
    width_ = width;
    height_ = height;
  }

  void draw() override {
    if (!isOpen_)
      return;

    ImGui::SetNextWindowSize(ImVec2((float)width_, (float)height_),
                             ImGuiCond_FirstUseEver);
    ImGui::Begin("Render Result",
                 &isOpen_); // 'X' no canto fecha (seta isOpen_ = false)

    ImVec2 available = ImGui::GetContentRegionAvail();
    ImGui::Image(textureId_, available);

    ImGui::End();
  }

private:
  bool isOpen_ = false;
  unsigned int textureId_ = 0;
  int width_ = 0, height_ = 0;
};
} // namespace temprenderer::pd::editor
