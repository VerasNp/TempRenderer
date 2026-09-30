# Montando uma Half Edge com Wavefront

No contexto do projeto os arquivos `(.obj)` exportados na sua grande maioria pelo blender podem ser úteis para facilitar na definição de malhas complexas.

O projeto trabalha com um `ConfigValue` que é uma estrutura que representa justamente a configuração colocada em um arquivo.

O arquivo wavefront é apresentado com a seguinte configuração:

```obj
# Blender 5.2.1 LTS
# www.blender.org
mtllib Untitled.mtl
o Cube
v 1.000000 1.000000 -1.000000
v 1.000000 -1.000000 -1.000000
v 1.000000 1.000000 1.000000
v 1.000000 -1.000000 1.000000
v -1.000000 1.000000 -1.000000
v -1.000000 -1.000000 -1.000000
v -1.000000 1.000000 1.000000
v -1.000000 -1.000000 1.000000
vn -0.0000 1.0000 -0.0000
vn -0.0000 -0.0000 1.0000
vn -1.0000 -0.0000 -0.0000
vn -0.0000 -1.0000 -0.0000
vn 1.0000 -0.0000 -0.0000
vn -0.0000 -0.0000 -1.0000
vt 0.625000 0.500000
vt 0.875000 0.500000
vt 0.875000 0.750000
vt 0.625000 0.750000
vt 0.375000 0.750000
vt 0.625000 1.000000
vt 0.375000 1.000000
vt 0.375000 0.000000
vt 0.625000 0.000000
vt 0.625000 0.250000
vt 0.375000 0.250000
vt 0.125000 0.500000
vt 0.375000 0.500000
vt 0.125000 0.750000
s 0
usemtl Material
f 1/1/1 5/2/1 7/3/1 3/4/1
f 4/5/2 3/4/2 7/6/2 8/7/2
f 8/8/3 7/9/3 5/10/3 6/11/3
f 6/12/4 2/13/4 4/5/4 8/14/4
f 2/13/5 1/1/5 3/4/5 4/5/5
f 6/11/6 5/10/6 1/1/6 2/13/6
```

- `mtllib <string>`: Referência ao arquivo `.mtl`;
- `o <string>`: Nome do objeto
- `v <float> <float> <float>`: Define a posição de um vértice. São usadas três números de ponto flutuante para isso;
- `vn <float> <float> <float>`: Representa o vetor normal associado a um vértice, representado através de três números de ponto flutuante;
- `vt <float> <float>`: Coordenada de textura de coordenada, conhecida como coordenada UV, normalmente representado por dois números de ponto flutuante, usado para determinar como pintar a superfície tridimensional com pixels de um mapeamento de textura 2D;
- `s 1/0`: Suavização ou não;
- `usemtl <string>`: Define a alteração de material para as faces descritas em seguida
- `f v/vn/vt`: Descreve uma face

Tendo em vista esse fato é necessário agora parsear para que dessa maneira seja possível retirar apenas as informações necessárias para a montagem da malha. Dentre as informações no arquivo as relevantes serão:

- Lista de vértices;
- Descrição da composição das faces;

Por via das dúvidas as outras informações serão posteriormente alocadas.
