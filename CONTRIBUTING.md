# Guia para Desenvolvimento

Este documento lista tudo que é necessário instalar e contribuir com o projeto, desde build, testes, formatação, lint
e hooks de commit.

## Índice

- [Guia para Desenvolvimento](#guia-para-desenvolvimento)
	- [Índice](#índice)
	- [Toolchain de Build](#toolchain-de-build)
	- [Qualidade de Código](#qualidade-de-código)
	- [Testes](#testes)
	- [Git Hooks (pre-commit)](#git-hooks-pre-commit)
	- [Configuração da aplicação com `config.toml`](#configuração-da-aplicação-com-configtoml)
	- [Configuração do projeto](#configuração-do-projeto)
	- [Setup Rápido](#setup-rápido)
	- [Antes de começar a contribuir](#antes-de-começar-a-contribuir)
	- [Guideline de mensagens de commits](#guideline-de-mensagens-de-commits)

---

## Toolchain de Build

| Ferramenta         | Papel                 |
|--------------------|-----------------------|
| **CMake** (≥ 3.20) | Meta-sistema de build |
| **Ninja**          | Backend de build      |
| **Clang**          | Compilador            |

> O CMake já resolve a orquestração entre Ninja e Clang, não é necessário configurar nada manualmente além de garantir
> que os binários estejam no `PATH`.

## Qualidade de Código

| Ferramenta       | Papel                                                    |
|------------------|----------------------------------------------------------|
| **clang-format** | Formatador automático                                    |
| **clang-tidy**   | Linter estático                                          |
| **cppcheck**     | Análise estática adicional (usado no hook de pre-commit) |

> `clang-format` e `clang-tidy` respeitam as configurações já versionadas em `.clang-format` e `.clang-tidy` na raiz do
> repositório, não é necessário configurar nada manualmente.

## Testes

| Ferramenta  | Papel                     |
|-------------|---------------------------|
| **CTest**   | Test runner               |
| **doctest** | Framework de testes (C++) |

## Git Hooks (pre-commit)

Usamos o framework [`pre-commit`](https://pre-commit.com/) para orquestrar hooks locais de formatação, lint e validação
de mensagens de commit.

| Ferramenta           | Papel                               | Instalação               |
|----------------------|-------------------------------------|--------------------------|
| **Python 3** (≥ 3.9) | Runtime do `pre-commit`             | -                        |
| **pre-commit**       | Framework de gerenciamento de hooks | `pip install pre-commit` |
| **gitlint**          | Validação de Conventional Commits   | -                        |

### Ativando os hooks

Após clonar o repositório e instalar as dependências acima:

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

## Configuração da aplicação com `config.toml`

É necessário indicar algumas configuraçÕes em um arquivo `.toml`, um exemplo de configuração está presente
em [example_config.toml](https://github.com/VerasNp/TempRenderer/blob/main/example_config.toml).

### Window

- `title = "<string>"`: Título da janela da aplicação
- `width = <integer>`: Largura da janela da aplicação
- `height = <integer>`: Altura da janela da aplicação

### Render

- `resolution_width = <integer>`: Largura da resolução da renderização
- `aspect_ratio = "16:9" | "4:3" | "21:9"`: Razão entre largura e altura da resolução
- `viewport_height = <float>`: Tamanho físico da viewport

## Configuração do projeto

A aplicação trabalha com um arquivo di tipo JSON para definição de configurações específicas de cada projeto:

```json
{
	// Nome do projeto
	"name": "<string>",
	// Configurações da cena renderizada
	"scene": {
		// Unidades métricas
		"units": {
			// Unidade métrica de tamanhos
			"length": "km|m|cm"
		},
		// Configuração de mundo
		"world": {
			// Cor do background ([0, 255])
			"background": {
				"color": {
					"r": <integer>,
					"g": <integer>,
					"b": <integer>
				}
			}
		},
		// Configuração de câmera
		"camera": {
			// Nome do componente câmera
			"name": "<string>",
			// Posicionamento da câmera
			"eye": {
				"x": <float>,
				"y": <float>,
				"z": <float>
			},
			// Distância da câmera até viewport
			"focal_length": <float>
		},
		// Configuração das câmeras
		"lights": [
			{
				// Nome do componente luz
				"name": "<string>",
				// Tipos da luz
				"type": "ambient|point",
				// Posicionamento da luz
				"position": {
					"x": <float>,
					"y": <float>,
					"z": <float>
				},
				// Intensidade da luz
				"intensity": <float>,
				// Cor da luz ([0, 255])
				"color": {
					"r": <integer>,
					"g": <integer>,
					"b": <integer>
				}
			}
		],
		// Configuração dos objetos da cena
		"objects": [
			{
				// Nome do componente objeto
				"name": "<string>",
				"type": "plane|sphere|cone|cylinder",
				// Configurações específicas do objeto...
				"material": {
					// Componente de reflexão ambiente
					"ka": {
						"r": <integer>,
						"g": <integer>,
						"b": <integer>
					},
					// Componente de reflexão difusa
					"kd": {
						"r": <integer>,
						"g": <integer>,
						"b": <integer>
					},
					// Componente de reflexão especular
					"ks": {
						"r": <integer>,
						"g": <integer>,
						"b": <integer>
					},
					// Componente de rugosidade do material
					"alpha": <float>
				}
			}
		]
	}
}

```

## Setup Rápido (Exemplo para Debian/Ubuntu)

Resumo para quem só quer copiar e colar (Debian/Ubuntu):

```bash
# Toolchain de build
sudo apt install cmake ninja-build clang

# Qualidade de código
sudo apt install clang-format clang-tidy cppcheck

# Clone o projeto
git clone https://github.com/VerasNp/TempRenderer.git

cd TempRenderer

# Git hooks
pip install pre-commit
pre-commit install
pre-commit install --hook-type commit-msg

# Execução do projeto
make run
```

Depois disso, o primeiro `git commit` vai instalar os ambientes isolados dos hooks (`clang-format`, `gitlint`, etc.)
automaticamente, pode demorar um pouco na primeira vez.

## Antes de começar a contribuir

Para garantir um bom processo de contribuição e evitar esforço desperdiçado, por favor, faça um esforço em seguir a
guideline antes de iniciar qualquer trabalho.

### Consulte as issues

Provavelmente alguma coisa de importante a ser desenvolvida vai estar descrita ou indicada
nas [GitHub Issues](https://github.com/VerasNp/TempRenderer/issues), assim comece a investigar o que deseja contribuir
por lá, pode ser que você esteja implementando algo que já está em desenvolvimento. Seja comunicativo, isso vai evitar
trabalho desperdiçado!

> Caso seja o cenário de não estar mapeado é recomendado abrir uma issue a respeito para discussão do tema.

### IA como ferramente

Vou ser claro sobre isso, **USE APENAS COMO FERRAMENTA**, ela pode realmente te ajudar a entender alguns conceitos
aplicados no desenvolvimento do projeto ou até rascunhar algo, mas deixo aqui a minha veemente tolerância zero para
vibe coding.

#### Permitido (e encorajado)

- Um tutor para ajudar a enter uma issue ou parte específica.
- Ajudar a limpar a fazer uma limpeza no código ou escrever PRs mais descritivas.
- Fazer você entender **toda linha de código** que for enviada.

#### PROIBIDO (AI SLOP)

- Vibe codar a PR inteira (IA escreveu tudo sem nenhuma supervisão humana).
- Enviar o código sem que realmente entenda ou nem testou.
- Ignorar casos degenerados, pois a IA os ignorou.

> Se for detectado que a PR foi produto de vibe code ela **vai ser automaticamente recusada** e você vai ter feito
> trabalho à toa.

## Workflow de contribuição

### 1. Crie a branch

```shell
git checkout -b <type>/<nome>
# Ex.:
git checkout -b feat/issue-11 # Para branch baseada em issue
# Ou
git checkout -b feat/implementa-malhas
```

> Encorajo que o desenvolvimento seja guiado por issue!

### 2. Faça as mudanças

- Siga o padrão de código
- Escreva um código auto documentado
- Adicione comentários para lógicas muito complexas
- Atualize a documentação se necessário

> O projeto usa o Doxygen, recomendo que o mesmo seja feito para documentar os componentes desenvolvidos

### 3. Teste o seu código

Para maiores detalhes da suíte de testes olhe em [Testes](#testes).

```shell
# Rode o linter
make tidy

# Build com teste
make test
```

### 4. Commit suas mudanças

Siga a [guideline de mensagens de commit](#guideline-de-mensagens-de-commits)

```shell
git add .
git commit
```

### 5. Push e criação de pull request

```shell
git push -u origin <type>/<nome>
```

Abra seu pull request no GitHub com:

- Um título descritivo sobre as mudanças
- Descrição detalhada do que foi mudado e o porquê
- Referência a qualquer issue relacionada

## Guideline de mensagens de commits

Estamos usando o padrão de [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/).

### Formato

```
<type>(<scope>): <description>

[optional body]

[optional footer]
```

### Tipos

| Tipo     | Descrição                                                                                           |
|----------|-----------------------------------------------------------------------------------------------------|
| feat     | Nova feature                                                                                        |
| fix      | Correção de bug                                                                                     |
| docs     | Mudanças de documentação                                                                            |
| style    | Mudanças que não afetam o significado do código (white-space, formatação, e afins)                  |
| refactor | Refatoração de código sem impacto na lógica da aplicação                                            |
| perf     | Alteração que melhorou a performance da aplicação                                                   |
| test     | Criação ou alteração de códigos de teste                                                            |
| build    | Mudanças que afetam o sistema de build ou dependências externas                                     |
| ci       | Mudanças nos arquivos de configuração de CI                                                         |
| chore    | Mudanças no projeto que não afetem o sistema ou arquivos de testes. São mudanças de desenvolvimento |
| revert   | Indica a reverão de um commit anterior                                                              |

### Escopos

Escopos comuns do projeto:

- `core`
- `editor`
- `renderer`
- `scene`

### Dicas

- Use o presente do indicativo (eu fui pesquisar...eu também não sei português, aqui é um ambiente seguro para cometer
	erros gramaticais). Pense na frase "Se aplicado, este commit..."
- Não use letras maiúsculas :)
- Sem pontos finais!
- A descrição do título deve ter no máximo 72 caracteres

### Exemplo

```
# Adição de feature
feat(config): adiciona a leitura de arquivos json

- adiciona uma biblioteca parser específico para o formato json
- implementa um novo adapter para o port de parsers

refs.: #14
```

**Cheat Sheet**: [Conventional Commits Cheat Sheet](https://gist.github.com/Zekfad/f51cb06ac76e2457f11c80ed705c95a3)
