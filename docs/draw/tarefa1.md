## Descrição

Escreva um código, baseado na discussão das duas primeiras aulas, que pinte uma esfera no canvas. Os requisitos são os seguintes:

### 1. Definir janela

> A largura da janela, em metros, será armazenada na variável `wJanela`
>
> A altura da janela, em metros, será armazenada na variável `hJanela`
>
> O centro da janela está sobre o eixo `z` do sistema de coordenadas na posição `(0, 0, - dJanela)` em metros. Assim, todos os pontos no plano da Janela terão coordenada `z = - dJanela`.

A janela aqui que ele se refere, na literatura é a **viewport**.

### 2. Definir câmera

>  O olho do pintor está na origem do sistema de coordenadas `(0,0,0)`

Aqui será a posição em que os raios partirão, ou seja, a câmera gera um raio que vai se dirigir até uma pixel do viewport com de coluna `col` e linha `lin`.

A câmera no caso precisa saber mais sobre essa viewport, que nas aulas do Creto é a "janela" e assim deve conhecer onde se encontra o centro de cada um dos "quadrados" da tela de galinheiro, mais abaixo será descrito como isso é feito.

### 3. Definir esfera

> O raio da esfera deve ser armazenado na variável `rEsfera`
>
> O centro da esfera deve estar sobre o eixo z com coordenada `z = (dJanela + rEsfera)`
>
> A cor da esfera deve ser `esfColor = 255, 0, 0`

A esfera é no caso uma forma geométrica que no caso terá um raio definido pelo usuário, enquanto que seu centro estará em um ponto `Ponto3D(0, 0, (dJanela + rEsfera))` e cor `RGB(255, 0, 0)`.

>  A cor de background deve ser cinza `bgColor = 100, 100, 100`

A cor de background, ou seja, quando o raio não interceptar a esfera deve ser de `RGB(100, 100, 100)`

### 4. Definindo canvas

> Defina o número de colunas `nCol` e o número de linhas `nLin` da matriz de cores da imagem.

Aqui é pedido um certo número de linhas e colunas para a matriz de cores de image. Estaremos assim definindo a resolução da nossa imagem.

> `nCol` representa tanto o número de colunas na tela de mosquito que está presa na Janela, quanto o número de colunas do quadriculado que o pintor marcou a lápis no Canvas de pintura.
>
> `nLin`, de maneira análoga, representa tanto o número de linhas na tela de mosquito que está presa na Janela, quanto o número de linhas do quadriculado que o pintor marcou a lápis no Canvas de pintura

Aqui aparecem dois conceitos "tela de mosquito" e "Canvas", um deles é realmente o nome que leva, canvas, que será aquilo que irá também levar mais em conta a resolução escolhida (número de colunas e linhas) enquanto que o outro seria o conhecido viewport, uma moldura da janela que ao se misturar com a quantidade de colunas e linhas (resolução) temos o conceito de "tela de mosquitos".

> As dimensões dos retângulos da tela de mosquito são:
>
> `Dx = wJanela/nCol`
>
> `Dy = hJanela/nLin`

Aqui a grade de pixels será então calculado o quão grande ou pequeno o pixel será.

> As coordenadas do ponto do centro de um retângulo da tela de mosquito correspondente ao retângulo (riscado a lápis) na posição (l, c) é dado por
>
> $x = \frac{- wJanela}{2} + \frac{Dx}{2}  + c  \times Dx$
>
> $y =  \frac{hJanela}{2}  -  \frac{Dy}{2}  -  l  \times Dy$

O raio deve passar pelo centro do pixel passando assim pelo ponto `Ponto3D(x, y, -dJanela)`
