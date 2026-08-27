#include "temprenderer/renderer/RendererManager.hpp"
#include "temprenderer/core/config/ApplicationConfig.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"
#include "temprenderer/platform/graphics/WindowManager.hpp"
#include "temprenderer/platform/linux/FileSystem.hpp"

#include <cassert>

namespace temprenderer::renderer {

namespace {

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
      -1.0f, -1.0f, 0.0f, -1.0f, 1.0f,  0.0f,
      1.0f,  1.0f,  0.0f, 1.0f,  -1.0f, 0.0f,
  };
  unsigned int indices[] = {0, 1, 2, 2, 3, 0};

  // Geração do Vertex Array Object (VAO)
  glGenVertexArrays(1, &this->vao_);
  glGenBuffers(1, &this->vbo_);
  // Geração do Element Buffer Object (EBO)
  glGenBuffers(1, &this->ebo_);

  // Bind do o VAO
  glBindVertexArray(this->vao_);

  // Copia o array de vértices em um buffer para o OpenGL usar
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Copia  os índices para buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  /**
   * 1º argumento: qual atributo do vértice queremos configurar (layout(location
   * = 0) em "basic.vert") 2º atributo: Tamanho do atributo de vértice; 3º
   * atributo: Tipo do dado 4º atributo: Dado deve ser normalizado? Dados
   * inteiros (int, byte) com isso setado para verdadeiro são levados a 0 (ou -1
   * se sinalizados) e 1 quando convertidos em float 5º argumento: É o stride,
   * espaço entre cada um dos atributos de vértice 6º argumento: É o offset,
   * onde o dado começa
   */
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  // Habilita o atributo de vértice dando como atributo o seu local. Eles são
  // desabilitados por padrão
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void RendererManager::compileShaders() {
  std::string vertexCodeStr = platform::linux::FileSystem::loadFileSource("");
  std::string fragmentCodeStr = platform::linux::FileSystem::loadFileSource("");
  const char *vShaderCode = vertexCodeStr.c_str();
  const char *fShaderCode = fragmentCodeStr.c_str();
  unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vShaderCode);
  unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fShaderCode);

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
  // desenha primitivas usando o shader atualmente ativo, a configuração de
  // atributo de vértice previamente definida e com os dados de vértices do VBO
  // (indiretamente vinculados através do VAO).
  // glDrawArrays(GL_TRIANGLES, 0, 3);

  // O primeiro argumento especifica o modo que queremos desenhar, semelhante ao
  // glDrawArrays. O segundo argumento é a contagem ou o número de elementos que
  // gostaríamos de desenhar. Nós especificamos 6 índices, então queremos
  // desenhar 6 vértices no total. O terceiro argumento é o tipo de índices que
  // é do tipo GL_UNSIGNED_INT. O último argumento nos permite especificar um
  // deslocamento no EBO (ou passar em um array de índice, mas é quando você não
  // está usando objetos de buffer de elementos), mas vamos deixar isso em 0.
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

} // namespace temprenderer::renderer
