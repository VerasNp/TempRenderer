#pragma once

#ifdef API_OPENGL
#include <glad/glad.h>
#endif

#include "GLFW/glfw3.h"
#include "temprenderer/ISubsystem.hpp"

#include <string>

namespace temprenderer::platform::graphics {
/**
 * @brief Windows properties
 */
struct WindowProps {
  std::string title;
  unsigned int width;
  unsigned int height;

  explicit WindowProps(std::string title_, const unsigned int width_,
                       const unsigned int height_)
      : title(std::move(title_)), width(width_), height(height_) {}
};

/**
 * @brief Orchestrate windows
 */
class WindowManager : public ISubsystem {
public:
  void startUp() override;
  void shutDown() override;

  /**
   * @brief Creates the window referênce
   *
   * @param props Windows properties
   * @return Return if window was created of not
   */
  bool createWindow(const WindowProps &props);
  /**
   * @brief Process events on the window event queue
   */
  void update() const;
  /**
   * @brief Presents rendered frame
   */
  void swapBuffers() const;
  /**
   * @brief Closes the window
   *
   * @return Return if window was closed successfully
   */
  bool shouldClose() const;

  /**
   * @brief Gets windows width
   *
   * @return Return window width
   */
  [[nodiscard]] unsigned int getWidth() const { return this->data_.width; };
  /**
   * @brief Gets windows height
   *
   * @return Return window height
   */
  [[nodiscard]] unsigned int getHeight() const { return this->data_.height; };
  /**
   * @brief Gets window context
   *
   * @return Returns windows context
   */
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  [[nodiscard]] GLFWwindow *getWindowContext() {
    return glfwGetCurrentContext();
  };

private:
  struct WindowData {
    std::string title;
    unsigned int width = 0;
    unsigned int height = 0;
  };

  WindowData data_;
  GLFWwindow *window_ = nullptr;
  bool isWindowInit_ = false;
};
} // namespace temprenderer::platform::graphics
