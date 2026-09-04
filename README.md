# TempRender

> Motor de renderização.

![license](https://img.shields.io/badge/license-GNU_AGPLv3-blue)
![c++](https://img.shields.io/badge/C%2B%2B-20-blue)

## Status

> Em desenvolvimento ativo.

## Motivação

Projeto final da cadeira Computação Gráfica I (CK0245) do curso de Ciência da Computação da Universidade Federal do
Ceará (UFC) que deve ser um renderizador que consiga fazer a exibição de uma cena pensada pelos desenvolvedores.

## Escopo

O projeto final da cadeira tem uma série de requisitos técnicos a ser alcançados, os quais poderão ser alcançados de forma incremental através da resolução das atividades passadas durante a cadeira. Os requisitos do projeto final são:

- Definição do cenário:
	- [ ] Coerência temática (Obrigatório) : O cenário tem de ter coerência temática, isto é, não pode ser um amontoado de objetos
		aleatoriamente distribuídos.
	- [ ] Coordenadas do mundo (Obrigatório) : O cenário deve ser montado de forma que todos os objetos estejam no primeiro octante, isto
		é, as coordenadas dos vértices de todos os objetos terão x, y e z positivos;
	- Objetos:
		- Tipos de objetos (apresentar pelo menos um objeto de cada tipo) (Obrigatório) :
			- [ ] Esfera;
			- [ ] Cilindro;
			- [ ] Cone;
			- [ ] Malha.
		- [ ] Materiais (pelo menos quatro materiais distintos) (Obrigatório);
		- [ ] Textura (pelo menos uma textura aplicada) (Obrigatório)
	- Transformações:
		- [ ] Translação (Obrigatório)
		- Rotação:
			- [ ] Em torno de um dos eixos x, y ou z (Obrigatório)
			- Em torno de um eixo arbitrário (Obrigatório um dos dois métodos)
				- [ ] Mudança de sistemas de coordenadas
				- [ ] Quatérnios
		- [ ] Escala (Obrigatório)
		- [ ] Cisalhamento ($+ 0.5$)
		- [ ] Espelho em relação a um plano arbitrário ($+ 0.5$)
	- Fontes luminosas:
		- [ ] Pontual (Obrigatório)
		- [ ] Spot ($+ 1.0$)
		- [ ] Direcional ($+ 0.5$)
		- [ ] Ambiente (Obrigatório)
- Câmera
  - Permitir a especificação de (Obrigatório)
  	- [ ] Posição da câmera (Eye)
  	- [ ] Direcionamento de visada (At point)
  	- [ ] Orientação da câmara em torno do eixo de visada (Up point)
	- Parâmetros adicionais (Obrigatório)
		- [ ] Distância focal (d)
		- [ ] Campo de visão (definir as coordenadas de câmera da janela: xmin, xmax, ymin, ymax)
- Projeções
	- Perspectiva (Obrigatório)
		- Alterar os parâmetros adicionais da câmera para
			- [ ] aumentar o campo de visão (zoom out) (Obrigatório)
			- [ ] diminuir o campo de visão (zoom in) (Obrigatório)
		- Demonstrar como posicionar a câmera para obter
			- [ ] Perspectiva com um ponto de fuga ($+ 0.5$)
			- [ ] Perspectiva com dois pontos de fuga ($+ 0.5$)
			- [ ] Perspectiva com três ou mais pontos de fuga ($+ 0.5$)
	- [ ] Ortográfica ($+ 0.5$)
	- [ ] Oblíqua ($+ 0.5$)
- [ ] Sombra (Obrigatório)
- Interatividade
	- [ ] Implementar a função de pick (Obrigatório)
	- [ ] Uso de interface gráfica (Bônus de 0.5 a 1.0)
- [ ] Imagem gerada por ray casting com pelo menos 500 x 500 pixels (Obrigatório)
- [ ] Bônus de criatividade e beleza (até 1.0)

## Requisitos

- Compilador com suporte a **C++20** (ou superior)
- Testado com: **Clang**

> Requisitos de desenvolvimento (linters, hooks, CI) estão documentados em [`CONTRIBUTING.md`](./CONTRIBUTING.md).

## Documentação

Toda a documentação do projeto está disponível via Doxygen.

## Uso

O projeto conta com uma executável, use o executável da última release.

## Testes

```bash
cmake -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

## Decisões de arquitetura

Todas as decisões técnicas relevantes estão documentadas como ADRs (formato MADR) em [`docs/decisions/`](./docs/decisions/).

## Contribuindo

Veja [`CONTRIBUTING.md`](./CONTRIBUTING.md) para pré-requisitos de desenvolvimento, hooks de commit e pipeline de CI.

## Licença

Veja o arquivo [LICENSE](./LICENSE) para maiores detalhes.
