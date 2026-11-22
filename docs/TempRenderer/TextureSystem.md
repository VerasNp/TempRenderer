# Sistema de texturas

Documentação para detalhar a implementação de feature Textura

## Fluxo geral

1. `SceneConfig::loadConfig()` percorre os objetos. Para cada objeto,
   `ObjectConfig::loadConfig()` lê o material e `setObjectProps()` chama
   `parseTextureFromConfig()` quando encontra `material.texture`.
2. A função `parseTextureFromConfig()` cria o tipo de textura configurado. No
   caso de `image`, a criação de `ImageTexture` já lê o PPM e armazena seus
   pixels em memória; essa leitura ocorre antes da renderização.
3. `SceneComposer::compose()` transfere o material para a primitiva da cena.
4. Quando um raio atinge uma primitiva, seu método `intersect()` preenche um
   `TextureContext` com a posição, a normal, as UVs e as opções de repetição.
5. `SamplerIntegrator::render()` gera raios para os pixels. Para cada
   interseção, `PhongIntegrator::li()` consulta o material, que amostra a
   textura no contexto daquele ponto.
6. A cor calculada é gravada no `Canvas` e, depois, enviada ao renderer OpenGL
   para exibição.

## Arquivos e funções principais

| Arquivo | Responsabilidade |
| --- | --- |
| [`main.cpp`](../../src/temprenderer/main.cpp#L12-L25) | Ponto de entrada que carrega a configuração do projeto antes de iniciar a aplicação. |
| [`ProjectConfig.cpp`](../../src/temprenderer/core/config/ProjectConfig.cpp#L7-L26) | Lê o JSON do projeto, determina o diretório-base e inicia a leitura da cena. |
| [`SceneConfig.cpp`](../../src/temprenderer/core/config/SceneConfig.cpp#L7-L28) | Percorre os objetos da cena e encaminha o diretório-base para cada `ObjectConfig`. |
| [`ObjectConfig.cpp`](../../src/temprenderer/core/config/ObjectConfig.cpp#L5-L15) e [leitura do material](../../src/temprenderer/core/config/ObjectConfig.cpp#L104-L136) | Constrói a configuração de cada objeto; `setObjectProps()` lê o material e cria a textura configurada. |
| [`utils.hpp`](../../include/temprenderer/core/config/utils.hpp#L47-L124) | Implementa `parseTextureFromConfig()`: valida o tipo, aplica padrões, resolve caminhos relativos e instancia a textura. |
| [`ImageTexture.hpp`](../../include/temprenderer/core/math/ImageTexture.hpp#L10-L36) e [`ImageTexture.cpp`](../../src/temprenderer/core/math/ImageTexture.cpp#L13-L155) | Implementam leitura de PPM, armazenamento dos pixels e amostragem/interpolação da imagem. |
| [`Materials.hpp`](../../include/temprenderer/core/math/Materials.hpp#L52-L98) | Define `TextureContext`, a interface `Texture` e os valores padrão de amostragem de cor, rugosidade e normal. |
| [`Materials.hpp`](../../include/temprenderer/core/math/Materials.hpp#L172-L205) | Define `Material` e encaminha as consultas para sua textura, quando há uma. |
| [`Materials.cpp`](../../src/temprenderer/core/math/Materials.cpp#L83-L255) | Implementa os padrões procedurais de madeira e metal, incluindo variações de rugosidade e normal do metal. |
| [`Sphere.cpp`](../../src/temprenderer/renderer/Sphere.cpp#L14-L50), [`Plane.cpp`](../../src/temprenderer/renderer/Plane.cpp#L7-L37), [`Cylinder.cpp`](../../src/temprenderer/renderer/Cylinder.cpp#L120-L154) e [`Cone.cpp`](../../src/temprenderer/renderer/Cone.cpp#L100-L128) | Calculam interseções e preenchem o `TextureContext`; cada forma define seu próprio mapeamento UV. |
| [`SceneComposer.cpp`](../../src/temprenderer/scene/SceneComposer.cpp#L13-L60) | Cria as primitivas da cena e passa a cada uma o material configurado. |
| [`SamplerIntegrator.hpp`](../../include/temprenderer/renderer/SamplerIntegrator.hpp#L23-L30) e [`PhongIntegrator.cpp`](../../src/temprenderer/renderer/PhongIntegrator.cpp#L8-L37) | Percorrem os pixels e consultam a textura no ponto atingido para calcular a cor iluminada. |
| [`ApplicationManager.cpp`](../../src/temprenderer/ApplicationManager.cpp#L63-L72) e [`RendererManager.cpp`](../../src/temprenderer/renderer/RendererManager.cpp#L111-L125) | Entregam o `Canvas` renderizado para a textura OpenGL de saída e a interface. |

## Tipos de textura disponíveis

O campo `type` de `material.texture` escolhe uma das implementações:

- **`solid`**: devolve sempre a cor configurada em `color`.
- **`image`**: lê uma imagem PPM P3 (texto) ou P6 (binário). `path` é obrigatório;
  caminhos relativos são resolvidos a partir da pasta do JSON. `scale` é
  positivo, finito e vale `1.0` por padrão. O valor maior que 1 repete mais a
  imagem nas direções UV que permitem repetição.
- **`metal`**: gera manchas, riscos e cavidades proceduralmente. Também pode
  variar a rugosidade e perturbar a normal para simular relevo. Se o material
  não definir `metallic`, esse tipo de textura faz o parser usar `1.0`.
- **`wood`**: gera tábuas, juntas, veios e nós usando a posição global x/z; por
  isso, seu padrão não é baseado nas UV da superfície.

Parâmetros e valores padrão são definidos em
[`parseTextureFromConfig()`](../../include/temprenderer/core/config/utils.hpp#L57-L124).
Um tipo explícito que não seja reconhecido gera erro. Se `texture` estiver
ausente do material, não há textura e `Material::sampleColor()` usa `kd`; se o
campo existir, mas não for uma tabela ou não tiver `type`, o parser cria uma
textura sólida branca.

### Exemplo de configuração

```json
"material": {
  "texture": {
    "type": "image",
    "path": "assets/texture_demo.ppm",
    "scale": 1.0
  }
}
```

Para uma imagem que já contém uma esfera renderizada sobre um fundo, é possível
usar `"projection": "sphere_preview"`. Nesse modo, a amostragem usa as
componentes x/y da normal em vez das UV e desativa a repetição, mantendo a região
circular da esfera no lugar do fundo.

## Contexto e coordenadas UV

`TextureContext` reúne posição global (`worldPosition`), posição local
(`localPosition`), normal, coordenadas UV e os flags `repeatU`/`repeatV`. As UV
indicam que região da imagem corresponde ao ponto da superfície; os flags
definem se a coordenada deve repetir ou ficar limitada às bordas.

O mapeamento depende da forma:

- **Esfera:** U acompanha a longitude e V a latitude; V não repete.
- **Plano:** U e V são projeções da posição local sobre vetores tangente e
  bitangente.
- **Cilindro:** a lateral usa ângulo e altura; as tampas usam uma projeção
  circular sem repetição.
- **Cone:** a lateral usa ângulo e altura; a base usa uma projeção circular sem
  repetição.

Para imagens, `ImageTexture::sample()` aplica `scale`, repete ou limita U/V
conforme o contexto e interpola bilinearmente os quatro pixels vizinhos. Assim,
a cor varia suavemente entre os pixels da imagem.

## Amostragem pelo material e iluminação

`Material::sampleColor()` chama `Texture::sample()` quando há textura; sem
textura, retorna `kd`. A cor retornada pela textura é usada diretamente como
cor-base, não multiplicada por `kd`. `sampleRoughness()` e `sampleNormal()`
também podem delegar à textura: a implementação padrão mantém os valores do
material e a normal original, enquanto `MetalTexture` pode variá-los.

No `PhongIntegrator::li()`, o ponto de interseção fornece tanto o material
quanto o `TextureContext`. O integrador consulta cor, normal e rugosidade e usa
esses valores no cálculo de iluminação. O `Canvas` final é então apresentado
pela janela; os shaders OpenGL exibem a imagem pronta e não aplicam as texturas
dos objetos.

## Exemplo no projeto

[`texture_material_demo.json`](../../texture_material_demo.json) mostra uma
imagem PPM tileable, metal procedural, uma imagem com `sphere_preview` e madeira
procedural. Os testes de coordenadas UV estão em
[`texture_coordinates_test.cpp`](../../tests/renderer/texture_coordinates_test.cpp#L17-L76).
