## Esfera

### Configuração
```json
{
	// Nome do componente objeto
	"name": "<string>",
	"type": "sphere",
	// Raio da esfera
	"radius": <float>,
	// Ponto central da esfera
	"center": {
		"x": <float>,
		"y": <float>,
		"z": <float>
	},
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
```

A esfera nesse caso será dada usando a seguinte fórmula:

$$
t_I^2 + 2 t_I (\langle \vec{v}, \vec{d_r} \rangle) + \langle \vec{v}, \vec{v} \rangle - R_S^2 = 0
$$

O que faz com que os coeficientes da equação do segundo grau sejam:

$$
\begin{align*}
a &= 1 \\
b &= 2 (\langle \vec{v}, \vec{d_r} \rangle) \\
c &= \langle \vec{v}, \vec{v} \rangle - R_S^2
\end{align*}
$$

Ao se descobrir o ponto de interseção $P_I$ é usado para calcular o ponto normal do ponto:

$$
	\vec{n} = - \frac{C_S - P_I}{||C_S - P_I||}
$$
