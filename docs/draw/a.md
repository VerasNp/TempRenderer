## Descrição

Escreva um código, baseado na discussão das duas primeiras aulas, que pinte uma esfera no canvas. Os requisitos são os seguintes:

### 1. Definir janela

### 2. Definir câmera

- A origem deve estar em $P(0,0,0)$

### 3. Definir raio

### 4. Definir esfera

### 5. Montar cena

- A câmera deve estar em posição `Ponto3D(0,0,0)`
- O background deve ter cor `RGB(100, 100, 100)`;
- A esfera deve ter as seguintes propriedades:
  - Deve ter cor `RGB(255, 0, 0)`;
  - O centro dela deve estar sob o eixo `z`, onde a coordenada `z = window.distance + sphere,radius`
- A viewport:
  - Deve estar localizada com seu centro em `Point3D(0,0,-window.distance)`
