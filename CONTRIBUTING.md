# Guia de Setup para Desenvolvimento

Este documento lista tudo que é necessário instalar para contribuir com o projeto, desde build, testes, formatação, lint
e hooks de commit.

## Índice

- [Guia de Setup para Desenvolvimento](#guia-de-setup-para-desenvolvimento)
	- [Índice](#índice)
	- [Toolchain de Build](#toolchain-de-build)
	- [Qualidade de Código](#qualidade-de-código)
	- [Testes](#testes)
	- [Git Hooks (pre-commit)](#git-hooks-pre-commit)
		- [Ativando os hooks](#ativando-os-hooks)
		- [O que roda em cada commit](#o-que-roda-em-cada-commit)
	- [CI](#ci)
	- [Setup Rápido](#setup-rápido)

---

## Toolchain de Build

| Ferramenta         | Papel                 | Instalação                |
| ------------------ | --------------------- | ------------------------- |
| **CMake** (≥ 3.20) | Meta-sistema de build | `apt install cmake`       |
| **Ninja**          | Backend de build      | `apt install ninja-build` |
| **Clang**          | Compilador            | `apt install clang`       |

> O CMake já resolve a orquestração entre Ninja e Clang, não é necessário configurar nada manualmente além de garantir
> que os binários estejam no `PATH`.

## Qualidade de Código

| Ferramenta       | Papel                                                    | Instalação                 |
| ---------------- | -------------------------------------------------------- | -------------------------- |
| **clang-format** | Formatador automático                                    | `apt install clang-format` |
| **clang-tidy**   | Linter estático                                          | `apt install clang-tidy`   |
| **cppcheck**     | Análise estática adicional (usado no hook de pre-commit) | `apt install cppcheck`     |

> `clang-format` e `clang-tidy` respeitam as configurações já versionadas em `.clang-format` e `.clang-tidy` na raiz do
> repositório, não é necessário configurar nada manualmente.

## Testes

| Ferramenta  | Papel                     | Instalação                                                                   |
| ----------- | ------------------------- | ---------------------------------------------------------------------------- |
| **CTest**   | Test runner               | Incluído com o CMake                                                         |
| **doctest** | Framework de testes (C++) | Gerenciado via CMake (FetchContent ou submódulo, ver `tests/CMakeLists.txt`) |

## Git Hooks (pre-commit)

Usamos o framework [`pre-commit`](https://pre-commit.com/) para orquestrar hooks locais de formatação, lint e validação
de mensagens de commit.

| Ferramenta           | Papel                               | Instalação                                                                |
| -------------------- | ----------------------------------- | ------------------------------------------------------------------------- |
| **Python 3** (≥ 3.9) | Runtime do `pre-commit`             | Geralmente já vem no sistema; senão `apt install python3`                 |
| **pre-commit**       | Framework de gerenciamento de hooks | `pip install pre-commit`                                                  |
| **gitlint**          | Validação de Conventional Commits   | Instalado automaticamente pelo `pre-commit` via `.pre-commit-config.yaml` |

### Ativando os hooks

Depois de clonar o repositório e instalar as dependências acima:

```bash
pre-commit install                          # hook de pre-commit (formatação, lint)
pre-commit install --hook-type commit-msg   # hook de commit-msg (Conventional Commits)
```

### O que roda em cada commit

- `clang-format`: formata os arquivos `.cpp`/`.hpp` staged;
- `cppcheck`: análise estática rápida nos arquivos staged;
- `trailing-whitespace`, `end-of-file-fixer`, `check-added-large-files`, `check-merge-conflict`, `mixed-line-ending`:
  higiene geral de repositório;
- `gitlint`: valida se a mensagem de commit segue [Conventional Commits](https://www.conventionalcommits.org/)

> Hooks de autofix (como `end-of-file-fixer`) podem modificar arquivos automaticamente. Se isso acontecer, rode
> `git add -A` e tente o commit novamente.

## CI

TODO

---

## Setup Rápido

Resumo para quem só quer copiar e colar (Debian/Ubuntu):

```bash
# Toolchain de build
sudo apt install cmake ninja-build clang

# Qualidade de código
sudo apt install clang-format clang-tidy cppcheck

# Git hooks
pip install pre-commit
pre-commit install
pre-commit install --hook-type commit-msg
```

Depois disso, o primeiro `git commit` vai instalar os ambientes isolados dos hooks (`clang-format`, `gitlint`, etc.)
automaticamente, pode demorar um pouco na primeira vez.
