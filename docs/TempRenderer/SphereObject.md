## Esfera

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
