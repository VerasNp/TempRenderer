#pragma once
#include "temprenderer/pd/editor/IWidget.hpp"
#include <functional>
#include <imgui.h>

namespace temprenderer::pd::editor {
class MainLayoutWidget : public IWidget {
public:
  void setOnRenderRequested(std::function<void()> callback) {
    onRenderRequested_ = std::move(callback);
  }

  void draw() override {
    const ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(mainViewport->WorkPos);
    ImGui::SetNextWindowSize(mainViewport->WorkSize);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin("MainLayout", nullptr, flags);
    drawToolbar();
    drawMainArea();
    ImGui::End();
  }

private:
  void drawToolbar() {
    if (ImGui::Button("Render")) {
      if (onRenderRequested_) {
        onRenderRequested_();
      }
    }
    ImGui::SameLine();
    ImGui::Separator();
  }

  void drawMainArea() {
    ImVec2 available = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("MainArea", available, true);
    ImGui::Text("Preview da cena (edição) vai aqui");
    ImGui::EndChild();
  }

  std::function<void()> onRenderRequested_;
};
} // namespace temprenderer::pd::editor
