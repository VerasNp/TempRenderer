## Descrição

A câmera é um ponto no espaço 3D de onde todos os raios da cena irão ser originados.

A partir da câmera também é onde será calculado o viewport, onde segundo o Ray tracing in one weekend:

> "Um retângulo virtual que contêm um grid dos locais de pixels da imagem"

Isso nada mais é que a ideia da telha de galinheiro que o Creto apresenta.

Outro detalhe disso é a questão do aspect ratio. Segundo o RTIOW:

> É a razão entre largura e altura de uma imagem

Esse detalhe também se aplica ao viewport, onde:

> Se os pixels estiverem espaçados na mesma distância horizontalmente como estão na vertical, a janela (viewport) de visualização que os limita terá a mesma proporção que a imagem renderizada (canvas)

Isso é, se o pixel for quadrado então o viewport é proporcional ao canvas. A `Canvas` também será onde definiremos a **resolução** da nossa imagem, isso é o "nCol" ou número de colunas e "nLin" ou número de linhas "que o pintor marcou a lápis no Canvas de pintura", essa é uma configuração que poderia entrar nas configurações gerais `application_config.toml`.

No viewport podemos escolher um valor arbitrário `2`, isso significa então que "minha janela vai ter 2 metros"
