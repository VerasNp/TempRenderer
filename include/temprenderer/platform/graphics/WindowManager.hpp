#pragma once

#ifdef API_OPENGL
#include <glad/glad.h>
#endif

#include "GLFW/glfw3.h"
#include "temprenderer/ISubsystem.hpp"

#include <string>

namespace temprenderer::platform::graphics {
struct WindowProps {
  std::string title;
  unsigned int width;
  unsigned int height;

  explicit WindowProps(std::string title_, const unsigned int width_,
                       const unsigned int height_)
      : title(std::move(title_)), width(width_), height(height_) {}
};
class WindowManager : public ISubsystem {
public:
  WindowManager() = default;
  ~WindowManager() override = default;

  void startUp() override;
  void shutDown() override;

  bool createWindow(const WindowProps &props);
  void update() const;
  bool shouldClose() const;
  [[nodiscard]] unsigned int getWidth() const { return this->width_; };
  [[nodiscard]] unsigned int getHeight() const { return this->height_; };
  [[nodiscard]] GLFWwindow *getWindowContext();

private:
  struct WindowData {
    std::string title;
    unsigned int width = 0;
    unsigned int height = 0;
  };

  WindowData data_;
  GLFWwindow *window_ = nullptr;
  unsigned int width_ = 0;
  unsigned int height_ = 0;
};
} // namespace temprenderer::platform::graphics
