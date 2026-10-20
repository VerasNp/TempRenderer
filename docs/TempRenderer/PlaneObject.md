## Plano

```json
{
	// Nome do componente objeto
	"name": "<string>",
	"type": "plane",
	// Ponto conhecido do plano
	"point": {
		"x": <float>,
		"y": <float>,
		"z": <float>
	},
	// Normal do plano
	"normal": {
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
},
```
