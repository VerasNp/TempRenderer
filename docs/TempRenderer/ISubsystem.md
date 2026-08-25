# temprenderer::ISubsystem

```cpp
class ISubsystem {
public:
    ISubsystem() = default;
    virtual ~ISubsystem() = default;

    virtual void startUp() = 0;
    virtual void shutDown() = 0;
};
```

`ISubsystem` é uma interface abstrata (classe base pura) que define o contrato de ciclo de vida comum a todos os subsistemas do motor. Qualquer componente que precise ser inicializado e finalizado de forma explícita e ordenada, evitando o problema da ordem estática de inicialização (*Static Initialization Order Fiasco*), deve herdar de `ISubsystem` e implementar `startUp()` e `shutDown()`.

Classes que implementam `ISubsystem` são tipicamente instanciadas localmente no *Composition Root* (`main.cpp`), e não como estáticas globais.

## Funções membro

|                               |                                                        |
| ----------------------------- | ------------------------------------------------------ |
| [`(construtor)`](#construtor) | constrói o subsistema *(função membro pública)*        |
| [`(destrutor)`](#destrutor)   | destrói o subsistema *(função membro pública virtual)* |
| [`startUp`](#startup)         | inicializa o subsistema *(função membro virtual pura)* |
| [`shutDown`](#shutdown)       | finaliza o subsistema *(função membro virtual pura)*   |

## (construtor)

```cpp
ISubsystem() = default;
```

Construtor padrão. Não realiza inicialização de recursos, isso é responsabilidade de `startUp()`.

## (destrutor)

```cpp
virtual ~ISubsystem() = default;
```

Destrutor virtual padrão. Garante destruição polimórfica correta ao deletar um objeto derivado através de um ponteiro para `ISubsystem`.

**Notas**

O destrutor não substitui `shutDown()`: espera-se que `shutDown()` seja chamado explicitamente antes da destruição do objeto, já que a liberação de recursos (arquivos, contexto gráfico, etc.) deve ser determinística e não depender apenas do fim do escopo.

## `startUp`

```cpp
virtual void startUp() = 0;
```

Função virtual pura. Classes derivadas devem implementá-la para realizar a inicialização do subsistema (alocação de recursos, configuração inicial, etc.).

**Parâmetros**
(nenhum)

**Valor de retorno**
(nenhum)

## `shutDown`

```cpp
virtual void shutDown() = 0;
```

Função virtual pura. Classes derivadas devem implementá-la para liberar quaisquer recursos alocados em `startUp()`.

**Parâmetros**
(nenhum)

**Valor de retorno**
(nenhum)

## Exemplo

```cpp
#include "ISubsystem.hpp"

class MySubsystem : public temprenderer::ISubsystem {
public:
    void startUp() override {
        // inicializa recursos
    }

    void shutDown() override {
        // libera recursos
    }
};

int main() {
    MySubsystem subsystem; // instanciado localmente no Composition Root
    subsystem.startUp();

    // ... uso do subsistema ...

    subsystem.shutDown();
}
```
