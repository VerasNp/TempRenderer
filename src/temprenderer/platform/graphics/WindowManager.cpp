#include "temprenderer/platform/graphics/WindowManager.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"

#include <iostream>

namespace temprenderer::platform::graphics {
static bool isGLFWInit = false;

void WindowManager::startUp() {
  if (!isGLFWInit) {
    LC_LOG_VERBOSE(core::logging::LogLevel::INFO, "Staring up window manager");
    if (glfwInit() == 0) {
      LC_LOG(core::logging::LogLevel::ERROR, "Failed to initialize GLFW");
      exit(1);
    }
#ifndef API_OPENGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    isGLFWInit = true;
  }
}

void WindowManager::shutDown() {
  LC_LOG_VERBOSE(core::logging::LogLevel::INFO, "Shutting down window manager");
  glfwDestroyWindow(this->window_);
  glfwTerminate();
  isGLFWInit = false;
}

bool WindowManager::createWindow(const WindowProps &props) {
  if (!isGLFWInit) {
    LC_LOG(core::logging::LogLevel::ERROR, "GLFW not initialized");
    return false;
  }
  this->data_.title = props.title;
  this->data_.width = props.width;
  this->data_.height = props.height;
  this->window_ = glfwCreateWindow(static_cast<int>(this->data_.width),
                                   static_cast<int>(this->data_.height),
                                   this->data_.title.c_str(), nullptr, nullptr);
  if (this->window_ == nullptr) {
    LC_LOG(core::logging::LogLevel::ERROR, "Failed to create GLFW window");
    glfwTerminate();
    exit(1);
  }

#ifdef API_OPENGL
  glfwMakeContextCurrent(this->window_);
  if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) ==
      0) {
    LC_LOG(core::logging::LogLevel::ERROR, "Failed to initialize GLAD");
    exit(1);
  }
  glfwSetFramebufferSizeCallback(
      this->window_, [](GLFWwindow *window, const int width, const int height) {
        glViewport(0, 0, width, height);
      });
#endif
  LC_LOG_VERBOSE(core::logging::LogLevel::INFO, "Created GLFW window");
  return true;
}

void WindowManager::update() const {
#ifdef API_OPENGL
  glfwPollEvents();
#endif
}

void WindowManager::swapBuffers() const {
#ifdef API_OPENGL
  glfwSwapBuffers(this->window_);
#endif
}

bool WindowManager::shouldClose() const {
  return glfwWindowShouldClose(this->window_) != 0;
}

GLFWwindow *WindowManager::getWindowContext() {
  return glfwGetCurrentContext();
}
} // namespace temprenderer::platform::graphics
