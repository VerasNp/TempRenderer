#include "temprenderer/renderer/RendererManager.hpp"
#include "temprenderer/core/config/ApplicationConfig.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"
#include "temprenderer/platform/graphics/WindowManager.hpp"
#include <cassert>

namespace temprenderer::renderer {

namespace {
const char *kVertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
out vec2 uv;
void main() {
    uv = aUV;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char *kFragmentShaderSrc = R"(
#version 330 core
in vec2 uv;
out vec4 fragColor;
uniform sampler2D canvasTexture;
void main() {
    // v invertido: Canvas linha 0 = topo, mas textura OpenGL v=0 = base
    fragColor = texture(canvasTexture, vec2(uv.x, 1.0 - uv.y));
}
)";

unsigned int compileShader(unsigned int type, const char *src) {
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    LC_LOG(core::logging::LogLevel::ERROR,
           std::string("Shader compile error: ") + infoLog);
  }
  return shader;
}
} // namespace

void RendererManager::startUp() {
  if (isRendererInit_) {
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
  isRendererInit_ = true;
}

void RendererManager::shutDown() {
  if (!isRendererInit_) {
    return;
  }

  isRendererInit_ = false;
}

void RendererManager::createQuad() {
  float vertices[] = {
      -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 0.0f,
      1.0f,  1.0f,  1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 1.0f,
  };
  unsigned int indices[] = {0, 1, 2, 2, 3, 0};

  glGenVertexArrays(1, &this->vao_);
  glGenBuffers(1, &this->vbo_);
  glGenBuffers(1, &this->ebo_);

  glBindVertexArray(this->vao_);

  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void RendererManager::compileShaders() {
  unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, kVertexShaderSrc);
  unsigned int fragmentShader =
      compileShader(GL_FRAGMENT_SHADER, kFragmentShaderSrc);

  this->shaderProgram_ = glCreateProgram();
  glAttachShader(this->shaderProgram_, vertexShader);
  glAttachShader(this->shaderProgram_, fragmentShader);
  glLinkProgram(this->shaderProgram_);

  int success;
  glGetProgramiv(this->shaderProgram_, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(this->shaderProgram_, 512, nullptr, infoLog);
    LC_LOG(core::logging::LogLevel::ERROR,
           std::string("Shader link error: ") + infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void RendererManager::setCanvas(const Canvas &canvas) const {
  glBindTexture(GL_TEXTURE_2D, this->textureId_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->canvasWidth_,
               this->canvasHeight_, 0, GL_RGB, GL_UNSIGNED_BYTE, canvas.data());
}
void RendererManager::draw() const {
  glUseProgram(this->shaderProgram_);
  glBindTexture(GL_TEXTURE_2D, this->textureId_);
  glBindVertexArray(this->vao_);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

} // namespace temprenderer::renderer
