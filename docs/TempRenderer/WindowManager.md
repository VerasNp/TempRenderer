# temprenderer::platform::graphics

| | |
|---|---|
| [`WindowProps`](#temprendererplatformgraphicswindowprops) | parâmetros de criação de janela *(struct)* |
| [`WindowManager`](#temprendererplatformgraphicswindowmanager) | gerencia janela e contexto gráfico via GLFW *(class)* |

# temprenderer::platform::graphics::WindowProps

```cpp
struct WindowProps {
    std::string title;
    unsigned int width;
    unsigned int height;

    explicit WindowProps(std::string title_ = "LeninCanvas",
                          unsigned int width_ = 1280,
                          unsigned int height_ = 768);
};
```

Agrupa os parâmetros necessários para criar uma janela: título, largura e altura.

## Membros de dados

| Nome | Tipo | Descrição |
|---|---|---|
| `title` | `std::string` | Título da janela. |
| `width` | `unsigned int` | Largura da janela, em pixels. |
| `height` | `unsigned int` | Altura da janela, em pixels. |

## Funções membro

| | |
|---|---|
| [`(construtor)`](#construtor) | constrói `WindowProps`, com valores padrão para todos os parâmetros *(função membro pública)* |

## (construtor)

```cpp
explicit WindowProps(std::string title_, unsigned int width_, unsigned int height_);
```

Constrói uma `WindowProps`.

**Parâmetros**

- `title_`: título da janela.
- `width_`: largura em pixels.
- `height_`: altura em pixels.

**Notas**

O construtor é `explicit`. Isso impede conversões implícitas de um único argumento (ex.: `WindowProps props = "Foo";` não compila), mas não afeta a construção com todos os argumentos padrão (`WindowProps props;` continua válido).

# temprenderer::platform::graphics::WindowManager

```cpp
class WindowManager : public temprenderer::ISubsystem {
public:
    WindowManager() = default;
    ~WindowManager() override = default;

    void startUp() override;
    void shutDown() override;

    bool createWindow(const WindowProps& props = WindowProps());
    void update() const;
    bool shouldClose() const;

    [[nodiscard]] unsigned int getWidth() const;
    [[nodiscard]] unsigned int getHeight() const;
};
```

`WindowManager` gerencia a janela e o contexto gráfico da aplicação por meio do GLFW. Implementa [`ISubsystem`](ISubsystem.md), seguindo o ciclo de vida `startUp()` / `shutDown()`. Quando compilado com a macro `API_OPENGL` definida, também inicializa o carregamento de ponteiros de função OpenGL via GLAD.

## Funções membro

| | |
|---|---|
| [`startUp`](#startup) | inicializa o GLFW *(função membro pública)* |
| [`shutDown`](#shutdown) | destrói a janela e finaliza o GLFW *(função membro pública)* |
| [`createWindow`](#createwindow) | cria a janela com as propriedades especificadas *(função membro pública)* |
| [`update`](#update) | processa eventos e troca os buffers da janela *(função membro pública)* |
| [`shouldClose`](#shouldclose) | verifica se a janela recebeu um pedido de fechamento *(função membro pública)* |
| [`getWidth`](#getwidth) | retorna a largura atual da janela *(função membro pública)* |
| [`getHeight`](#getheight) | retorna a altura atual da janela *(função membro pública)* |

## `startUp`

```cpp
void startUp() override;
```

Inicializa o GLFW (`glfwInit`) e define os *window hints* apropriados: sem contexto de cliente e não redimensionável quando `API_OPENGL` não está definido, ou versão 3.3 core profile quando está.

**Parâmetros**
(nenhum)

**Valor de retorno**
(nenhum)

**Notas**

Encerra o processo (`exit(1)`) caso `glfwInit()` falhe.

## `shutDown`

```cpp
void shutDown() override;
```

Destrói a janela (`glfwDestroyWindow`) e finaliza o GLFW (`glfwTerminate`).

**Parâmetros**
(nenhum)

**Valor de retorno**
(nenhum)

**Notas**

Chamar `shutDown()` sem antes chamar `createWindow()` com sucesso destrói uma janela nula.

## `createWindow`

```cpp
bool createWindow(const WindowProps& props = WindowProps());
```

Cria a janela GLFW com o título, largura e altura de `props`. Se compilado com `API_OPENGL`, também torna o contexto OpenGL corrente, carrega os ponteiros de função via GLAD e registra o callback de redimensionamento do framebuffer.

**Parâmetros**

- `props`: propriedades da janela a ser criada (padrão: `WindowProps{}`).

**Valor de retorno**

`true` se a janela foi criada com sucesso; `false` se o GLFW ainda não foi inicializado (`startUp()` não foi chamado).

**Notas**

Encerra o processo (`exit(1)`) caso a criação da janela ou o carregamento do GLAD falhem.

## `update`

```cpp
void update() const;
```

Processa eventos pendentes da janela (`glfwPollEvents`). Se compilado com `API_OPENGL`, também troca os buffers (`glfwSwapBuffers`) antes de processar os eventos.

**Parâmetros**
(nenhum)

**Valor de retorno**
(nenhum)

## `shouldClose`

```cpp
bool shouldClose() const;
```

Indica se a janela recebeu um pedido de fechamento (ex.: usuário clicou no "X").

**Parâmetros**
(nenhum)

**Valor de retorno**

`true` se um fechamento foi solicitado; `false` caso contrário.

## `getWidth`

```cpp
[[nodiscard]] unsigned int getWidth() const;
```

Retorna a largura da janela.

**Valor de retorno**

Largura da janela, em pixels.

## `getHeight`

```cpp
[[nodiscard]] unsigned int getHeight() const;
```

Retorna a altura da janela.

**Valor de retorno**

Altura da janela, em pixels.

## Exemplo

```cpp
#include "temprenderer/platform/graphics/WindowManager.hpp"

int main() {
    using namespace temprenderer::platform::graphics;

    WindowManager windowManager;
    windowManager.startUp();
    windowManager.createWindow(WindowProps("LeninsCanvas", 1280, 720));

    while (!windowManager.shouldClose()) {
        windowManager.update();
    }

    windowManager.shutDown();
}
```
