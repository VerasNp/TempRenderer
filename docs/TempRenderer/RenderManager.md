# temprenderer::renderer::RendererManager

```cpp
class RendererManager : public temprenderer::ISubsystem {
public:
    RendererManager() = default;
    ~RendererManager() override = default;

    void startUp() override;
    void shutDown() override;

    void setEngineConfig(const core::config::EngineConfig& config);
    void run() const;

private:
    platform::graphics::WindowManager window_;
    core::config::EngineConfig config_{};
    bool isRendererInit_ = false;
    bool configSet_ = false;
};
```

`RendererManager` orquestra o ciclo de vida do renderer: possui um [`WindowManager`](WindowManager.md), configurado a partir de uma [`EngineConfig`](EngineConfig.md). Implementa [`ISubsystem`](ISubsystem.md), com inicialização (`startUp`), execução (`run`) e finalização (`shutDown`) como etapas explicitamente separadas.

## Funções membro

|                                       |                                                                      |
| ------------------------------------- | -------------------------------------------------------------------- |
| [`(construtor)`](#construtor)         | constrói o `RendererManager` *(função membro pública)*               |
| [`(destrutor)`](#destrutor)           | destrói o `RendererManager` *(função membro pública virtual)*        |
| [`startUp`](#startup)                 | inicializa a janela *(função membro pública)*                        |
| [`shutDown`](#shutdown)               | finaliza a janela *(função membro pública)*                          |
| [`setEngineConfig`](#setengineconfig) | define a configuração do motor a ser usada *(função membro pública)* |
| [`run`](#run)                         | executa o laço principal de renderização *(função membro pública)*   |

## (construtor)

```cpp
RendererManager() = default;
```

Construtor padrão. Não realiza inicialização de janela ou de recursos gráficos.

## (destrutor)

```cpp
~RendererManager() override = default;
```

Destrutor padrão. Não chama `shutDown()` automaticamente.

## `startUp`

```cpp
void startUp() override;
```

Inicializa o [`WindowManager`](WindowManager.md) interno e cria a janela com base na configuração atual (`config_.window`).

**Parâmetros**
(nenhum)

**Valor de retorno**
(nenhum)

**Pré-condições**

`setEngineConfig()` deve ter sido chamado antes. Isso é verificado via `assert` em builds de debug (`configSet_`); em builds de release, chamar `startUp()` sem configurar antes usa uma `EngineConfig` default-construída silenciosamente.

## `shutDown`

```cpp
void shutDown() override;
```

Finaliza a janela (`window_.shutDown()`) e reseta o estado de inicialização.

**Parâmetros**
(nenhum)

**Valor de retorno**
(nenhum)

## `setEngineConfig`

```cpp
void setEngineConfig(const core::config::EngineConfig& config);
```

Define a configuração usada por `startUp()` ao criar a janela (título, largura, altura).

**Parâmetros**

- `config`: configuração do motor a ser copiada para uso interno.

**Valor de retorno**
(nenhum)

**Notas**

Deve ser chamada **antes** de `startUp()`. Marca internamente `configSet_ = true`, o que satisfaz a pré-condição verificada por `assert` em `startUp()`.

## `run`

```cpp
void run() const;
```

Executa o laço principal: chama `window_.update()` repetidamente até que `window_.shouldClose()` retorne `true`.

**Parâmetros**
(nenhum)

**Valor de retorno**
(nenhum)

## Exemplo

```cpp
#include "temprenderer/renderer/RendererManager.hpp"
#include "temprenderer/core/config/EngineConfig.hpp"

int main() {
    using namespace temprenderer::renderer;
    using namespace temprenderer::core::config;

    RendererManager renderer;

    EngineConfig config;
    config.window.title = "LeninsCanvas";
    config.window.width = 1280;
    config.window.height = 720;
    renderer.setEngineConfig(config);

    renderer.startUp();
    renderer.run();
    renderer.shutDown();
}
```
