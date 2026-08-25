# temprenderer::core::config

Namespace que agrupa as estruturas de configuração do motor e o utilitário responsável por carregá-las a partir de arquivo.

|                                                                 |                                                             |
| --------------------------------------------------------------- | ----------------------------------------------------------- |
| [`WindowConfig`](#temprendercoreconfigwindowconfig)             | configuração da janela *(struct)*                           |
| [`EngineConfig`](#temprendercoreconfigengineconfig)             | configuração geral do motor *(struct)*                      |
| [`EngineConfigLoader`](#temprendercoreconfigengineconfigloader) | carrega uma `EngineConfig` a partir de um arquivo *(class)* |

# temprenderer::core::config::WindowConfig

```cpp
struct WindowConfig {
    std::string title = "TempRenderer Engine";
    unsigned int width = 1280;
    unsigned int height = 720;
};
```

Agrupa os parâmetros de criação da janela.

## Membros de dados

| Nome     | Tipo           | Valor padrão            | Descrição                          |
| -------- | -------------- | ----------------------- | ---------------------------------- |
| `title`  | `std::string`  | `"TempRenderer Engine"` | Título exibido na barra da janela. |
| `width`  | `unsigned int` | `1280`                  | Largura da janela, em pixels.      |
| `height` | `unsigned int` | `720`                   | Altura da janela, em pixels.       |

# temprenderer::core::config::EngineConfig

```cpp
struct EngineConfig {
    WindowConfig window;
};
```

Agrega todas as configurações do motor. Atualmente contém apenas a configuração de janela; espera-se que novas seções (renderer, input, etc.) sejam adicionadas aqui conforme o projeto cresce.

## Membros de dados

| Nome     | Tipo                                                | Descrição                                                                                  |
| -------- | --------------------------------------------------- | ------------------------------------------------------------------------------------------ |
| `window` | [`WindowConfig`](#temprendercoreconfigwindowconfig) | Configuração da janela. Usa os valores padrão de `WindowConfig` caso não seja sobrescrita. |

# temprenderer::core::config::EngineConfigLoader

```cpp
class EngineConfigLoader {
public:
    static EngineConfig loadFromFile(const std::string& path);
};
```

Classe utilitária (sem estado) responsável por carregar uma [`EngineConfig`](#temprendercoreconfigengineconfig) a partir de um arquivo em disco.

## Funções membro

|                                 |                                                                                            |
| ------------------------------- | ------------------------------------------------------------------------------------------ |
| [`loadFromFile`](#loadfromfile) | carrega a configuração a partir do caminho especificado *(função membro pública estática)* |

## `loadFromFile`

```cpp
static EngineConfig loadFromFile(const std::string& path);
```

Lê o arquivo em `path` e constrói uma [`EngineConfig`](#temprendercoreconfigengineconfig) a partir dele.

**Parâmetros**

- `path`: caminho para o arquivo de configuração.

**Valor de retorno**

Uma instância de `EngineConfig` preenchida com os valores lidos do arquivo.

## Exemplo

```cpp
#include "EngineConfig.hpp"

int main() {
    using namespace temprenderer::core::config;

    EngineConfig config = EngineConfigLoader::loadFromFile("config/engine.json");

    // config.window.title, config.window.width, config.window.height
    // já refletem o conteúdo do arquivo (ou os valores padrão, se aplicável)
}
```
