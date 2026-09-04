#include "temprenderer/renderer/RendererManager.hpp"
#include "temprenderer/core/config/ApplicationConfig.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"
#include "temprenderer/platform/graphics/WindowManager.hpp"
#include "temprenderer/platform/linux/FileSystem.hpp"

#include <cassert>

namespace temprenderer::renderer {

namespace {

unsigned int compileShader(const unsigned int type, const char *src) {
  const unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == 0) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    LC_LOG(core::logging::LogLevel::ERROR,
           std::string("Shader compile error: ") + infoLog);
  }
  return shader;
}
} // namespace

void RendererManager::startUp() {
  if (this->isRendererInit_) {
    return;
  }
  LC_LOG(core::logging::LogLevel::INFO, "Starting renderer manager");
  this->createQuad();
  this->compileShaders();

  glGenTextures(1, &textureId_);
  glBindTexture(GL_TEXTURE_2D, textureId_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  this->isRendererInit_ = true;
}

void RendererManager::shutDown() {
  if (!isRendererInit_) {
    return;
  }

  glDeleteTextures(1, &this->textureId_);
  glDeleteProgram(this->shaderProgram_);
  glDeleteVertexArrays(1, &this->vao_);
  glDeleteBuffers(1, &this->vbo_);
  glDeleteBuffers(1, &this->ebo_);

  isRendererInit_ = false;
}

void RendererManager::createQuad() {
  constexpr float vertices[] = {
      -1.0F, -1.0F, 0.0F, 0.0F, 1.0F,  -1.0F, 1.0F, 0.0F,
      1.0F,  1.0F,  1.0F, 1.0F, -1.0F, 1.0F,  0.0F, 1.0F,
  };
  const unsigned int indices[] = {0, 1, 2, 2, 3, 0};
  glGenVertexArrays(1, &this->vao_);
  glGenBuffers(1, &this->vbo_);
  glGenBuffers(1, &this->ebo_);
  glBindVertexArray(this->vao_);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        static_cast<void *>(nullptr));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        reinterpret_cast<void *>(2 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);
}

void RendererManager::compileShaders() {
  const std::string vertexCodeStr =
      platform::linux::FileSystem::loadFileSource("shaders/basic.vert");
  const std::string fragmentCodeStr =
      platform::linux::FileSystem::loadFileSource("shaders/basic.frag");
  const char *vShaderCode = vertexCodeStr.c_str();
  const char *fShaderCode = fragmentCodeStr.c_str();
  const unsigned int vertexShader =
      compileShader(GL_VERTEX_SHADER, vShaderCode);
  const unsigned int fragmentShader =
      compileShader(GL_FRAGMENT_SHADER, fShaderCode);
  this->shaderProgram_ = glCreateProgram();
  glAttachShader(this->shaderProgram_, vertexShader);
  glAttachShader(this->shaderProgram_, fragmentShader);
  glLinkProgram(this->shaderProgram_);
  int success;
  glGetProgramiv(this->shaderProgram_, GL_LINK_STATUS, &success);
  if (success == 0) {
    char infoLog[512];
    glGetProgramInfoLog(this->shaderProgram_, 512, nullptr, infoLog);
    LC_LOG(core::logging::LogLevel::ERROR,
           std::string("Shader link error: ") + infoLog);
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void RendererManager::setCanvas(const Canvas &canvas) {
  this->canvasHeight_ = canvas.getRenderingHeight();
  this->canvasWidth_ = canvas.getRenderingWidth();
  glBindTexture(GL_TEXTURE_2D, this->textureId_);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->canvasWidth_,
               this->canvasHeight_, 0, GL_RGB, GL_UNSIGNED_BYTE, canvas.data());
}
void RendererManager::draw() const {
  glUseProgram(this->shaderProgram_);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->textureId_);
  glUniform1i(glGetUniformLocation(this->shaderProgram_, "uCanvas"), 0);
  glBindVertexArray(this->vao_);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

} // namespace temprenderer::renderer
