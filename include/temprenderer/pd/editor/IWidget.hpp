#pragma once

namespace temprenderer::pd::editor {
/**
 * @brief Interface for a single self-contained ImGui UI element.
 */
class IWidget {
public:
  virtual ~IWidget() = default;
  /**
   * @brief Issues this widget's ImGui calls for the current frame.
   */
  virtual void draw() = 0;
};
} // namespace temprenderer::pd::editor
