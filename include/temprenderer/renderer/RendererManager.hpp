#pragma once

#include "Canvas.hpp"
#include "temprenderer/ISubsystem.hpp"
#include "temprenderer/core/config/ApplicationConfig.hpp"
#include "temprenderer/platform/graphics/WindowManager.hpp"

namespace temprenderer::renderer {
class RendererManager : public ISubsystem {
public:
  void startUp() override;
  void shutDown() override;
  void createQuad();

  void compileShaders();
  void setCanvas(const Canvas &canvas) const;

  void draw() const;

private:
  bool isRendererInit_ = false;
  unsigned int textureId_ = 0;
  unsigned int vao_ = 0, vbo_ = 0, ebo_ = 0;
  unsigned int shaderProgram_ = 0;
  unsigned int canvasWidth_ = 0, canvasHeight_ = 0;
};
} // namespace temprenderer::renderer
