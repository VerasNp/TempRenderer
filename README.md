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

### Texturas de imagem

Materiais podem usar imagens PPM nos formatos P3 ou P6. O caminho relativo da imagem é resolvido a partir da pasta do arquivo JSON do projeto. `scale` controla a repetição da imagem nos eixos UV.

Para uma textura tileable, omita `projection`. Imagens que mostram uma esfera renderizada sobre um fundo podem usar `"projection": "sphere_preview"`; isso projeta apenas a área circular da esfera, sem espalhar o fundo preto pela superfície.

```json
"texture": {
	"type": "image",
	"path": "assets/texture_demo.ppm",
	"scale": 1.0
}
```

Para ver a imagem de demonstração aplicada a uma esfera, um cilindro e um cone, execute a partir da raiz do projeto:

```bash
make run PROJECT=image_texture_demo.json
```

O arquivo [image_texture_demo.json](image_texture_demo.json) reutiliza `assets/texture_demo.ppm` nos três objetos. As coordenadas UV são projetadas para esferas, planos, laterais e tampas de cilindros e cones. O exemplo ativo padrão, `tarefa03.json`, usa uma textura procedural de aço galvanizado desgastado na esfera e mantém a textura de madeira no piso. A textura `metal` oferece controles `mottling`, `scratch_density`, `pit_strength`, `roughness_variation` e `bump_strength`; a rugosidade base é definida no material. Luzes retangulares usam `type: "area"` e aceitam `width` e `height` para highlights mais largos e sombras suaves.

Para comparar uma textura de imagem, metal e madeira na mesma renderização, execute:

```bash
make run PROJECT=texture_material_demo.json
```

O arquivo [texture_material_demo.json](texture_material_demo.json) aplica uma textura PPM tileable à esfera da esquerda, metal galvanizado procedural ao cilindro, a imagem de uma esfera renderizada à esfera da direita com `sphere_preview` e madeira procedural ao piso.

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
