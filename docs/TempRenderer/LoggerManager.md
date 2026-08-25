# temprenderer::core::logging::LoggerManager

```cpp
class LoggerManager : public ISubsystem {
public:
    LoggerManager() = default;
    virtual ~LoggerManager() = default;

    void startUp() override;
    void shutDown() override;

    static void log(LogLevel logLevel, const std::string& message);

private:
    static std::string levelToString(LogLevel level);
};
```

`LoggerManager` é responsável por gerenciar o log do sistema. Herda de [`ISubsystem`](ISubsystem.md) e segue o padrão de ciclo de vida `startUp()` / `shutDown()`. As mensagens de log são categorizadas em cinco níveis de severidade, definidos pelo enum `LogLevel`.

## O `LogLevel`

```cpp
enum class LogLevel { DEBUG, INFO, WARNING, ERROR, UNKNOWN };
```

| Constante | Descrição                                                       |
| --------- | --------------------------------------------------------------- |
| `DEBUG`   | Informações detalhadas úteis apenas em desenvolvimento.         |
| `INFO`    | Mensagens informativas sobre o funcionamento normal do sistema. |
| `WARNING` | Situação inesperada, mas que não impede a execução.             |
| `ERROR`   | Falha que compromete a operação corrente.                       |
| `UNKNOWN` | Nível não classificado / fallback.                              |

## Funções membro

|                         |                                                                                                 |
| ----------------------- | ----------------------------------------------------------------------------------------------- |
| [`startUp`](#startup)   | inicializa o subsistema de logging *(função membro pública)*                                    |
| [`shutDown`](#shutdown) | finaliza o subsistema de logging *(função membro pública)*                                      |
| [`log`](#log)           | registra uma mensagem com o nível de severidade especificado *(função membro pública estática)* |

---

## `startUp`

```cpp
void startUp() override;
```

Inicializa o subsistema de logging. Deve ser chamada antes de qualquer chamada a `log`.

**Parâmetros**
(nenhum)

**Valor de retorno**
(nenhum)

---

## `shutDown`

```cpp
void shutDown() override;
```

Finaliza o subsistema de logging.

**Parâmetros**
(nenhum)

**Valor de retorno**
(nenhum)

---

## `log`

```cpp
static void log(LogLevel logLevel, const std::string& message);
```

Registra `message` com o nível de severidade `logLevel` assim como precedido de um timestamp do momento de output do log.

**Parâmetros**

- `logLevel`: nível de severidade da mensagem.
- `message`: texto a ser registrado.

**Valor de retorno**
(nenhum)

**Notas**

Em vez de chamar `LoggerManager::log` diretamente, prefira a macro de conveniência `LC_LOG`:

```cpp
#define LC_LOG(level, msg) temprenderer::core::logging::LoggerManager::log(level, msg)
```

---

## Exemplo

```cpp
#include "LoggerManager.hpp"

int main() {
    temprenderer::core::logging::LoggerManager loggerManager;
    loggerManager.startUp();
    LC_LOG(temprenderer::core::logging::LogLevel::DEBUG, "Foo");
    LC_LOG(temprenderer::core::logging::LogLevel::INFO, "Foo");
    LC_LOG(temprenderer::core::logging::LogLevel::WARNING, "Foo");
    LC_LOG(temprenderer::core::logging::LogLevel::ERROR, "Foo");
    LC_LOG(temprenderer::core::logging::LogLevel::UNKNOWN, "Foo");
    loggerManager.shutDown();
}
```

Saída possível:

```
[2026-08-24 22:06:05] [DEBUG] Foo
[2026-08-24 22:06:05] [INFO] Foo
[2026-08-24 22:06:05] [WARNING] Foo
[2026-08-24 22:06:05] [ERROR] Foo
[2026-08-24 22:06:05] [UNKNOWN] Foo
```
