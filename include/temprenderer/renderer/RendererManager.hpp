#pragma once

#include "Canvas.hpp"
#include "temprenderer/ISubsystem.hpp"

namespace temprenderer::renderer {
/**
 * @brief Displays a CPU-computed Canvas on screen.
 */
class RendererManager : public ISubsystem {
public:
  void startUp() override;
  void shutDown() override;
  void createQuad();

  void compileShaders();
  void setCanvas(const Canvas &canvas);

  void draw() const;

  [[nodiscard]] unsigned int getTextureId() const { return this->textureId_; };

private:
  bool isRendererInit_ = false;
  unsigned int textureId_ = 0;
  unsigned int vao_ = 0, vbo_ = 0, ebo_ = 0;
  unsigned int shaderProgram_ = 0;
  unsigned int canvasWidth_ = 0;
  unsigned int canvasHeight_ = 0;
};
} // namespace temprenderer::renderer
